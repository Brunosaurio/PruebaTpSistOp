#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include <pthread.h>
#include <unistd.h>

int serverKernel_escuchar_entradaSalida(int server_socket, t_log* logger);
void conexion_entradaSalida_kernel();