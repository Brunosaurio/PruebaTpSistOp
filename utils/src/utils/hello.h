#ifndef UTILS_HELLO_H_
#define UTILS_HELLO_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <commons/config.h>
#include <commons/log.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "mensajes.h"
#include "stream.h"
#include <commons/collections/list.h>
#include <stdint.h>
#include <inttypes.h>


/**
* @fn    decir_hola
* @brief Imprime un saludo al nombre que se pase por parámetro por consola.
*/
void decir_hola(char* quien);

int crear_conexion_servidor(char* puerto);
int esperar_cliente(int socket_servidor, t_log* logger);
int crear_conexion_cliente(char *ip, char* puerto);

#endif
