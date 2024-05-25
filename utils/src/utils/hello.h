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
#include <commons/string.h>
#include <stdint.h>
#include <inttypes.h>
#include <readline/readline.h>
#include "protocolo.h"

typedef struct t_registros{
    uint32_t PC;
    uint8_t AX;
    uint8_t BX;
    uint8_t CX;
    uint8_t DX;
    uint32_t EAX;
    uint32_t EBX;
    uint32_t ECX;
    uint32_t EDX;
    uint32_t SI;
    uint32_t DI;
}t_registros;

typedef struct t_contexto {
    uint32_t pid;
    uint32_t programCounter;
    t_list* instrucciones;
    t_registros registrosDeCPU;
}t_contexto;

typedef struct t_pcb {
    uint32_t pid;
    uint32_t tamanio;
    t_buffer* instrucciones;
    uint32_t programCounter;
    
    time_t tiempoDellegadaAReady;
    
    uint32_t tiempoDeBloqueo;
    uint32_t tiempoEjecutado;
    
    t_registros registros_CPU;

    uint8_t estado;
    t_list* tablaDeArchivosAbiertos;
}t_pcb;

/**
* @fn    decir_hola
* @brief Imprime un saludo al nombre que se pase por parámetro por consola.
*/
void decir_hola(char* quien);

int crear_conexion_servidor(char* puerto);
int esperar_cliente(int socket_servidor, t_log* logger);
int crear_conexion_cliente(char *ip, char* puerto);

#endif
