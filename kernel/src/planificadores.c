#include <utils/hello.h>
#include "planificadores.h"

pthread_mutex_t siguientePIDmutex;
uint32_t siguientePID = 0;

uint32_t obtener_siguiente_pid(){
    pthread_mutex_lock(&siguientePIDmutex);
    uint32_t nuevoPID = siguientePID++;
    pthread_mutex_unlock(&siguientePIDmutex);
    return nuevoPID;
}