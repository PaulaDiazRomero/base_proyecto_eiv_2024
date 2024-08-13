#ifndef COLA_EVENTOS_H
#define COLA_EVENTOS_H
#include "evento.h"
#include <stdbool.h>

#ifndef MAX_EVENTOS
#define MAX_EVENTOS 16
#else
_Static_assert(((MAX_EVENTOS - 1) & MAX_EVENTOS) == 0, "MAX_EVENTOS debe ser potencia de dos")
#endif

typedef struct ColaEventos ColaEventos;

struct ColaEventos{
    const Evento *eventos[MAX_EVENTOS];
    unsigned escritura;
    unsigned lectura;
};

/**
 * @brief Inicializa una cola de eventos
 * 
 * @param self La cola
 */
void ColaEventos_init(ColaEventos *self);
/**
 * @brief Pone un evento en cola
 * 
 * @param self La cola
 * @param evento El evento
 * @retval true Evento puesto en cola
 * @retval false Operación fallida por cola llena
 */
bool ColaEventos_pon(ColaEventos *self,const Evento *evento);
/**
 * @brief Toma un evento de la cola
 * 
 * @param self La cola
 * @param evento Referencia a variable donde guardar el evento
 * @return true Evento obtenido
 * @return false Operación fallida por cola vacía
 */
bool ColaEventos_toma(ColaEventos *self,const Evento **evento);

/**
 * @brief Borra los eventos de la cola
 * 
 * @param self La cola
 */
void ColaEventos_borra(ColaEventos *self);
#endif