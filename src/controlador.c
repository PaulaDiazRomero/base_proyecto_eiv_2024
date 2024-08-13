#include "controlador.h"
#include "container_of.h"

typedef struct Controlador{
    Mef mef;
    int temperatura;
    int tiempo;
    int temperaturaMaxima;
    int tiempoMaximo;
}Controlador;

static Controlador controlador;

static void ingresaTemperatura(Mef *m,const Evento *e);
static void ingresaTiempo(Mef *m,const Evento *e);
static void carga(Mef *m,const Evento *e);

Mef * Controlador_init(int temperaturaMaxima,int tiempoMaximo)
{
    Mef_init(&controlador.mef, ingresaTemperatura);
    controlador.temperaturaMaxima=temperaturaMaxima;
    controlador.tiempoMaximo=tiempoMaximo;
    return &controlador.mef;
}



static void ingresaTemperatura(Mef *m,const Evento *e)
{
    static EventoInt muestraIngresaTemperatura = {.evento.mensaje=MensajeInt_INGRESA_TEMP};

    const Mensaje mensaje = Evento_obtMensaje(e);
    switch (mensaje)
    {
    case Mensaje_ENTRADA:
        controlador.temperatura = 0;
        muestraIngresaTemperatura.valor = controlador.temperatura;
        Mef_enviaEvento(m,&muestraIngresaTemperatura.evento);
    break;case MensajeInt_DIGITO:{
        const int d = container_of(e,const EventoInt,evento)->valor;
        const int nuevoValor = (10*controlador.temperatura)+d;
        controlador.temperatura = nuevoValor < controlador.temperaturaMaxima ? nuevoValor:controlador.temperaturaMaxima;
        muestraIngresaTemperatura.valor = controlador.temperatura;
        Mef_enviaEvento(m,&muestraIngresaTemperatura.evento);
    }
    break;case Mensaje_NUMERAL:
        Mef_transiciona(m, ingresaTemperatura);
    break;case Mensaje_ASTERISCO:
        if(controlador.temperatura != 0){
            Mef_transiciona(m, ingresaTiempo);
        }
        else{
            Mef_transiciona(m, ingresaTemperatura);    
        }
    break;default:
    break;
    }
}


static void ingresaTiempo(Mef *m,const Evento *e)
{
    static EventoInt muestraIngresaTiempo = {.evento.mensaje=MensajeInt_INGRESA_TIEMPO};

    const Mensaje mensaje = Evento_obtMensaje(e);
    switch (mensaje)
    {
    case Mensaje_ENTRADA:
        controlador.tiempo = 0;
        muestraIngresaTiempo.valor = controlador.tiempo;
        Mef_enviaEvento(m,&muestraIngresaTiempo.evento);
    break;case MensajeInt_DIGITO:{
        const int d = container_of(e,const EventoInt,evento)->valor;
        const int nuevoValor = (10*controlador.tiempo)+d;
        controlador.tiempo = nuevoValor < controlador.tiempoMaximo ? nuevoValor:controlador.tiempoMaximo;
        muestraIngresaTiempo.valor = controlador.tiempo;
        Mef_enviaEvento(m,&muestraIngresaTiempo.evento); 
    }
    break;case Mensaje_NUMERAL:
        if(controlador.tiempo != 0){
            Mef_transiciona(m, ingresaTiempo);
        }
        else{
            Mef_transiciona(m, ingresaTemperatura);    
        }
    break;case Mensaje_ASTERISCO:   
        if(controlador.tiempo != 0){
            Mef_transiciona(m, carga);
        }
        else{
            Mef_transiciona(m, ingresaTiempo);    
        }
    break;default:
    break;
    }
}


static void carga(Mef *m,const Evento *e)
{
    static const Evento establecePosicionDeseadaDentro = {.mensaje=Mensaje_SPOS_DENTRO};
    static const Evento muestraCargando = {.mensaje=Mensaje_CARGANDO};
    static const Evento muestraEnTransito = {.mensaje=Mensaje_CARGANDO_EN_TRANSITO};
    static const Evento muestraFuera = {.mensaje=Mensaje_CARGANDO_FUERA};
    static const Evento averiguaPosicion = {.mensaje=Mensaje_POSP};

    const Mensaje mensaje = Evento_obtMensaje(e);
    switch (mensaje)
    {
    case Mensaje_ENTRADA:
        Mef_enviaEvento(m,&establecePosicionDeseadaDentro);
        Mef_enviaEvento(m,&muestraCargando);
    break;case Mensaje_RESPUESTA_OK:
        Mef_enviaEvento(m,&averiguaPosicion);
    break;case Mensaje_RESPUESTA_EN_TRANSITO:
        Mef_enviaEvento(m,&muestraEnTransito);
        Mef_enviaEvento(m,&averiguaPosicion);
    break;case Mensaje_RESPUESTA_FUERA:
        Mef_enviaEvento(m,&muestraFuera);
        Mef_enviaEvento(m,&averiguaPosicion);
    break;default:
    break;
    }
}
