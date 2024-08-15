#include "temporizador.h"
#include "tempo_ms.h"
#include "definicion_eventos.h"
#include "container_of.h"

void Temporizador_init(Temporizador *self)
{
    ColaEventos_init(&self->cola);
    FuenteEventos_init(&self->fuente);
    self->temporizando = false;
}
void Temporizador_ejecuta(Temporizador *self)
{
    static const Evento tiempoCumplido = {.mensaje=Mensaje_TIEMPO_CUMPLIDO};

    if(self->temporizando){
        const uint32_t tiempo = Tempo_obtMilisegundos();
        if(tiempo-self->tiempoInicial >= self->espera){
            self->temporizando = false;
            FuenteEventos_enviaEvento(&self->fuente,&tiempoCumplido);
        }
    }else{
        const Evento *e;
        if(!ColaEventos_toma(&self->cola,&e)) return;
        if(e->mensaje == MensajeInt_TEMPORIZA){
            self->espera = container_of(e,const EventoInt,evento)->valor;
            self->tiempoInicial = Tempo_obtMilisegundos();
            self->temporizando = true;
        }
    }
}
ObservadorEventos *Temporizador_obtObservador(Temporizador *self)
{
    return ColaEventos_obtObservador(&self->cola);
}
bool Temporizador_registraObservador(Temporizador *self, ObservadorEventos *obs)
{
    return FuenteEventos_registraObservador(&self->fuente, obs);
}