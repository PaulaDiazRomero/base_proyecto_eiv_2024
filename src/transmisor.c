#include "transmisor.h"
#include "serie.h"
#include "definicion_eventos.h"
#include <string.h>
#include <stdio.h>
#include "container_of.h"

enum{TAMANO_BUFFER=17};

typedef struct Transmisor{
    ColaEventos cola;
    char buffer[TAMANO_BUFFER];
    int indice;
    bool transmitiendo;
}Transmisor;

static Transmisor transmisor;

void Transmisor_init(void){
    ColaEventos_init(&transmisor.cola);
    transmisor.transmitiendo = false;
}


ObservadorEventos *Transmisor_obtObservador(void){

    return ColaEventos_obtObservador(&transmisor.cola);
}

static void transmite (const char *cadena);

void Transmisor_ejecuta(void){
    const Evento *e;
    if (transmisor.transmitiendo){
        const char c = transmisor.buffer[transmisor.indice];
        if(c && Serie_intentaEnviarCaracter(c)) transmisor.indice++;
        if (!c) transmisor.transmitiendo=false;
    }else if (ColaEventos_toma(&transmisor.cola, &e)){
        const Mensaje mensaje = Evento_obtMensaje(e);
        switch (mensaje)
        {
        case Mensaje_SPOS_DENTRO:
            transmite("spos dentro\n");
        break;case Mensaje_SPOS_FUERA:
            transmite("spos fuera\n");
        break;case Mensaje_POSP:
            transmite("pos?\n");
        break;case Mensaje_TEMPP:
            transmite("temp?\n");
        break;case MensajeInt_STEMP:{
            const int valor=container_of(e,const EventoInt,evento)->valor;
            snprintf (transmisor.buffer, TAMANO_BUFFER,"stemp %d\n",valor);
            transmisor.indice=0;
            transmisor.transmitiendo=true;
        }
        break;default:
        break;
        }
    }

}



static void transmite(const char *cadena)
{
    strncpy(transmisor.buffer,cadena,TAMANO_BUFFER);
    transmisor.buffer[TAMANO_BUFFER-1]=0;
    transmisor.indice=0;
    transmisor.transmitiendo=true;
}
