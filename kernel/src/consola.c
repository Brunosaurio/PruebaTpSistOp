#include "../src/utils/consola.h"


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

_atender_instruccion_validada(leido); 
free(leido); 
leido = readline ("> "); 

} 
free(leido); 


bool validacion_de_instruccion_de_consola(char* leido){
 bool resultado_validacion = false;
   // FALTA Hacer hacer mas controles de validacion 
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
 }else if (strcmp (comando_consola[0], "HELP") == 0){
    resultado_validacion = true; 
}else if (strcmp (comando_consola[0], "PRINT") == 0){
 resultado_validacion = true; 
}else{
    log_error(loggerKernel, "Comando no reconocido."); 
    resultado_validacion = false; 
}

string_array_destroy(comando_consola);

return resultado_validacion;
}

void atender_instruccion_validada(char* leido){
    char** comando_consola = string_split(leido, "");
    pthread_t un_hilo;
    t_buffer* un_buffer = crear_buffer();

    if (strcmp(comando_consola[0],"INICIAR_PROCESO") == 0){ //[PATH][SIZE][PRIORIDAD]
        cargar_string_al_buffer(un_buffer, comando_consola[1]);//[path]
        cargar_string_al_buffer(un_buffer, comando_consola[2]);//[size]
        cargar_string_al_buffer(un_buffer, comando_consola[3]);//[prioridad]
        f_iniciar_proceso(un_buffer);
        pthread_create(&un_hilo,NULL, (void*)f_iniciar_proceso, un_buffer);
        pthread_detach(un_hilo);

    }else if(strcmp(comando_consola[0], "FINALIZAR_PROCESO" ) == 0 ){
    
    }else if(strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0){
    
    }else if (strcmp (comando_consola[0], "INICIAR_PLANIFICACION") == 0){
    
    }else if (strcmp (comando_consola[0], "MULTIPROGRAMACION") == 0){
    
    }else if (strcmp (comando_consola[0], "PROCESO_ESTADO")==0){
    
    }else if (strcmp (comando_consola[0], "HELP") == 0){
    
    }else if (strcmp (comando_consola[0], "PRINT") == 0){
    
    }else{
        log_error(loggerKernel, "Comando no reconocido, pero paso el filtro");
        exit(EXIT_FAILURE);
        
    }

    string_array_destroy(comando_consola);
}

void f_iniciar_proceso(t_buffer* un_buffer){
    char* path = extraer_string_del_buffer(un_buffer);
    char* size = extraer_string_del_buffer(un_buffer);
    char* prioridad =extraer_string_del_buffer(un_buffer);
    log_trace(kernel_log_debug, "BUFFER(%d): [PATH:%s][SIZE:%s][PRIORIDAD:%s]", un_buffer->size,path,prioridad);
    destruir_buffer(un_buffer);

    int pid = asignar_pid();
    int size_num = atoi(size);

    //avisar a memoria [int pid][char* path][int size]
    t_buffer* a_enviar = crear_buffer();
    cargar_int_al_buffer(a_enviar,pid);
    cargar_string_al_buffer(a_enviar, path);
    cargar_int_al_buffer(a_enviar, size_num);
    t_paquete* un_paquete = crear_super_paquete(CREAR_PROCESO_KM, a_enviar);
    enviar_paquete(un_paquete, fd_memoria);
    destruir_paquete(un_paquete);
    

    //falta hacer el resto de la logica para el funcionamiento del kernel






            
            


            