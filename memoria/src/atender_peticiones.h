#include <utils/hello.h>
#include "procesoEnMemoria.h"

struct interfaz{
    int socketInterfaz;
    char* nombreInterfaz;
}typedef interfaz_info;

void procesar_conexiones_memoria();
void atender_proceso_nuevo();
int server_escuchar_nuevas_conexiones(int server_socket, t_log* logger);
void* escuchar_conexiones_IO(int* socketServidor);



int serverMemoria_escuchar_kernel(int server_socket, t_log* logger);
void conexion_kernel_memoria();

int serverMemoria_escuchar_cpuDispatch(int server_socket, t_log* logger);
void conexion_cpuDispatch_memoria();

int serverMemoria_escuchar_cpuInterrupt(int server_socket, t_log* logger);
void conexion_cpuInterrupt_memoria();