#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include <pthread.h>
#include <unistd.h>
#include "ejecutar_intrucciones.h"


void conexion_kernel_cpu();
int serverCpu_escuchar_kernel(int server_socket, t_log* logger);
void atender_peticiones_kernel();
void prueba_mostrar_contexto(t_contexto* contexto);