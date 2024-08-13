#ifndef PANTALLA_H
#define PANTALLA_H
#include "lcd.h"
#include "definicion_eventos.h"

typedef struct Pantalla{
    ColaEventos cola;
    Lcd *lcd;
}Pantalla;

void Pantalla_init(Pantalla *self, Lcd *lcd);
ObservadorEventos *Pantalla_obtObservador(Pantalla *self);
void Pantalla_ejecuta(Pantalla *self);


#endif
