#include <stdlib.h>
#include <stdio.h>
#include "consola.h"
#include <commons/config.h>
#include <commons/log.h>
#include <utils/hello.h>

t_log* loggerKernel;
t_kernel_config* configKernel;

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
    decir_hola("KERNEL");
    
    //kernel se conecta al servidor memoria
    int socketKernelMemoria= crear_conexion_cliente(configKernel->IP_MEMORIA, configKernel->PUERTO_MEMORIA);
    handshake_Inicial_CL(socketKernelMemoria);
    configKernel->SOCKET_MEMORIA = socketKernelMemoria;
    
    
    /*//kernel se conecta al servidor cpu
    int socketCpuDispatch = crear_conexion_cliente(configKernel->IP_CPU, configKernel->PUERTO_CPU_DISPATCH);
    handshake_Inicial_CL(socketCpuDispatch);
    int socketCpuInterrupt = crear_conexion_cliente(configKernel->IP_CPU, configKernel->PUERTO_CPU_INTERRUPT);
    handshake_Inicial_CL(socketCpuInterrupt);


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
    iniciar_consola_interactiva();
    //pthread_ hilo_consola;
    //pthread_create(&hilo_consola.NULL, (void*)iniciar_consola_interactiva,NULL);
    //pthread_join(hilo_consola,NULL);
    
    return 0;

}



