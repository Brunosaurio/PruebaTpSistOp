#include "consola.h"

extern sem_t semProcesosEnNew;

extern t_log* loggerKernel;
extern t_kernel_config* configKernel;

void f_iniciar_proceso(char* path){
    bool procesoCreado = false;
    int pid = obtener_siguiente_pid();

    procesoCreado = iniciar_proceso_en_kernel(pid);
    //avisar a memoria [int pid][char* path][int size]
    t_buffer* a_enviar = buffer_crear();
    //cargar_int_al_buffer(a_enviar,pid);
    buffer_empaquetar(a_enviar, &pid, sizeof(int));
    buffer_empaquetar_string(a_enviar, path);
    
    log_info(loggerKernel, "%s", path);
    stream_enviar_buffer(configKernel->SOCKET_MEMORIA, PROCESO_NUEVO, a_enviar);
    buffer_destruir(a_enviar);

    sem_post(&semProcesosEnNew);
}

void atender_instruccion_validada(char* leido){
    char** comando_consola = string_split(leido, " ");
    //pthread_t un_hilo;
    //t_buffer* un_buffer = crear_buffer();

    if (strcmp(comando_consola[0],"INICIAR_PROCESO") == 0){ //[PATH]
        f_iniciar_proceso(comando_consola[1]);
        /*cargar_string_al_buffer(un_buffer, comando_consola[1]);//[path]
        cargar_string_al_buffer(un_buffer, comando_consola[2]);//[size]
        cargar_string_al_buffer(un_buffer, comando_consola[3]);//[prioridad]
        f_iniciar_proceso(un_buffer);
        pthread_create(&un_hilo,NULL, (void*)f_iniciar_proceso, un_buffer);
        pthread_detach(un_hilo);*/

    }else if(strcmp(comando_consola[0], "FINALIZAR_PROCESO" ) == 0 ){
    
    }else if(strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0){
    
    }else if (strcmp (comando_consola[0], "INICIAR_PLANIFICACION") == 0){
    
    }else if (strcmp (comando_consola[0], "MULTIPROGRAMACION") == 0){
    
    }else if (strcmp (comando_consola[0], "PROCESO_ESTADO")==0){
    
    }else if (strcmp (comando_consola[0], "EJECUTAR_SCRIPT") == 0){
    
    }else{
        log_error(loggerKernel, "Comando no reconocido, pero paso el filtro");
        exit(EXIT_FAILURE);
        
    }

    string_array_destroy(comando_consola);
}

bool validacion_de_instruccion_de_consola(char* leido){
    bool resultado_validacion = false;
   // FALTA Hacer hacer mas controles de validacion con la cantidad de parametros
    char** comando_consola = string_split(leido, " ");

    if(strcmp(comando_consola[0], "INICIAR_PROCESO") == 0) {
        resultado_validacion = true;
    }else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0){
        resultado_validacion = true;
    }else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0){
        resultado_validacion = true; 
    }else if (strcmp (comando_consola[0], "INICIAR_PLANIFICACION") == 0){
        resultado_validacion = true; 
    }else if (strcmp (comando_consola[0], "MULTIPROGRAMACION") == 0){
        resultado_validacion = true; 
    }else if (strcmp (comando_consola[0], "PROCESO_ESTADO")==0){
        resultado_validacion = true; 
    }else if (strcmp (comando_consola[0], "EJECUTAR_SCRIPT") == 0){
        resultado_validacion = true; 
    }else{
        resultado_validacion = false; 
}

string_array_destroy(comando_consola);

return resultado_validacion;
}

void iniciar_consola_interactiva(){
    char* leido; 
    leido = readline("> ");
    bool validacion_leido; 

    while(strcmp(leido, "\0") != 0){
        validacion_leido = validacion_de_instruccion_de_consola(leido);
        if(!validacion_leido){
        log_error(loggerKernel, "Comando de CONSOLA no reconocido");
        free(leido);
        leido = readline("> "); 
        continue; //Saltar y continua con el resto de la iteracion 
    } 
    atender_instruccion_validada(leido); 
    free(leido); 
    leido = readline ("> "); 
    } 
    free(leido); 
}