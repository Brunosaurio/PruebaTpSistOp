#include "procesoEnMemoria.h"

extern t_log* loggerMemoria;

t_list* leer_archivo_y_cargar_instrucciones(t_list* lista_de_instrucciones,const char* path_archivo) {
    FILE *pseudocodiogo = fopen(path_archivo, "r");
    
    if (pseudocodiogo != NULL)
    {
        char *linea = NULL;
        size_t longitud = 0;
        ssize_t bytes_leidos;
        while ((bytes_leidos = getline(&linea, &longitud, pseudocodiogo)) != -1) {
            log_info(loggerMemoria,"%s", linea);//Comentar esta linea
            list_add(lista_de_instrucciones, linea);
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