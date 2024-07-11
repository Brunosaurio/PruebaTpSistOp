#include "procesoEnMemoria.h"

extern t_log* loggerMemoria;
extern t_list* procesosEnMemoria;

t_list* leer_archivo_y_cargar_instrucciones(t_list* lista_de_instrucciones,const char* path_archivo) {
    FILE *pseudocodiogo = fopen(path_archivo, "r");
    
    if (pseudocodiogo != NULL)
    {
        char *linea = NULL;
        size_t longitud = 0;
        ssize_t bytes_leidos;
        while ((bytes_leidos = getline(&linea, &longitud, pseudocodiogo)) != -1) {
            log_info(loggerMemoria,"%s", linea);//Comentar esta linea
            list_add(lista_de_instrucciones, strdup(linea));
        }
        return lista_de_instrucciones;
    }
    else
    {
        log_error(loggerMemoria, "No se pudo abrir el archivo");
        exit(1);
    }
    
    fclose(pseudocodiogo);
};

bool tieneMismoPid(void* proceso, int pid){
    return ((t_proceso_en_memoria*)proceso)->pid == pid;
}

char*  rastrear_instruccion(int pid, int pc){
    bool tienepid(void *proceso){
        return tieneMismoPid(proceso, pid);
    }
    int cantProcEnMemo;

    t_proceso_en_memoria* procesoEncontrado = malloc(sizeof(t_proceso_en_memoria));
    procesoEncontrado = list_find(procesosEnMemoria, tienepid);
    log_info(loggerMemoria, "Proceso pedido para pasar instruccion %d", procesoEncontrado->pid);
    //log_info(loggerMemoria, "Cantidad de instrucciones del proceso %d ", list_size(procesoEncontrado->instrucciones));
    //log_info(loggerMemoria, "Instruccion %s", (char*) list_get(procesoEncontrado->instrucciones, pc));
    return (char*) list_get(procesoEncontrado->instrucciones, pc);
}