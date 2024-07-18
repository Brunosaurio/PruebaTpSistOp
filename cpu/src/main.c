#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include <commons/config.h>
#include <commons/log.h>
#include "atender_kernel.h"

t_log* cpuLogger;
t_cpu_config* cpuConfig;
t_registros* registrosCPU;

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
    
    cpuConfig->SOCKET_MEMORIA=-1;
    cpuConfig->SOCKET_KERNEL_DISPATCH=-1;
    cpuConfig->SOCKET_KERNEL_INTERRUPT=-1;


log_info(logger,"Carga de config completa");
 return cpuConfig;
}
/////////////////////////////////////////////////////////////Mover a donde corresponda
t_registros* iniciarRegistrosCpu(t_registros* registros){
    registros->AX = 0;
    registros->BX = 0;
    registros->CX = 0;
    registros->DX = 0;
    registros->EAX= 0;
    registros->EBX= 0;
    registros->ECX= 0;
    registros->EDX= 0;
    registros->DI = 0;
    registros->PC = 0;
    registros->SI = 0;
    return registros;
}
//Esto se debe hacer cada vez que se reciba 

int main(int argc, char* argv[]) {
    cpuLogger = log_create("Cpu.log", "CPU", 1, LOG_LEVEL_DEBUG);
    cpuConfig = cargarCpuConfig(argv[1],cpuLogger);
    registrosCPU = malloc(sizeof(t_registros));
    registrosCPU = iniciarRegistrosCpu(registrosCPU);
    decir_hola("CPU");
    //cpu se conecta al servidor memoria
    int socketMemoria = crear_conexion_cliente(cpuConfig->IP_MEMORIA, cpuConfig->PUERTO_MEMORIA);
    handshake_Inicial_CL(socketMemoria);
    cpuConfig->SOCKET_MEMORIA = socketMemoria;

    //Arranca servidor cpu al que se nos va a conectar kernel 
    log_info(cpuLogger, "Iniciando Servidor CPU...");
    
    int socketCpuDispatch = crear_conexion_servidor(cpuConfig->PUERTO_ESCUCHA_DISPATCH);
    int socketCpuInterrupt = crear_conexion_servidor(cpuConfig->PUERTO_ESCUCHA_INTERRUPT);
    
    if (socketCpuDispatch < 0 || socketCpuInterrupt < 0){
        log_error(cpuLogger, "Error intentando iniciar Servidor CPU.");
        log_destroy(cpuLogger);    
        exit(-1); 
    }
    else log_info(cpuLogger, "CPU listo para recibir al KERNEL en conexiones Dispatch e Interrupt");
    
    //Hacemos handshake
    cpuConfig->SOCKET_KERNEL_DISPATCH = esperar_cliente(socketCpuDispatch, cpuLogger);
    handshake_Inicial_SV(cpuConfig->SOCKET_KERNEL_DISPATCH);
    cpuConfig->SOCKET_KERNEL_INTERRUPT = esperar_cliente(socketCpuInterrupt, cpuLogger);
    handshake_Inicial_SV(cpuConfig->SOCKET_KERNEL_INTERRUPT);
    log_info(cpuLogger, "Handshake completado");
    
    //Esperamos las peticiones del Kernel

    atender_peticiones_kernel();



    return 0;
}    
    

