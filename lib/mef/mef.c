#include "mef.h"
#include <stddef.h>
#include "container_of.h"

static struct{
    Evento inicializa;
    Evento entrada;
    Evento salida;
}const eventosSistema={
    .inicializa={.mensaje=Mensaje_INICIALIZA},
    .entrada={.mensaje=Mensaje_ENTRADA},
    .salida={.mensaje=Mensaje_SALIDA}
};

static bool observadorRecibeEvento(ObservadorEventos *observador,const Evento *evento)
{
    return Mef_recibeEvento(container_of(observador,Mef,observador),evento);
}

void Mef_init(Mef *self,Estado inicial)
{
    static const ObservadorEventos_VT observadorVt = {.recibeEvento=observadorRecibeEvento};
    self->observador.vptr_ = &observadorVt;
    self->inicial = inicial;
    self->estado = NULL;
    self->transicion = false;
    ColaEventos_init(&self->eventos);
    FuenteEventos_init(&self->fuente);
}
void Mef_ejecuta(Mef *self)
{
    const Evento *evento;
    
    if(!self->estado) evento = &eventosSistema.inicializa;
    else if(!ColaEventos_toma(&self->eventos,&evento)) return;

    Estado estadoInicial = self->estado;
    if (Evento_obtMensaje(evento) == Mensaje_INICIALIZA){
        self->estado = self->inicial;
        self->estado(self,evento);
        self->transicion = true;
    }else{
        self->estado(self,evento);
    }
    if(self->transicion)
    {
        self->transicion = false;
        if(estadoInicial) estadoInicial(self,&eventosSistema.salida);
        self->estado(self,&eventosSistema.entrada);
    }
}

void Mef_transiciona(Mef *self,Estado siguiente)
{
    self->estado = siguiente;
    self->transicion = true;
}
bool Mef_recibeEvento(Mef *self,const Evento *evento)
{
    return ColaEventos_pon(&self->eventos,evento);
}

void Mef_finaliza(Mef *self)
{
    if(self->estado) self->estado(self,&eventosSistema.salida);
    self->estado = NULL;
    ColaEventos_borra(&self->eventos);
}

ObservadorEventos *Mef_obtObservador(Mef *self)
{
    return &self->observador;
}

bool Mef_enviaEvento(Mef *self,const Evento *e)
{
    return FuenteEventos_enviaEvento(&self->fuente,e);
}

bool Mef_registraObservador(Mef *self,ObservadorEventos *observador)
{
    return FuenteEventos_registraObservador(&self->fuente,observador);
}

bool Mef_desregistraObservador(Mef *self,ObservadorEventos *observador)
{
    return FuenteEventos_desregistraObservador(&self->fuente,observador);
}