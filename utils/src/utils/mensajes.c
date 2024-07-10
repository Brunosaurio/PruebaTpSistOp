#include "hello.h"

extern t_log* loggerMemoria;

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}


void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}

void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	//close(socket_cliente);
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0){
		printf("%d", cod_op);
		return cod_op;
	}
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	//log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
t_buffer* buffer_crear(void) {
    t_buffer* self = malloc(sizeof(*self));
    self->size = 0;
    self->stream = NULL;
    return self;
}

t_buffer* buffer_crear_copia(t_buffer* bufferACopiar) {
    t_buffer* self = malloc(sizeof(*self));
    self->size = bufferACopiar->size;
    self->stream = malloc(self->size);
    memcpy(self->stream, bufferACopiar->stream, self->size);
    return self;
}

void buffer_destruir(t_buffer* self) {
    free(self->stream);
    free(self);
}

void buffer_empaquetar(t_buffer* self, void* streamToAdd, int size) {
    self->stream = realloc(self->stream, self->size + size);
    memcpy(self->stream + self->size, streamToAdd, size);
    self->size += size;
}

void buffer_empaquetar_string(t_buffer* self, char* string) {
    int largoString = strlen(string) + 1; //Se tiene en cuenta el '\0', del otro lado va a copiarse también
    buffer_empaquetar(self, &largoString, sizeof(largoString));
    buffer_empaquetar(self, string, largoString);
}

void buffer_desempaquetar(t_buffer* self, void* dest, int size) {
    if (self->stream == NULL || self->size == 0) {
        puts("\e[0;31mbuffer_unpack: Error en el desempaquetado del buffer\e[0m");
        exit(-1);
    }
    memcpy(dest, self->stream, size);
    self->size -= size;
    memmove(self->stream, self->stream + size, self->size);
    self->stream = realloc(self->stream, self->size);
}

void buffer_desempaquetar_string(t_buffer* self, char** pString) {
    int largoString;
    buffer_desempaquetar(self, &largoString, sizeof(largoString));
    *pString = realloc(*pString, largoString);
    buffer_desempaquetar(self, *pString, largoString);
}

void buffer_empaquetar_registros(t_buffer* buffer, t_registros* regs){
    buffer_empaquetar(buffer, &regs->AX, sizeof(uint8_t));
    /*buffer_empaquetar(buffer, &regs->BX, sizeof(*regs->BX)));
    buffer_empaquetar(buffer, &regs->CX, sizeof((regs->CX)));
    buffer_empaquetar(buffer, &regs->DX, sizeof((regs->DX)));
    buffer_empaquetar(buffer, &regs->EAX, sizeof((regs->EAX)));
    buffer_empaquetar(buffer, &regs->EBX, sizeof((regs->EBX)));
    buffer_empaquetar(buffer, &regs->ECX, sizeof((regs->ECX)));
    buffer_empaquetar(buffer, &regs->EDX, sizeof((regs->EDX)));
    buffer_empaquetar(buffer, &regs->SI, sizeof((regs->SI)));
    buffer_empaquetar(buffer, &regs->DI, sizeof((regs->DI)));
    buffer_empaquetar(buffer, &regs->PC, sizeof((regs->PC)));*/
}

void buffer_desempaquetar_registros(t_buffer* buffer, t_registros* regs){
    buffer_desempaquetar(buffer, &regs->AX, sizeof(uint8_t));
    /*buffer_desempaquetar(buffer, regs->BX, sizeof((regs->BX)));
    buffer_desempaquetar(buffer, regs->CX, sizeof((regs->CX)));
    buffer_desempaquetar(buffer, regs->DX, sizeof((regs->DX)));
    buffer_desempaquetar(buffer, regs->EAX, sizeof((regs->EAX)));
    buffer_desempaquetar(buffer, regs->EBX, sizeof((regs->EBX)));
    buffer_desempaquetar(buffer, regs->ECX, sizeof((regs->ECX)));
    buffer_desempaquetar(buffer, regs->EDX, sizeof((regs->EDX)));
    buffer_desempaquetar(buffer, regs->SI, sizeof((regs->SI)));
    buffer_desempaquetar(buffer, regs->DI, sizeof((regs->DI)));
    buffer_desempaquetar(buffer, regs->PC, sizeof((regs->PC)));*/
}

void handshake_Inicial_SV(int socketCliente){
    size_t bytes;

    int32_t handshake;
    int32_t resultOk = 0;
    int32_t resultError = -1;

    bytes = recv(socketCliente, &handshake, sizeof(int32_t), MSG_WAITALL);
    if (handshake == 1) {
        bytes = send(socketCliente, &resultOk, sizeof(int32_t), 0);
    } else {
        bytes = send(socketCliente, &resultError, sizeof(int32_t), 0);
    }
}
void handshake_Inicial_CL(int socketServidor){
    size_t bytes;

    int32_t handshake = 1;
    int32_t result;

    bytes = send(socketServidor, &handshake, sizeof(int32_t), 0);
    bytes = recv(socketServidor, &result, sizeof(int32_t), MSG_WAITALL);

    if (result == 0) {
        printf("Conexion Establecida\n");
    } else {
        printf("Fallo al intentar conectar\n");
    }
}
