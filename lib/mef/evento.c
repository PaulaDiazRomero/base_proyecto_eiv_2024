#include "evento.h"
#include <stddef.h>

Mensaje Evento_obtMensaje(const Evento * e)
{
    return e->mensaje;
}

bool ObservadorEventos_recibeEvento(ObservadorEventos *self,const Evento *e)
{
    return self->vptr_->recibeEvento(self,e);
}

void FuenteEventos_init(FuenteEventos *self)
{
    self->cantidadObservadores = 0;
}

bool FuenteEventos_registraObservador(FuenteEventos *self,ObservadorEventos *obs)
{
    if(self->cantidadObservadores >= MAX_OBSERVADORES) return false;
    self->observadores[self->cantidadObservadores++] = obs;
    return true;
}

bool FuenteEventos_enviaEvento(FuenteEventos *self,const Evento* evento)
{
    bool recibido = false;
    for(unsigned i=0; i< self->cantidadObservadores;++i){
        ObservadorEventos *const observador = self->observadores[i];
        if(ObservadorEventos_recibeEvento(observador,evento)) recibido=true;
    }
    return recibido;
}

bool FuenteEventos_desregistraObservador(FuenteEventos *self,ObservadorEventos *obs)
{
    bool desregistrado = false;
    unsigned i;
    for(i=0;i<self->cantidadObservadores;++i){;
        ObservadorEventos *const obs_i = self->observadores[i];
        if(obs_i == obs) break;
    }
    if (i < self->cantidadObservadores){
        desregistrado = true;
        self->observadores[i] = self->observadores[--self->cantidadObservadores];
    }
    return desregistrado;
}