#include <utils/hello.h>
#include "planificadores.h"

pthread_mutex_t mutex_core;
pthread_mutex_t mutex_pausa;

pthread_mutex_t mutex_New;
pthread_mutex_t mutex_Ready;

sem_t semProcesoEnNew;
sem_t semProcesoEnReady;
sem_t semPcbEnExec;
sem_t semGradoMultiprogramacion;
sem_t dispatchPermitido;

pthread_mutex_t siguientePIDmutex;

uint32_t siguientePID = 0;

extern t_log* loggerKernel;
extern t_kernel_config* configKernel;

//Colas/Listas//
extern t_list* pcbEnExec;
extern t_list* procesosEnNew;
extern t_list* procesosEnReady;
extern t_list* procesosEnBloq;
extern t_list* procesosEnExit;

uint32_t obtener_siguiente_pid(){
    pthread_mutex_lock(&siguientePIDmutex);
    uint32_t nuevoPID = siguientePID++;
    pthread_mutex_unlock(&siguientePIDmutex);
    return nuevoPID;
}

void loggear_cambio_estado(const char* prev, const char* post, int pid) {
    char* cambioDeEstado = string_from_format("\e[1;93m%s->%s\e[0m", prev, post);
    log_info(loggerKernel, "cambio de estado de %s de PCB con ID %d", cambioDeEstado, pid);
    free(cambioDeEstado);
}

/////////Planificador Largo plazo

void iniciar_proceso_en_kernel(char* path){
    int pid = obtener_siguiente_pid();
	t_pcb* procesoNuevo = malloc(sizeof(t_pcb));
    
	procesoNuevo->pid = pid;
    procesoNuevo->estado = NEW;
    procesoNuevo->programCounter=0;
    procesoNuevo->registros_CPU = malloc(sizeof(t_registros));
    procesoNuevo->quantum = 0;
    procesoNuevo->pathInstrucciones = path;
	
    list_add(procesosEnNew, procesoNuevo);
	log_info(loggerKernel,"Se crea el proceso <%d> en NEW", pid);
	sem_post(&semProcesoEnNew);
}

void iniciar_proceso_en_memoria(t_pcb* unPcb){
    t_buffer* a_enviar = buffer_crear();
    
    buffer_empaquetar(a_enviar, &unPcb->pid, sizeof(int));
    buffer_empaquetar_string(a_enviar, unPcb->pathInstrucciones);
    
    log_info(loggerKernel, "%s", unPcb->pathInstrucciones);
    
	stream_enviar_buffer(configKernel->SOCKET_MEMORIA, PROCESO_NUEVO, a_enviar);
    
	buffer_destruir(a_enviar);
}


void  planificador_largo_plazo(void) {
    /*pthread_t liberarPcbsEnExitHilo;
    pthread_create(&liberarPcbsEnExitHilo, NULL, (void*)finalizar_pcbs_en_hilo_con_exit, NULL);
    pthread_detach(liberarPcbsEnExitHilo);*/
    while(1) {
        sem_wait(&semProcesoEnNew);
        sem_wait(&semGradoMultiprogramacion);
        //sem_wait(&controlDeIntercambioDePcbs);

        pthread_mutex_lock(&mutex_New);
        t_pcb* pcbQuePasaAReady = list_remove(procesosEnNew, 0);
        pthread_mutex_unlock(&mutex_New);
        
        pthread_mutex_lock(&mutex_Ready);
        list_add(procesosEnReady, pcbQuePasaAReady);
        pthread_mutex_unlock(&mutex_Ready);
        
		////// PARTE DE MEMORIA 
        iniciar_proceso_en_memoria(pcbQuePasaAReady);
        
		t_buffer* bufferSegmentoCreado = buffer_crear();
        uint8_t respuestaDeMemoria = stream_recibir_header(configKernel->SOCKET_MEMORIA);
        if (respuestaDeMemoria != CONF_PR_NUEVO) {
            log_error(loggerKernel,"Error al intentar iniciar estructuras del proceso %d",pcbQuePasaAReady->pid);
            exit(-1);
        }
        //stream_recibir_buffer(kernel_config_obtener_socket_memoria(kernelConfig), bufferSegmentoCreado);
        //buffer_desempaquetar_tabla_de_segmentos(bufferSegmentoCreado,pcb_obtener_tabla_de_segmentos(pcbQuePasaAReady),cantidadDeSegmentos);
        buffer_destruir(bufferSegmentoCreado);

        log_info(loggerKernel, "Proceso %d cargado en MEMORIA", pcbQuePasaAReady->pid);
        ///////////
        loggear_cambio_estado("NEW", "READY", pcbQuePasaAReady->pid);
        sem_post(&semProcesoEnReady);
        pcbQuePasaAReady = NULL;
    }
}

/////////Planificador Corto plazo

void planificador_corto_plazo() {
    /*pthread_t atenderPCBHilo;

    pthread_create(&atenderPCBHilo, NULL, (void*)atender_pcb, NULL);
    pthread_detach(atenderPCBHilo);*/

    while(1) {
        t_pcb* pcbToDispatch;
            
        sem_wait(&dispatchPermitido); // fijarse si la cpu esta libre
        log_info(loggerKernel, "Se permite dispatch");

        sem_wait(&semProcesoEnReady);
		log_info(loggerKernel, "Se toma una instancia de READY");
		//algoritmoConfigurado == ALGORITMO_FIFO
        if(strcmp(configKernel->ALGORITMO_PLANIFICACION,"FIFO") == 0){

			pthread_mutex_lock(&mutex_Ready);
    		pcbToDispatch = (t_pcb*) list_remove(procesosEnReady,0);
    		pthread_mutex_unlock(&mutex_Ready);

        }else{
			//otro algoritmo
            //pcbToDispatch = iniciar_HRRN(estadoReady, kernel_config_obtener_hrrn_alfa(kernelConfig));
        }
		list_add(pcbEnExec,pcbToDispatch);
        sem_post(&semPcbEnExec);
    }
}