#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include "atender_peticiones.h"
#include "procesoEnMemoria.h"

extern t_log* loggerMemoria;
extern t_memoria_config* configMemoria;
extern t_list* interfacesConectadas;
extern t_list* procesosEnMemoria;
extern int socketKernel;

t_memoria_config* cargarMemoriaConfig(char* path, t_log* logger) {
    t_config* tempCfg = config_create(path);
    if (tempCfg == NULL){
        log_info(logger, "No se encontro ruta hacia la config");
        exit(-1);
    }
    t_memoria_config* memoriaConfig = malloc(sizeof(t_memoria_config));
    memoriaConfig->puertoEscucha = strdup(config_get_string_value(tempCfg, "PUERTO_ESCUCHA"));
    memoriaConfig->tamMemoria = config_get_int_value(tempCfg, "TAM_MEMORIA");
    memoriaConfig->tamPagina = config_get_int_value(tempCfg, "TAM_PAGINA");
    memoriaConfig->pathInstrucciones = strdup(config_get_string_value(tempCfg, "PATH_INSTRUCCIONES"));
    memoriaConfig->retardoRespuesta = config_get_int_value(tempCfg, "RETARDO_RESPUESTA");
    log_info(logger,"Carga de config completa");
    return memoriaConfig;
}

/////////////////////////Prueba hilos mas de 1 parametro


/////////////////////////Hilos
void crearHilo_EscucharNuevasIO(int socketSv){
    pthread_t escucharIO;
	pthread_create(&escucharIO, NULL, (void*) escuchar_conexiones_IO, (int*)socketSv);
	pthread_detach(escucharIO);
}


int main(int argc, char* argv[]) {
    interfacesConectadas = list_create();
    procesosEnMemoria= list_create();

    loggerMemoria = log_create("Memoria.log", "MEMORIA", 1, LOG_LEVEL_DEBUG);
    configMemoria = cargarMemoriaConfig(argv[1],loggerMemoria);
    //procesar_conexiones_memoria();
    
    decir_hola("Memoria");
    int socketMemoria = crear_conexion_servidor(configMemoria->puertoEscucha);
	if (socketMemoria < 0){
        log_info(loggerMemoria, "ERROR al iniciar el servidor de MEMORIA");
    return EXIT_FAILURE;
    }
    log_info(loggerMemoria, "%d", socketMemoria);
    
    log_info(loggerMemoria, "MEMORIA lista para recibir al CPU");
    int socketCpu = esperar_cliente(socketMemoria, loggerMemoria);
    log_info(loggerMemoria, "%d", socketCpu);
    handshake_Inicial_SV(socketCpu);
    
    
    log_info(loggerMemoria, "MEMORIA lista para recibir al KERNEL");
    socketKernel = esperar_cliente(socketMemoria, loggerMemoria);
    handshake_Inicial_SV(socketKernel);
    log_info(loggerMemoria, "%d", socketKernel);
    
    //log_info(loggerMemoria, "MEMORIA lista para recibir entredaSalida");

    //crearHilo_EscucharNuevasIO(socketMemoria);
    
    conexion_kernel_memoria();

    //procesosEnMemoria = leer_archivo_y_cargar_instrucciones(procesosEnMemoria,"pseudocodigo.txt"); No sirve
    /*--------------Prueba para comprobar que llegan los procesos
    char* leido; 
    leido = readline("> ");

    t_proceso_en_memoria* unProceso = list_get(procesosEnMemoria,0);
    log_info(loggerMemoria,"El proceso obtenido tiene pid: %d", unProceso->pid);
    log_info(loggerMemoria, "Su primera Instruccion es: %s", (char*) list_get(unProceso->instrucciones, 0));
    */
    /*------------Prueba que demuestra los dispositivos conectados
    int tamanioLista = list_size(interfacesConectadas);
    int i = 0;
    while(tamanioLista-i>0){
        interfaz_info* interfaz = list_get(interfacesConectadas,i);
        printf("%s", interfaz->nombreInterfaz);
        i++;
    }*/
 
    /*
    prueba* intento = malloc(sizeof(prueba));//{1,"algoParaMandar"};
    intento->unParametro = 1;
    intento->segundoParametro = "salimos";

    pthread_t hilo;
	pthread_create(&hilo, NULL, (void*) escuchar_conexiones_IO, intento);
	pthread_join(hilo,NULL);
    */
    return 0;
}
    
    