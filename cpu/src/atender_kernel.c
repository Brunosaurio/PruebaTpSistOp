#include "atender_kernel.h"

extern t_log* cpuLogger;
extern t_cpu_config* cpuConfig;
/*
void conexion_kernel_cpu()
{
	log_info(cpuLogger, "Estoy corriendo");
    

} 

int serverCpu_escuchar_kernel(int server_socket, t_log* logger) {
	int cliente_socket = esperar_cliente(server_socket, logger);
	//log_info(logger, "Hola %d", cliente_socket);
	if (cliente_socket != -1) {
		pthread_t hilo;
		pthread_create(&hilo, NULL, (void*) conexion_kernel_cpu, NULL);
		pthread_detach(hilo);
		return 1;
	}
	return 0;
}*/

t_contexto* crear_contexto(uint32_t pid, uint32_t programCounter){
    t_contexto *contexto = malloc(sizeof(t_contexto));
    contexto->pid = pid;
    contexto->programCounter = programCounter;
    contexto->registrosDeCPU = malloc(sizeof(t_registros));
    return contexto;
}

void prueba_mostrar_contexto(t_contexto* contexto){
	log_info(cpuLogger, "Contexto recibido: PID %d", contexto->pid);
	log_info(cpuLogger, "PC %d", contexto->programCounter);
	log_info(cpuLogger, "AX %d", contexto->registrosDeCPU->AX);
}

void rastrear_instruccion(int pid, int programCounter){
    log_info(cpuLogger, "Rastreando instruccion %d, del proceso %d", pid, programCounter);
    t_buffer* datosDeBusqueda = buffer_crear();
    buffer_empaquetar(datosDeBusqueda, &pid, sizeof(int));
    buffer_empaquetar(datosDeBusqueda, &programCounter, sizeof(int));
    stream_enviar_buffer(cpuConfig->SOCKET_MEMORIA, HEADER_CPU_FETCH, datosDeBusqueda);
    //Tengo que pedir una instruccion a Memoria
}

void decodificar_instruccion(){
    //recibo alguna estructura que contenga todos los datos necesarios de la instruccion
}

bool ejecutar_instruccion(){
    return true;//Modifico lo que sea que daba modificar
}
 void check_interrupt(){
    //revisar si hay alguna interrupcion pendiente por atender. Como tratamos las interrupciones?
}

bool cpu_ejecutar_ciclo_de_instruccion(t_contexto* contexto) {
    bool parar = false;
    rastrear_instruccion(contexto->pid,contexto->programCounter); //fetch
    decodificar_instruccion(); //decode
    parar = ejecutar_instruccion(); //execute
    check_interrupt();
    return parar;
}




void atender_peticiones_kernel(){
	uint32_t pidRecibido = 0;
    uint32_t programCounter = 0;
    log_info(cpuLogger, "Esperando Operacion de Kernel");

    while(1) {
        uint8_t kernelRespuesta = stream_recibir_header(cpuConfig->SOCKET_KERNEL_DISPATCH);
        t_buffer* bufferContexto = NULL;
        t_contexto* contexto = NULL;
        if (kernelRespuesta != HEADER_proceso_a_ejecutar) {
            log_error(cpuLogger, "Error al intentar recibir el CONTEXTO de KERNEL. Recibido %d ", kernelRespuesta);
            exit(-1);
        }
            bufferContexto = buffer_crear();
            stream_recibir_buffer(cpuConfig->SOCKET_KERNEL_DISPATCH, bufferContexto);
            buffer_desempaquetar(bufferContexto, &pidRecibido, sizeof(pidRecibido));
            buffer_desempaquetar(bufferContexto, &programCounter, sizeof(programCounter));
            
			contexto = crear_contexto(pidRecibido, programCounter);
            
            //buffer_desempaquetar_registros(bufferContexto, contexto->registrosDeCPU);
            buffer_destruir(bufferContexto);
			
			prueba_mostrar_contexto(contexto);
			
            bool pararDeEjecutar = false;
            while (!pararDeEjecutar) {
                pararDeEjecutar = cpu_ejecutar_ciclo_de_instruccion(contexto);
            }
            contexto_destruir(contexto);
    }
}