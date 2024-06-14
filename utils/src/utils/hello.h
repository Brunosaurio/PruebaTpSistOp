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
#include <semaphore.h>
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
    t_registros* registrosDeCPU;
}t_contexto;

typedef enum{
	NEW,//=0
	READY,//=1
	EXEC,//=2
	BLOCKED,//=3
	EXIT//=4
}est_pcb;

typedef struct t_pcb {
    uint32_t pid;
    uint32_t programCounter;
    t_registros* registros_CPU;
    uint8_t quantum;
    est_pcb estado;
    char* pathInstrucciones;
}t_pcb;



struct t_kernel_config {
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* PUERTO_ESCUCHA;
    char* IP_CPU;
    char* PUERTO_CPU_DISPATCH;
    char* PUERTO_CPU_INTERRUPT;
    char* ALGORITMO_PLANIFICACION;
    int GRADO_MULTIPROGRAMACION;
    int QUANTUM;

    int SOCKET_CPU;
    int SOCKET_MEMORIA;
}typedef t_kernel_config;

struct cpu_config {
    
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* PUERTO_ESCUCHA_DISPATCH;
    char* PUERTO_ESCUCHA_INTERRUPT;
    int CANTIDAD_ENTRADAS_TLB;
    char* ALGORITMO_TLB;

    int SOCKET_MEMORIA;
    int SOCKET_KERNEL_DISPATCH;
    int SOCKET_KERNEL_INTERRUPT;
}typedef t_cpu_config;

struct memoria_config {
    char* puertoEscucha;
    int tamMemoria;
    int tamPagina;
    char* pathInstrucciones;
    int retardoRespuesta;
}typedef t_memoria_config;
/**
* @fn    decir_hola
* @brief Imprime un saludo al nombre que se pase por parámetro por consola.
*/
void decir_hola(char* quien);

int crear_conexion_servidor(char* puerto);
int esperar_cliente(int socket_servidor, t_log* logger);
int crear_conexion_cliente(char *ip, char* puerto);
void buffer_desempaquetar_registros(t_buffer* buffer, t_registros* regs);
void contexto_destruir(t_contexto* contexto);
#endif
