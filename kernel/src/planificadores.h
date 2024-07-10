#include <utils/hello.h>

uint32_t obtener_siguiente_pid();
void iniciar_proceso_en_kernel(char* path);
void iniciar_proceso_en_memoria(t_pcb* pcbQuePasaAReady);
void loggear_cambio_estado(const char* prev, const char* post, int pid);
void planificador_largo_plazo(void);
void planificador_corto_plazo();
void atender_pcb();
void kernel_enviar_pcb_a_cpu(t_pcb* pcbAEnviar, t_kernel_config* kernelConfig, t_log* kernelLogger, uint8_t header);
t_pcb* kernel_recibir_pcb_actualizado_de_cpu(t_pcb* pcbAActualizar, uint8_t cpuResponse, t_kernel_config* kernelConfig, t_log* kernelLogger);