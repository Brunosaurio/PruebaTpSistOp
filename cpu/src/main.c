#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include <commons/config.h>
#include <commons/log.h>
#include "atender_kernel.h"

struct cpu_config {
    
    char* IP_MEMORIA;
    char* PUERTO_MEMORIA;
    char* PUERTO_ESCUCHA_DISPATCH;
    char* PUERTO_ESCUCHA_INTERRUPT;
    int CANTIDAD_ENTRADAS_TLB;
    char* ALGORITMO_TLB;

    int SOCKET_MEMORIA;
    int SOCKET_KERNEL;
}typedef t_cpu_config;

t_log* cpuLogger;
t_cpu_config* cpuConfig;

t_cpu_config* cargarCpuConfig(char* path, t_log* logger) {
    t_config* tempCfg = config_create(path);
    if (tempCfg == NULL){
        log_info(logger, "No se encontro ruta hacia la config");
        exit(-1);
    }

    t_cpu_config* cpuConfig = malloc(sizeof(t_cpu_config));

    cpuConfig->IP_MEMORIA = config_get_string_value(tempCfg, "IP_MEMORIA");
    cpuConfig->PUERTO_MEMORIA = config_get_string_value(tempCfg, "PUERTO_MEMORIA");
    cpuConfig->PUERTO_ESCUCHA_DISPATCH = config_get_string_value(tempCfg, "PUERTO_ESCUCHA_DISPATCH");
    cpuConfig->PUERTO_ESCUCHA_INTERRUPT = config_get_string_value(tempCfg, "PUERTO_ESCUCHA_INTERRUPT");
    cpuConfig->CANTIDAD_ENTRADAS_TLB = config_get_int_value(tempCfg, "CANTIDAD_ENTRADAS_TLB");
    cpuConfig->ALGORITMO_TLB = config_get_string_value(tempCfg, "ALGORITMO_TLB");
   

log_info(logger,"Carga de config completa");
 return cpuConfig;
}
 void pepito()
 {
    log_info(cpuLogger, "Estoy corriendo");
 }

int main(int argc, char* argv[]) {
    t_log* loggerCpu = log_create("Cpu.log", "CPU", 1, LOG_LEVEL_DEBUG);
    t_cpu_config* configCpu = cargarCpuConfig(argv[1],loggerCpu);
    decir_hola("CPU");
    //cpu se conecta al servidor memoria 
    int socketMemoria = crear_conexion_cliente(configCpu->IP_MEMORIA, configCpu->PUERTO_MEMORIA);
    handshake_Inicial_CL(socketMemoria);
     
    //arranca servidor cpu al que se nos va a conectar kernel 
    log_info(loggerCpu, "Iniciando Servidor CPU...");
    int socketCpuDispatch = crear_conexion_servidor(configCpu->PUERTO_ESCUCHA_DISPATCH);
    
    int socketCpuInterrupt = crear_conexion_servidor(configCpu->PUERTO_ESCUCHA_INTERRUPT);
    
    if (socketCpuDispatch < 0 || socketCpuInterrupt < 0){
        log_error(loggerCpu, "Error intentando iniciar Servidor CPU.");
        log_destroy(loggerCpu);    
        exit(-1); 
    }
    else log_info(loggerCpu, "CPU listo para recibir al KERNEL en conexiones Dispatch e Interrupt");
    int socketKernelDispatch = esperar_cliente(socketCpuDispatch, loggerCpu);
    handshake_Inicial_SV(socketKernelDispatch);
    int socketKernelInterrupt = esperar_cliente(socketCpuInterrupt, loggerCpu);
    handshake_Inicial_SV(socketKernelInterrupt);
    

    return 0;


}    
    

