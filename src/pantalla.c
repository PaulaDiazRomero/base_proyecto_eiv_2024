#include "pantalla.h"
#include <stdio.h>
#include "container_of.h"

void Pantalla_init(Pantalla *self, Lcd *lcd){
    self->lcd = lcd;
    ColaEventos_init(&self->cola);
}
ObservadorEventos *Pantalla_obtObservador(Pantalla *self){
    return ColaEventos_obtObservador(&self->cola);
}

static void Pantalla_escribeValor(Pantalla *self,int valor)
{
    enum{TAMANO_BUFFER=17};
    static char buffer[TAMANO_BUFFER];
    Lcd *const lcd = self->lcd;
    Lcd_establecePosicion(lcd,1,0);
    Lcd_escribeCadena(lcd,"                ");
    Lcd_establecePosicion(lcd,1,0);
    snprintf(buffer,TAMANO_BUFFER-1,"%d",valor);
    Lcd_escribeCadena(lcd,buffer);
}

void Pantalla_ejecuta(Pantalla *self)
{
    Lcd *const lcd = self->lcd;
    const Evento *e;

    if(!ColaEventos_toma(&self->cola,&e)) return;

    switch(Evento_obtMensaje(e)){
    case MensajeInt_INGRESA_TEMP:{
        const int valor = container_of(e,const EventoInt,evento)->valor;
        Lcd_establecePosicion(lcd,0,0);
        Lcd_escribeCadena(lcd,"Ingr.Temperatura");
        Pantalla_escribeValor(self,valor);
    }
    break;case MensajeInt_INGRESA_TIEMPO:{
        const int valor = container_of(e,const EventoInt,evento)->valor;
        Lcd_establecePosicion(lcd,0,0);
        Lcd_escribeCadena(lcd,"Ingresa Tiempo  ");
        Pantalla_escribeValor(self,valor);
    }
    break;case Mensaje_CARGANDO:{
        Lcd_establecePosicion(lcd,0,0);
        Lcd_escribeCadena(lcd,"Cargando        "); 
        Lcd_establecePosicion(lcd,1,0);
        Lcd_escribeCadena(lcd,"                ");
    }
    break;case Mensaje_CARGANDO_EN_TRANSITO:{
        Lcd_establecePosicion(lcd,0,0);
        Lcd_escribeCadena(lcd,"Cargando");
        Lcd_establecePosicion(lcd,1,0);
        Lcd_escribeCadena(lcd,"En Transito");
    }
    break;case Mensaje_CARGANDO_FUERA:{
        Lcd_establecePosicion(lcd,0,0);
        Lcd_escribeCadena(lcd,"Cargando");
        Lcd_establecePosicion(lcd,1,0);
        Lcd_escribeCadena(lcd,"Fuera");
    }
    break;case Mensaje_CALENTANDO:{
        Lcd_establecePosicion(lcd,0,0);
        Lcd_escribeCadena(lcd,"Calentando       "); 
        Lcd_establecePosicion(lcd,1,0);
        Lcd_escribeCadena(lcd,"                ");
    }
    break;case MensajeInt_CALENTANDO_TEMP:{
        const int t = container_of(e,const EventoInt,evento)->valor;
        Lcd_establecePosicion(lcd,0,0);
        Lcd_escribeCadena(lcd,"Calentando       ");
        Lcd_establecePosicion(lcd,1,0);
        snprintf (self->buffer, TAMANO_BUFFER,"Temperatura %-4d",t);
        Lcd_escribeCadena(lcd,self->buffer);
    }
    break;case MensajeInt_TRATAMIENTO:{
        const int t = container_of(e,const EventoInt,evento)->valor;
        Lcd_establecePosicion(lcd,0,0);
        Lcd_escribeCadena(lcd,"Tratamiento      "); 
        Lcd_establecePosicion(lcd,1,0);
        snprintf (self->buffer, TAMANO_BUFFER,"Faltan %5d min",t);
        Lcd_escribeCadena(lcd,self->buffer);
    }
    break;case Mensaje_ENFRIAMIENTO:{
        Lcd_establecePosicion(lcd,0,0);
        Lcd_escribeCadena(lcd,"Enfriando       "); 
        Lcd_establecePosicion(lcd,1,0);
        Lcd_escribeCadena(lcd,"                ");
    }
    break;case MensajeInt_ENFRIAMIENTO:{
        const int t = container_of(e,const EventoInt,evento)->valor;
        Lcd_establecePosicion(lcd,0,0);
        Lcd_escribeCadena(lcd,"Enfriando       ");
        Lcd_establecePosicion(lcd,1,0);
        snprintf (self->buffer, TAMANO_BUFFER,"Temperatura %-4d",t);
        Lcd_escribeCadena(lcd,self->buffer);
    }
    break;default:
    break;
    }
}
