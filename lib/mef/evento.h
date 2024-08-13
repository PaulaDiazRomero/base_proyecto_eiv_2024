#ifndef EVENTO_H
#define EVENTO_H
#include <stdbool.h>

#ifndef MAX_OBSERVADORES
#define MAX_OBSERVADORES 5
#endif

typedef struct Evento Evento;
typedef unsigned Mensaje;
typedef struct ObservadorEventos ObservadorEventos;
typedef struct FuenteEventos FuenteEventos;

typedef struct ObservadorEventos_VT{
    bool (*const recibeEvento)(ObservadorEventos *self,const Evento *e);
}ObservadorEventos_VT;
struct ObservadorEventos{
    const ObservadorEventos_VT *vptr_;
};
struct Evento
{
    Mensaje mensaje;
};

struct FuenteEventos{
    unsigned cantidadObservadores;
    ObservadorEventos *observadores[MAX_OBSERVADORES];
};
/**
 * @brief Obtiene el mensaje asociado a un evento
 * 
 * @param self El evento
 * @return Mensaje El mensaje
 */
Mensaje Evento_obtMensaje(const Evento * self);

/**
 * @brief Envía un evento a un observador de eventos
 * 
 * @param self El observador
 * @param e El evento
 * @retval true Evento recibido
 * @retval false No pudo recibir el evento
 */
bool ObservadorEventos_recibeEvento(ObservadorEventos *self,const Evento *e);

/**
 * @brief Inicializa una fuente de eventos
 * 
 * @param self La fuente
 */
void FuenteEventos_init(FuenteEventos *self);
/**
 * @brief Registra un observador en una fuente de eventos
 * 
 * @param self La fuente
 * @param obs El observador
 * @retval true Registro exitoso
 * @retval false No pudo registrar observador
 */
bool FuenteEventos_registraObservador(FuenteEventos *self,ObservadorEventos *obs);

/**
 * @brief Desregistra un observador previamente registrado
 * 
 * @param self La fuente
 * @param obs El observador
 * @retval true Observador desregistrado con exito 
 * @retval false No pudo desregistrarse el observador
 */
bool FuenteEventos_desregistraObservador(FuenteEventos *self,ObservadorEventos *obs);

/**
 * @brief Envía un evento a los observadores registrados en una fuente
 * 
 * @param self La fuente
 * @param evento El evento
 * @retval true Al menos un observador recibió el evento 
 * @retval false Ningún observador recibió el evento
 */
bool FuenteEventos_enviaEvento(FuenteEventos *self,const Evento* evento);
#endif