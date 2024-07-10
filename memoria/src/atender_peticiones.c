#include "atender_peticiones.h"

t_log* loggerMemoria;
t_memoria_config* configMemoria;
t_list* interfacesConectadas;
t_list* procesosEnMemoria;
int socketKernel;
int socketCpu;

///////////////////////Hilos
void* escuchar_conexiones_IO(int* socketMemoria){
    //prueba* algo = (prueba*) recibiendo;
    //printf("hola %d", recibiendo->unParametro);
	//printf("hola %s", recibiendo->segundoParametro);
	while(1){
		int socketCliente = esperar_cliente(socketMemoria, loggerMemoria);
		handshake_Inicial_SV(socketCliente);
		char* nombreInterfaz = malloc(sizeof(*nombreInterfaz));
		t_buffer* buffer = buffer_crear();
		uint8_t a = stream_recibir_header(socketCliente);
		stream_recibir_buffer(socketCliente, buffer);
		buffer_desempaquetar_string(buffer, &nombreInterfaz);
		log_info(loggerMemoria, "%s", nombreInterfaz);

		interfaz_info* nuevaInterfaz = malloc(sizeof(interfaz_info));
		nuevaInterfaz->nombreInterfaz = strdup(nombreInterfaz);
		nuevaInterfaz->socketInterfaz = socketCliente;

		list_add(interfacesConectadas, nuevaInterfaz);
	}

}

////////////////////////Procesar conexiones y peticiones

void atender_proceso_nuevo(){
	t_buffer* buffer = buffer_crear();
	t_proceso_en_memoria* nuevoProceso = malloc(sizeof(t_proceso_en_memoria));
	char* pathInstrucciones = malloc(sizeof(char*));
	t_list* instrucciones = list_create();
	int pid;
	//recibimos informacion
	stream_recibir_buffer(socketKernel, buffer);
	buffer_desempaquetar(buffer, &pid, sizeof(int));
	buffer_desempaquetar_string(buffer, &pathInstrucciones);

	log_info(loggerMemoria, "Creando estructuras del <%d> path <%s>", pid, pathInstrucciones);
	
	//Procesamos instrucciones
	instrucciones = leer_archivo_y_cargar_instrucciones(instrucciones, pathInstrucciones);
	
	nuevoProceso->pid = pid;
	nuevoProceso->instrucciones = instrucciones;
	//Agregamos el proceso a los procesos Activos en la memoria. CUIDADO, LAS INSTRUCCIONES HAY Q HACERLE GET AL REVEZ
	list_add(procesosEnMemoria,nuevoProceso);
	buffer_destruir(buffer);
	//Devolvemos la confirmacion del proceso nuevo creado
	//sleep(configMemoria->retardoRespuesta);
	stream_enviar_buffer_vacio(socketKernel, CONF_PR_NUEVO);
}
void procesar_conexiones_memoria(){
	log_info(loggerMemoria, "Estoy corriendo");
	//printf("Te odia chupavergas");
	//int *args = (int*) void_args;
	/**int cliente_socket = 1;//*args;;
	op_code cop;
    printf("Llegue aca");
	t_list lista;
	while (cliente_socket != -1) {
		cop = recibir_operacion(cliente_socket);
		if (cop == -1) {
			log_info(loggerMemoria, "El cliente se desconecto del server");
			return;
		}
		switch (cop) {
            case NUEVA_INSTANCIA_IO:
            log_info(loggerMemoria, "nueva instancia de io conectada");
            break;
			case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
			case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
            default: log_info(loggerMemoria, "q decis bobo?");
	    }
	    log_warning(loggerMemoria, "El cliente se desconecto de server");
	    return;
    }*/
}
int server_escuchar_nuevas_conexiones(int server_socket, t_log* logger) {
	int cliente_socket = esperar_cliente(server_socket, logger);
	if (cliente_socket != -1) {
		pthread_t hilo;
		pthread_create(&hilo, NULL, (void*) procesar_conexiones_memoria, NULL);
		pthread_detach(hilo);
		return 1;
	}
	return 0;
}

void conexion_kernel_memoria()
{
	while(1){
		cod_operacion cop = stream_recibir_header(socketKernel);
		//cod_operacion cop = recibir_operacion(socketKernel);
		//log_info(loggerMemoria, "Recibido el codigo de operacion: %d", cop);
		switch(cop){
			case PROCESO_NUEVO:
				atender_proceso_nuevo();
				break;
			default:
				log_info(loggerMemoria, "Recibido el codigo de operacion: %d no reconocido", cop);
				break;
		}
	}
}

int serverMemoria_escuchar_kernel(int server_socket, t_log* logger) {
	int cliente_socket = esperar_cliente(server_socket, logger);
	//log_info(logger, "Hola %d", cliente_socket);
	if (cliente_socket != -1) {
		pthread_t hilo;
		pthread_create(&hilo, NULL, (void*) conexion_kernel_memoria, NULL);
		pthread_join(hilo, NULL);
		return 1;
	}
	return 0;

}

void enviar_instruccion_a_Cpu(){
	int pid = 0;
    int programCounter = 0;

	t_buffer* datosDeBusqueda = buffer_crear();
	stream_recibir_buffer(configMemoria->SOCKET_CPU, datosDeBusqueda);
	buffer_desempaquetar(datosDeBusqueda,&pid, sizeof(int));
	buffer_desempaquetar(datosDeBusqueda,&programCounter,sizeof(int));
	log_info(loggerMemoria, "Recibido fetch de instruccion %d, del proceso %d", programCounter,pid);
	exit(-1);
}

void conexion_cpuDispatch_memoria()
{
	log_info(loggerMemoria, "Estoy corriendo");
	while(1){
		cod_operacion cop = stream_recibir_header(configMemoria->SOCKET_CPU);
		switch(cop){
			case HEADER_CPU_FETCH:
				enviar_instruccion_a_Cpu();
				break;
			default:
				log_info(loggerMemoria, "Recibido el codigo de operacion: %d no reconocido", cop);
				break;
		}
	}
} 

int serverMemoria_escuchar_cpuDispatch(int server_socket, t_log* logger) {
	int cliente_socket = esperar_cliente(server_socket, logger);
	//log_info(logger, "Hola %d", cliente_socket);
	if (cliente_socket != -1) {
		pthread_t hilo;
		pthread_create(&hilo, NULL, (void*) conexion_cpuDispatch_memoria, NULL);
		pthread_detach(hilo);
		return 1;
	}
	return 0;

}





