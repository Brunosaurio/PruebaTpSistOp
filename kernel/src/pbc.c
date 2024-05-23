
t_pcb* pcb_crear(uint32_t pid, uint32_t tamanio, int quantum, UINT32_t registros) {
    t_pcb* self = malloc(sizeof(*self));
    self->pid = pid;
    self->tamanio = tamanio;
    self->programCounter = 0;
    self->estado = NEW;
    self->socketConsola = NULL;
    self->instrucciones = NULL;
    self->quantum = quantum;
    self->realAnterior = 0.0;
    self->tiempoDeBloqueo = 0;
    self->tiempoEjecutado = 0.0;
    self->mutex = malloc(sizeof(*(self->mutex)));
    pthread_mutex_init(self->mutex, NULL);
    self->regitros_CPU = registros;
    return self;
}

void pcb_destruir(t_pcb* self) {
    if (self->instrucciones != NULL) {
        buffer_destruir(self->instrucciones);
    }
    if (self->socketConsola != NULL) {
        close(*self->socketConsola);
        free(self->socketConsola);
    }
    free(self);
}

void pcb_setear_socket(t_pcb* self, int* socketConsola) {
    self->socketConsola = socketConsola;
}
int pcb_obtener_socket_consola(t_pcb* self) {
    return *self->socketConsola;
}
t_buffer* pcb_obtener_buffer_de_instrucciones(t_pcb* self) {
    return self->instrucciones;
}
void pcb_setear_buffer_de_instrucciones(t_pcb* self, t_buffer* instructionsBuffer) {
    self->instrucciones = instructionsBuffer;
}
uint32_t pcb_obtener_program_counter(t_pcb* self) {
    return self->programCounter;
}
void pcb_setear_program_counter(t_pcb* self, uint32_t programCounter) {
    self->programCounter = programCounter;
}

time_t pcb_obtener_tiempoDellegadaAReady(t_pcb* self){
    return self->tiempoDellegadaAReady;
}
void pcb_setear_tiempoDellegadaAReady(t_pcb* self) {
    time(&self->tiempoDellegadaAReady);
}

uint8_t pcb_obtener_estado(t_pcb* self) {
    return self->estado;
}
void pcb_setear_estado(t_pcb* self, uint8_t estadoNuevo) {
    self->estado = estadoNuevo;
}
void pcb_setear_tiempoEjecutado(t_pcb* self, u_int32_t nuevo){
   self->tiempoEjecutado = nuevo;
}
uint32_t pcb_obtener_tiempoEjecutado(t_pcb* self){
   return self->tiempoEjecutado;
}
registros* pcb_obtener_registros(t_pcb* self){
    return &(self->registros_CPU);
}

pthread_mutex_t* pcb_obtener_mutex(t_pcb* self) {
    return self->mutex;
}

///pepe///