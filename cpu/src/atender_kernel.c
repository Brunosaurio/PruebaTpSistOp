#include "atender_kernel.h"

extern t_log* cpuLogger;

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

}