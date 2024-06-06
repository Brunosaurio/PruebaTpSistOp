#include <utils/hello.h>

uint32_t obtener_siguiente_pid();
void iniciar_proceso_en_kernel(char* path);
void iniciar_proceso_en_memoria(t_pcb* pcbQuePasaAReady);
void loggear_cambio_estado(const char* prev, const char* post, int pid);
void planificador_largo_plazo(void);
void planificador_corto_plazo();