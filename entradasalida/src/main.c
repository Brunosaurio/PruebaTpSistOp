#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include <ctype.h>

struct config_interfaz
{
    char* tipoInterfaz;
    int unidadTrabajo;
    char* ipKernel;
    char* puertoKernel;
    char* ipMemoria;
    char* puertoMemoria;
    char* pathBaseDialFS;
    int blockSize;
    int blockCount;
}typedef t_interfaz_config;

t_interfaz_config* cargarInterfazConfig(char* path, t_log* logger) {
    t_config* tempCfg = config_create(path);
    if (tempCfg == NULL){
        log_info(logger, "No se encontro ruta hacia la config");
        exit(-1);
    }
    t_interfaz_config* interfazConfig = malloc(sizeof(t_interfaz_config));
    interfazConfig->tipoInterfaz = strdup(config_get_string_value(tempCfg, "TIPO_INTERFAZ"));
    interfazConfig->unidadTrabajo = config_get_int_value(tempCfg, "TIEMPO_UNIDAD_TRABAJO");
    interfazConfig->ipKernel = strdup(config_get_string_value(tempCfg, "IP_KERNEL"));
    interfazConfig->puertoKernel = strdup(config_get_string_value(tempCfg, "PUERTO_KERNEL"));
    interfazConfig->ipMemoria = strdup(config_get_string_value(tempCfg, "IP_MEMORIA"));
    interfazConfig->puertoMemoria = strdup(config_get_string_value(tempCfg, "PUERTO_MEMORIA"));
    interfazConfig->pathBaseDialFS = strdup(config_get_string_value(tempCfg, "PATH_BASE_DIALFS"));
    interfazConfig->blockSize = config_get_int_value(tempCfg, "BLOCK_SIZE");
    interfazConfig->blockCount = config_get_int_value(tempCfg, "BLOCK_COUNT");
    log_info(logger,"Carga de config completa");
    free(tempCfg);
    return interfazConfig;
}
void enviarDatosAServidor(int socketServidor, char* nombreInterfaz){
    t_buffer* buffer = buffer_crear();
    buffer_empaquetar_string(buffer, nombreInterfaz);
    stream_enviar_buffer(socketServidor,1, buffer);
}
int main(int argc, char* argv[]) {
    t_log* loggerInterfaz = log_create(argv[1], argv[1] , 1, LOG_LEVEL_DEBUG);
    t_interfaz_config* configInterfaz = cargarInterfazConfig(argv[2],loggerInterfaz);
    decir_hola("una Interfaz de Entrada/Salida");
    
    //EntradaSalida se conecta con el servidor memoria
    int socketEntradaSalidaMem = crear_conexion_cliente(configInterfaz->ipMemoria, configInterfaz->puertoMemoria);
    handshake_Inicial_CL(socketEntradaSalidaMem);
    //send(socketEntradaSalidaMem,char* argv[1], strlen(argv[1])+1,0);
    enviarDatosAServidor(socketEntradaSalidaMem, argv[1]);
    //EntradaSalida se conecta con el servidor kernel
    /*int socketEntradaSalidaKer= crear_conexion_cliente(configInterfaz->ipKernel, configInterfaz->puertoKernel);
    handshake_Inicial_CL(socketEntradaSalidaKer);*/
    return 0;
}
