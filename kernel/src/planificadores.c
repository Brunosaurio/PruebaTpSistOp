#include <utils/hello.h>
#include "planificadores.h"

pthread_mutex_t mutex_core;
pthread_mutex_t mutex_pausa;
pthread_mutex_t mutex_New;
sem_t semProcesoEnNew;

int contadorMultiprogramacion;

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

/////////Planificador Largo plazo

void planificador_largoPlazo(){
	t_pcb* un_pcb = NULL;
    
    
    if(configKernel->GRADO_MULTIPROGRAMACION >= contadorMultiprogramacion){
        t_pcb* pcb
    }
}

bool iniciar_proceso_en_kernel(int pid){
    t_pcb* procesoNuevo = malloc(sizeof(t_pcb));
    procesoNuevo->pid = pid;
    procesoNuevo->estado = NEW;
    procesoNuevo->programCounter=0;
    procesoNuevo->registros_CPU = malloc(sizeof(t_registros));
    procesoNuevo->quantum = 0;
    log_info(loggerKernel,"Se crea el proceso <%d> en NEW", pid);
    list_add(procesosEnNew, procesoNuevo);
}

/////////Planificador Corto plazo

void pausador(){
	pthread_mutex_lock(&mutex_pausa);
	if(var_pausa == 1){
		log_info(kernel_log_obligatorio, "PAUSA DE PLANIFICACIÓN"); // --> Tiene que ser log_info, por ahora lo dejamos asi para que se note
		sem_wait(&sem_pausa);
	}
	pthread_mutex_unlock(&mutex_pausa);
}

void pcp_planificar_corto_plazo(){
	pausador();
	int flag_lista_ready_vacia = 0;

	pthread_mutex_lock(&mutex_lista_ready);
	if(list_is_empty(lista_ready)){
		flag_lista_ready_vacia = 1;
	}
	pthread_mutex_unlock(&mutex_lista_ready);

	if(flag_lista_ready_vacia == 0){

		switch (ALGORITMO_PLANIFICACION) {
			case FIFO:
				_atender_RR_FIFO();
				break;
			case ROUNDROBIN:
				_atender_RR_FIFO();
				break;
			case PRIORIDADES:
				_atender_PRIORIDADES();
				break;
			default:
				log_error(kernel_logger, "ALGORITMO DE CORTO PLAZO DESCONOCIDO");
				exit(EXIT_FAILURE);
				break;
		}
	}
}
