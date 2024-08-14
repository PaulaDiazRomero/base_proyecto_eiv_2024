#ifndef TRANSMISOR_H
#define TRANSMISOR_H

#include "cola_eventos.h"

void Transmisor_init(void);
ObservadorEventos *Transmisor_obtObservador(void);
void Transmisor_ejecuta(void);

#endif
