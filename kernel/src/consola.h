#include <utils/hello.h>
#include <readline/readline.h>
#include "planificadores.h"

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

void f_iniciar_proceso(char* path);
void atender_instruccion_validada(char* leido);
bool validacion_de_instruccion_de_consola(char* leido);
void iniciar_consola_interactiva();