#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include <pthread.h>
#include <unistd.h>


void conexion_kernel_cpu();
int serverCpu_escuchar_kernel(int server_socket, t_log* logger);