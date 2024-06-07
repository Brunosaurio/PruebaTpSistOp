#ifndef UTILS_H_
#define UTILS_H_
#include <commons/collections/list.h>

typedef enum
{
    NUEVA_INSTANCIA_IO,
	MENSAJE,
	PAQUETE
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


void enviar_mensaje(char* mensaje, int socket_cliente);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void liberar_conexion(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
void* recibir_buffer(int*, int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);
t_buffer* buffer_crear(void);
t_buffer* buffer_crear_copia(t_buffer*);
void buffer_destruir(t_buffer* self);
void buffer_empaquetar(t_buffer* self, void* streamToAdd, int size);
void buffer_empaquetar_string(t_buffer* self, char* string);

void buffer_desempaquetar(t_buffer* self, void* dest, int size);
void buffer_desempaquetar_string(t_buffer* self, char** pString);
void handshake_Inicial_SV(int socketCliente);
void handshake_Inicial_CL(int socketServidor);
#endif /* UTILS_H_ */