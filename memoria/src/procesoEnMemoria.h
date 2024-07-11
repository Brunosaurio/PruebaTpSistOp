#ifndef PROCESO_EN_MEMORIA_H
#define PROCESO_EN_MEMORIA_H
#include <utils/hello.h>

typedef struct proceso_en_memoria{
	int pid;
	//char* pathInstrucciones;
	t_list* instrucciones;
}t_proceso_en_memoria;

t_list* leer_archivo_y_cargar_instrucciones(t_list* lista_de_instrucciones,const char* path_archivo);
char* rastrear_instruccion(int pid, int pc);
#endif /* PROCESO_EN_MEMORIA_H */