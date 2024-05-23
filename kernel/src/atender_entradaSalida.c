#include "atender_entradaSalida.h"

extern t_log* loggerKernel;
/*
void conexion_entradaSalida_kernel()
{
	log_info(loggerKernel, "Estoy corriendo");


} 

int serverKernel_escuchar_entradaSalida(int server_socket, t_log* logger) {
	int cliente_socket = esperar_cliente(server_socket, logger);
	//log_info(logger, "Hola %d", cliente_socket);
	if (cliente_socket != -1) {
		pthread_t hilo;
		pthread_create(&hilo, NULL, (void*) conexion_entradaSalida_kernel, NULL);
		pthread_detach(hilo);
		return 1;
	}
	return 0;

}*/