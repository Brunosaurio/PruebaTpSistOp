#include "ejecutar_intrucciones.h"
#include <utils/hello.h>

extern t_log* cpuLogger;
extern t_registros* registrosCPU;

void ejecutar_SET(t_contexto* contexto, char* reg, char* param) {
    
    log_info(cpuLogger, "PID: %d - Ejecutando: SET ", contexto->pid);

    if(strcmp(reg,"AX") == 0){
        contexto->registrosDeCPU->AX = atoi(param);
    }/*else if (strcmp(reg,"BX") == 0){
        copiarStringAVector(param, registrosDeCpu.BX, 1);
    }else if (strcmp(reg,"CX") == 0){
        copiarStringAVector(param, registrosDeCpu.CX, 1);
    }else if (strcmp(reg,"DX") == 0){
        copiarStringAVector(param, registrosDeCpu.DX, 1);
    }else if (strcmp(reg,"EAX") == 0){
        copiarStringAVector(param, registrosDeCpu.EAX, 4);
    }else if (strcmp(reg,"EBX") == 0){
        copiarStringAVector(param, registrosDeCpu.EBX, 4);
    }else if (strcmp(reg,"ECX") == 0){
        copiarStringAVector(param, registrosDeCpu.ECX, 4);
    }else if (strcmp(reg,"EDX") == 0){
        copiarStringAVector(param, registrosDeCpu.EDX, 4);
    }else if (strcmp(reg,"PC") == 0){
        copiarStringAVector(param, registrosDeCpu.PC, 4);
    }else if (strcmp(reg,"SI") == 0){
        copiarStringAVector(param, registrosDeCpu.SI, 4);
    }else if (strcmp(reg,"DI") == 0){
        copiarStringAVector(param, registrosDeCpu.DI, 4);
    }*/else{
        log_info(cpuLogger, "error al ejecutar SET");
    }

}

/*
void ejecutar_MOV_IN(t_contexto* contexto,uint32_t programCounterActualizado, char* reg, char* dirLogica){
    uint32_t pid = contexto_obtener_pid(contexto);
    log_info(cpuLogger, "PID: %d - Ejecutando: MOV_IN ", pid);
    int direccionLogica = atoi(dirLogica);
    uint32_t nroSegmento = obtener_numero_de_segmento(direccionLogica);
    uint32_t offset = obtener_offset_de_segmento(direccionLogica);
    t_buffer* buffer = buffer_crear(); 
    if(offset < contexto_obtener_tabla_de_segmentos(contexto)[nroSegmento].tamanio){
        buffer_empaquetar(buffer, &pid, sizeof(pid));
        buffer_empaquetar(buffer,&nroSegmento, sizeof(nroSegmento));
        buffer_empaquetar(buffer,&offset, sizeof(offset));
        char* valorDeMemoria = malloc(sizeof(*valorDeMemoria));
        if (strcmp(reg, "AX") == 0){   
            pedirleAMemoria(buffer, 4, valorDeMemoria, offset, contexto, nroSegmento, programCounterActualizado);
            copiarStringAVector(valorDeMemoria, registrosDeCpu.AX, 4);
        }else if (strcmp(reg, "BX") == 0){   
            pedirleAMemoria(buffer, 4, valorDeMemoria, offset, contexto, nroSegmento, programCounterActualizado);
            copiarStringAVector(valorDeMemoria, registrosDeCpu.BX, 4);
        }else if (strcmp(reg, "CX") == 0){
            pedirleAMemoria(buffer, 4, valorDeMemoria, offset, contexto, nroSegmento, programCounterActualizado);
            copiarStringAVector(valorDeMemoria, registrosDeCpu.CX, 4);
        }else if (strcmp(reg, "DX") == 0){
            pedirleAMemoria(buffer, 4, valorDeMemoria, offset, contexto, nroSegmento, programCounterActualizado);    
            copiarStringAVector(valorDeMemoria, registrosDeCpu.DX, 4);
        }else if (strcmp(reg, "EAX") == 0){
            pedirleAMemoria(buffer, 8, valorDeMemoria, offset, contexto, nroSegmento, programCounterActualizado);
            copiarStringAVector(valorDeMemoria, registrosDeCpu.EAX, 8);
        }else if (strcmp(reg, "EBX") == 0){
            pedirleAMemoria(buffer, 8, valorDeMemoria, offset, contexto, nroSegmento, programCounterActualizado);
            copiarStringAVector(valorDeMemoria, registrosDeCpu.EBX, 8);
        }else if (strcmp(reg, "ECX") == 0){
            pedirleAMemoria(buffer, 8, valorDeMemoria, offset, contexto, nroSegmento, programCounterActualizado);
            copiarStringAVector(valorDeMemoria, registrosDeCpu.ECX, 8);
        }else if (strcmp(reg, "EDX") == 0){
            pedirleAMemoria(buffer, 8, valorDeMemoria, offset, contexto, nroSegmento, programCounterActualizado);
            copiarStringAVector(valorDeMemoria, registrosDeCpu.EDX, 8);
        }else if (strcmp(reg, "RAX") == 0){
            pedirleAMemoria(buffer, 16, valorDeMemoria, offset, contexto, nroSegmento, programCounterActualizado);
            copiarStringAVector(valorDeMemoria, registrosDeCpu.RAX, 16);
        }else if (strcmp(reg, "RBX") == 0){
            pedirleAMemoria(buffer, 16, valorDeMemoria, offset, contexto, nroSegmento, programCounterActualizado);
            copiarStringAVector(valorDeMemoria, registrosDeCpu.RBX, 16);
        }else if (strcmp(reg, "RCX") == 0){
            pedirleAMemoria(buffer, 16, valorDeMemoria, offset, contexto, nroSegmento, programCounterActualizado);
            copiarStringAVector(valorDeMemoria, registrosDeCpu.RCX, 16);
        }else if (strcmp(reg, "RDX") == 0){
            pedirleAMemoria(buffer, 16, valorDeMemoria, offset, contexto, nroSegmento, programCounterActualizado);
            copiarStringAVector(valorDeMemoria, registrosDeCpu.RDX, 16);
        } else {
            log_error(cpuLogger, "no se reconocio el registro para ejecutar MOV_IN");
            exit(-1);
        }
        free(valorDeMemoria);
    }else{
        empaquetar_contexto_para_kernel(buffer,programCounterActualizado,contexto);
        stream_enviar_buffer(cpu_config_obtener_socket_kernel(cpuConfig), HEADER_proceso_terminado_seg_fault,buffer);
        log_info(cpuLogger, "PID: %d- Error SEG_FAULT- Segmento: %d- Offset: %d- Tamaño: <TAMAÑO>", contexto_obtener_pid(contexto), nroSegmento, offset);
    
    }
    buffer_destruir(buffer);
}

void ejecutar_MOV_OUT(t_contexto* contexto,uint32_t programCounterActualizado, char* reg, char* dirLogica){
    uint32_t pid = contexto_obtener_pid(contexto);
    log_info(cpuLogger, "PID: %d - Ejecutando: MOV_OUT ", pid);
    int direccionLogica = atoi(dirLogica);
    uint32_t nroSegmento = obtener_numero_de_segmento(direccionLogica);
    uint32_t offset = obtener_offset_de_segmento(direccionLogica);
    t_buffer* buffer = buffer_crear(); 
    if(offset < contexto_obtener_tabla_de_segmentos(contexto)[nroSegmento].tamanio){
        buffer_empaquetar(buffer,&pid, sizeof(pid));
        buffer_empaquetar(buffer,&nroSegmento, sizeof(nroSegmento));
        buffer_empaquetar(buffer,&offset, sizeof(offset));
        char* valorDeMemoria = malloc(sizeof(*valorDeMemoria));
        if (strcmp(reg, "AX") == 0){   
            escribirEnMemoria(buffer, 4, registrosDeCpu.AX, offset, contexto, nroSegmento, programCounterActualizado);
        }else if (strcmp(reg, "BX") == 0){
            escribirEnMemoria(buffer, 4, registrosDeCpu.BX, offset, contexto, nroSegmento, programCounterActualizado);
        }else if (strcmp(reg, "CX") == 0){
            escribirEnMemoria(buffer, 4, registrosDeCpu.CX, offset, contexto, nroSegmento, programCounterActualizado);
        }else if (strcmp(reg, "DX") == 0){
            escribirEnMemoria(buffer, 4, registrosDeCpu.DX, offset, contexto, nroSegmento, programCounterActualizado);
        }else if (strcmp(reg, "EAX") == 0){
            escribirEnMemoria(buffer, 8, registrosDeCpu.EAX, offset, contexto, nroSegmento, programCounterActualizado);
        }else if (strcmp(reg, "EBX") == 0){
            escribirEnMemoria(buffer, 8, registrosDeCpu.EBX, offset, contexto, nroSegmento, programCounterActualizado);
        }else if (strcmp(reg, "ECX") == 0){
            escribirEnMemoria(buffer, 8, registrosDeCpu.ECX, offset, contexto, nroSegmento, programCounterActualizado);
        }else if (strcmp(reg, "EDX") == 0){
            escribirEnMemoria(buffer, 8, registrosDeCpu.EDX, offset, contexto, nroSegmento, programCounterActualizado);
        }else if (strcmp(reg, "RAX") == 0){
            escribirEnMemoria(buffer, 16, registrosDeCpu.RAX, offset, contexto, nroSegmento, programCounterActualizado);
        }else if (strcmp(reg, "RBX") == 0){
            escribirEnMemoria(buffer, 16, registrosDeCpu.RBX, offset, contexto, nroSegmento, programCounterActualizado);
        }else if (strcmp(reg, "RCX") == 0){
            escribirEnMemoria(buffer, 16, registrosDeCpu.RCX, offset, contexto, nroSegmento, programCounterActualizado);
        }else if (strcmp(reg, "RDX") == 0){
            escribirEnMemoria(buffer, 16, registrosDeCpu.RDX, offset, contexto, nroSegmento, programCounterActualizado);
        } else {
            log_error(cpuLogger, "no se reconocio el registro para ejecutar MOV_OUT");
            exit(-1);
        }
        free(valorDeMemoria);
    }else{
        empaquetar_contexto_para_kernel(buffer,programCounterActualizado,contexto);
        stream_enviar_buffer(cpu_config_obtener_socket_kernel(cpuConfig), HEADER_proceso_terminado_seg_fault,buffer);
        log_info(cpuLogger, "PID: %d- Error SEG_FAULT- Segmento: %d- Offset: %d- Tamaño: <TAMAÑO>", contexto_obtener_pid(contexto), nroSegmento, offset);
    }
    buffer_destruir(buffer);
}

void ejecutar_WAIT(t_contexto* contexto,uint32_t programCounterActualizado, char* recurso){
    log_info(cpuLogger, "PID: %d - Ejecutando: WAIT", contexto_obtener_pid(contexto));
    t_buffer *bufferWAIT = buffer_crear();
    empaquetar_contexto_para_kernel(bufferWAIT,programCounterActualizado,contexto);
    stream_enviar_buffer(cpu_config_obtener_socket_kernel(cpuConfig), HEADER_proceso_wait, bufferWAIT);
    buffer_destruir(bufferWAIT);

    t_buffer *bufferParametros = buffer_crear();
    buffer_empaquetar_string(bufferParametros, recurso);
    stream_enviar_buffer(cpu_config_obtener_socket_kernel(cpuConfig), HEADER_proceso_parametros, bufferParametros);
    buffer_destruir(bufferParametros);
}



void ejecutar_SIGNAL(t_contexto* contexto,uint32_t programCounterActualizado, char* recurso){
    log_info(cpuLogger, "PID: %d - Ejecutando: SIGNAL", contexto_obtener_pid(contexto));
    t_buffer *buffer = buffer_crear();
    empaquetar_contexto_para_kernel(buffer,programCounterActualizado,contexto);
    stream_enviar_buffer(cpu_config_obtener_socket_kernel(cpuConfig), HEADER_proceso_signal, buffer);
    buffer_destruir(buffer);

    t_buffer *bufferParametros = buffer_crear();
    buffer_empaquetar_string(bufferParametros, recurso);
    stream_enviar_buffer(cpu_config_obtener_socket_kernel(cpuConfig), HEADER_proceso_parametros, bufferParametros);
    buffer_destruir(bufferParametros);
}
void ejecutar_EXIT(t_contexto* contexto,uint32_t programCounterActualizado){
    log_info(cpuLogger, "PID: %d - Ejecutando: EXIT", contexto_obtener_pid(contexto));
    t_buffer *bufferExit = buffer_crear();
    empaquetar_contexto_para_kernel(bufferExit,programCounterActualizado,contexto);
    stream_enviar_buffer(cpu_config_obtener_socket_kernel(cpuConfig), HEADER_proceso_terminado, bufferExit);
    buffer_destruir(bufferExit);
}


bool cpu_ejecutar_instrucciones(t_contexto* contexto, t_tipo_instruccion tipoInstruccion, char* parametro1, char* parametro2, char* parametro3) {
    contexto_setear_program_counter(contexto, contexto_obtener_program_counter(contexto) + 1);
    uint32_t programCounterActualizado = contexto_obtener_program_counter(contexto);
    pararDeEjecutar = false;

    switch (tipoInstruccion)
    {
    case INSTRUCCION_set:
        ejecutar_SET(contexto, parametro1, parametro2);
        pararDeEjecutar = false;
        break;
    default:
        break;
    }

    return pararDeEjecutar;
}*/


