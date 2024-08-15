#ifndef PANTALLA_H
#define PANTALLA_H
#include "lcd.h"
#include "definicion_eventos.h"

enum{TAMANO_BUFFER=20};

typedef struct Pantalla{
    ColaEventos cola;
    Lcd *lcd;
    char buffer[TAMANO_BUFFER];
}Pantalla;

void Pantalla_init(Pantalla *self, Lcd *lcd);
ObservadorEventos *Pantalla_obtObservador(Pantalla *self);
void Pantalla_ejecuta(Pantalla *self);


#endif
