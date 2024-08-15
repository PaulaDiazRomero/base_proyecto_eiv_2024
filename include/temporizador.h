#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H
#include "cola_eventos.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct Temporizador{
    ColaEventos cola;
    FuenteEventos fuente;
    uint32_t tiempoInicial;
    uint32_t espera;
    bool temporizando;
}Temporizador;


void Temporizador_init(Temporizador *self);
void Temporizador_ejecuta(Temporizador *self);
ObservadorEventos *Temporizador_obtObservador(Temporizador *self);
bool Temporizador_registraObservador(Temporizador *self, ObservadorEventos *obs);


//init (self) temporizando en false
//ejecuta (self)
//obtObservador (self)
//registraObservador (self,observador)


#endif
