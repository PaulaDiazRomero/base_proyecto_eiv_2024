#include "cola_eventos.h"

void ColaEventos_init(ColaEventos *self)
{
    ColaEventos_borra(self);
}
bool ColaEventos_pon(ColaEventos *self,const Evento *evento)
{
    if (self->escritura - self->lectura >= MAX_EVENTOS) return false;
    self->eventos[(self->escritura++)&MAX_EVENTOS] = evento;
    return true;
}
bool ColaEventos_toma(ColaEventos *self,const Evento **evento)
{
    if (self->escritura == self->lectura) return false;
    *evento = self->eventos[(self->lectura++)&MAX_EVENTOS];
    return true;
}

void ColaEventos_borra(ColaEventos *self)
{
    self->escritura=0;
    self->lectura = 0;
}