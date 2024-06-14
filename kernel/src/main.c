#include <stdlib.h>
#include <stdio.h>
#include "consola.h"
#include <commons/config.h>
#include <commons/log.h>
#include <utils/hello.h>

extern pthread_mutex_t mutex_New;
extern pthread_mutex_t mutex_Ready;
extern pthread_mutex_t mutex_Ex;

extern sem_t semProcesoEnNew;
extern sem_t semProcesoEnReady;
extern sem_t semPcbEnExec;
extern sem_t semGradoMultiprogramacion;
extern sem_t dispatchPermitido;


t_log* loggerKernel;
t_kernel_config* configKernel;

t_list* pcbEnExec;
t_list* procesosEnNew;
t_list* procesosEnReady;
t_list* procesosEnBloq;
t_list* procesosEnExit;

void iniciar_planificadores(){
    pthread_t planificador_LP;
    pthread_create(&planificador_LP,NULL, (void*)planificador_largo_plazo,NULL);
    pthread_detach(planificador_LP);


}

void iniciar_semaforos(){
    sem_init(&semProcesoEnNew,0,0);
    sem_init(&semProcesoEnReady,0,0);
    sem_init(&semPcbEnExec,0,0);
    sem_init(&dispatchPermitido,0,1);
    sem_init(&semGradoMultiprogramacion,0,configKernel->GRADO_MULTIPROGRAMACION);



    pthread_mutex_init(&mutex_New,NULL);
    pthread_mutex_init(&mutex_Ready,NULL);
    pthread_mutex_init(&mutex_Ex,NULL);

    
}

void iniciar_listas(){
    pcbEnExec = list_create();
    procesosEnNew = list_create();
    procesosEnBloq = list_create();
    procesosEnReady = list_create();
    procesosEnExit = list_create();
}

void iniciar_estructuras(){
    iniciar_listas();
    iniciar_semaforos();
    
}



t_kernel_config* cargarKernelConfig(char* path,t_log* logger){
    t_config* tempCfg = config_create(path);
  
    if (tempCfg==NULL)
    {
      log_info(logger,"no se encontro ruta hacia la config ");
      exit(-1);
  }
  t_kernel_config* kernelConfig = malloc(sizeof(t_kernel_config));

  kernelConfig->IP_CPU = config_get_string_value(tempCfg, "IP_CPU");
  kernelConfig->IP_MEMORIA = config_get_string_value(tempCfg, "IP_MEMORIA");
  kernelConfig->PUERTO_MEMORIA = config_get_string_value(tempCfg, "PUERTO_MEMORIA");
  kernelConfig->PUERTO_CPU_DISPATCH = config_get_string_value(tempCfg, "PUERTO_CPU_DISPATCH");
  kernelConfig->PUERTO_CPU_INTERRUPT = config_get_string_value(tempCfg, "PUERTO_CPU_INTERRUPT");
  kernelConfig->ALGORITMO_PLANIFICACION = config_get_string_value(tempCfg, "ALGORITMO_PLANIFICACION");
  kernelConfig->GRADO_MULTIPROGRAMACION = config_get_int_value(tempCfg, "GRADO_MULTIPROGRAMACION");
  kernelConfig->QUANTUM = config_get_int_value(tempCfg, "QUANTUM");
  kernelConfig->PUERTO_ESCUCHA =  config_get_string_value(tempCfg, "PUERTO_ESCUCHA");

  kernelConfig->SOCKET_CPU = -1;
  kernelConfig->SOCKET_MEMORIA = -1;


  log_info(logger,"carga de config completa");
  return kernelConfig;
}



int main(int argc, char* argv[]) {
    loggerKernel = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);
    configKernel = cargarKernelConfig(argv[1],loggerKernel);
    iniciar_estructuras();
    decir_hola("KERNEL");
    
    //kernel se conecta al servidor memoria
    int socketKernelMemoria= crear_conexion_cliente(configKernel->IP_MEMORIA, configKernel->PUERTO_MEMORIA);
    handshake_Inicial_CL(socketKernelMemoria);
    configKernel->SOCKET_MEMORIA = socketKernelMemoria;
    
    
    //kernel se conecta al servidor cpu
    int socketCpuDispatch = crear_conexion_cliente(configKernel->IP_CPU, configKernel->PUERTO_CPU_DISPATCH);
    handshake_Inicial_CL(socketCpuDispatch);
    int socketCpuInterrupt = crear_conexion_cliente(configKernel->IP_CPU, configKernel->PUERTO_CPU_INTERRUPT);
    handshake_Inicial_CL(socketCpuInterrupt);

/*
    //arranca servidor KERNEL al que se nos va a conectar EntradaSalida
    int socketKernelSV = crear_conexion_servidor(configKernel->PUERTO_ESCUCHA);
	  if (socketKernelSV < 0){
        log_info(loggerKernel, "ERROR al iniciar el servidor de Kernel");
    return EXIT_FAILURE;
    }
    
    
    log_info(loggerKernel, "Kernel lista para recibir al dispositivo de entradaSalida");
    int socketEntradaSalida = esperar_cliente(socketKernelSV, loggerKernel);
    handshake_Inicial_SV(socketEntradaSalida);
    */

    //iniciar consola interactiva 
    iniciar_planificadores();
    iniciar_consola_interactiva();
    //pthread_ hilo_consola;
    //pthread_create(&hilo_consola.NULL, (void*)iniciar_consola_interactiva,NULL);
    //pthread_join(hilo_consola,NULL);
    
    return 0;

}



