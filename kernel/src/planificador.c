/*                  ALGORITMOS DE PLANIFICACION                              */
/*                            FIFO                                           */

t_pcb* iniciar_fifo(t_estado* estado){
    pthread_mutex_lock(estado_obtener_mutex(estado));
    t_pcb* pcb = estado_desencolar_primer_pcb(estado);
    pthread_mutex_unlock(estado_obtener_mutex(estado));
    return pcb;
}
/*    ROUND ROBIN 

*/
void planificador_corto_plazo() {
    pthread_t atenderPCBHilo;

    pthread_create(&atenderPCBHilo, NULL, (void*)atender_pcb, NULL);
    pthread_detach(atenderPCBHilo);

    for (;;) {
        t_pcb* pcbToDispatch;
            
        sem_wait(&dispatchPermitido); // fijarse si la cpu esta libre
        log_info(kernelLogger, "Se permite dispatch");

        if(hayQueReplanificar){
            sem_wait(estado_obtener_sem(estadoReady));

            log_info(kernelLogger, "Se toma una instancia de READY");

            if(algoritmoConfigurado == ALGORITMO_FIFO){
        
                pcbToDispatch = iniciar_fifo(estadoReady);

            }else{
            
               //inicia rr y vrr

            }
        }
        else{
            log_info(kernelLogger, "Se vuelve a ejecutar el ultimo proceso");
            pcbToDispatch = ultimoPcbEjecutado;
        }

        estado_encolar_pcb_con_semaforo(estadoExec, pcbToDispatch);
        sem_post(estado_obtener_sem(estadoExec));
    }
}