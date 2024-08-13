#include "cola_eventos.h"
#include "container_of.h"

static bool observadorRecibeEvento(ObservadorEventos *obs,const Evento *e)
{
    ColaEventos *const self = container_of(obs,ColaEventos,observador);
    return ColaEventos_pon(self,e);
}

void ColaEventos_init(ColaEventos *self)
{
    static const ObservadorEventos_VT observadorVt = {.recibeEvento=observadorRecibeEvento};
    self->observador.vptr_ = &observadorVt;
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

ObservadorEventos *ColaEventos_obtObservador(ColaEventos *self)
{
    return &self->observador;   
}