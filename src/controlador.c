#include "controlador.h"
#include "container_of.h"


typedef struct Controlador{
    Mef mef;
    int temperatura;
    int tiempo;
    int temperaturaMaxima;
    int tiempoMaximo;
    int tiempoRestante;
    bool finPaso;
}Controlador;

static Controlador controlador;

static void ingresaTemperatura(Mef *m,const Evento *e);
static void ingresaTiempo(Mef *m,const Evento *e);
static void carga(Mef *m,const Evento *e);
static void calienta(Mef *m,const Evento *e);
static void tratamiento(Mef *m,const Evento *e);
static void enfria(Mef *m,const Evento *e);
static void descarga(Mef *m,const Evento *e);

Mef * Controlador_init(int temperaturaMaxima,int tiempoMaximo)
{
    Mef_init(&controlador.mef, ingresaTemperatura);
    controlador.temperaturaMaxima=temperaturaMaxima;
    controlador.tiempoMaximo=tiempoMaximo;
    return &controlador.mef;
}

static const EventoInt temporizaPantalla = {.evento.mensaje=MensajeInt_TEMPORIZA,.valor=1000};


static void ingresaTemperatura(Mef *m,const Evento *e)
{
    static EventoInt muestraIngresaTemperatura = {.evento.mensaje=MensajeInt_INGRESA_TEMP};
    static const Evento establecePosicionInicialFuera = {.mensaje=Mensaje_SPOS_FUERA};
    static EventoInt temperaturaInicial = {.evento.mensaje=MensajeInt_STEMP};

    const Mensaje mensaje = Evento_obtMensaje(e);
    switch (mensaje)
    {
    case Mensaje_ENTRADA:
        controlador.temperatura = 0;
        muestraIngresaTemperatura.valor = controlador.temperatura;
        temperaturaInicial.valor = 0;
        Mef_enviaEvento(m,&muestraIngresaTemperatura.evento);
        Mef_enviaEvento(m,&establecePosicionInicialFuera);
        Mef_enviaEvento(m,&temperaturaInicial.evento);
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
    static const Evento muestraDentro = {.mensaje=Mensaje_CARGANDO_DENTRO};
    static const Evento averiguaPosicion = {.mensaje=Mensaje_POSP};
    static const EventoInt temporizaPantalla = {.evento.mensaje=MensajeInt_TEMPORIZA,.valor=1000};
    

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
    break; case Mensaje_RESPUESTA_ERROR:
        Mef_transiciona(m,carga);
    break;case Mensaje_RESPUESTA_DENTRO:
        Mef_enviaEvento(m,&muestraDentro);
        Mef_enviaEvento(m,&temporizaPantalla.evento);
    break;case Mensaje_TIEMPO_CUMPLIDO:
        Mef_transiciona(m,calienta);
    break;case Mensaje_NUMERAL:
            Mef_transiciona(m,descarga); 
    break;default:
    break;
    }
}


static void calienta(Mef *m,const Evento *e){
    static const Evento muestraCalentando = {.mensaje=Mensaje_CALENTANDO};
    static EventoInt enviaTemperatura= {.evento.mensaje=MensajeInt_STEMP};
    static const Evento averiguaTemperatura = {.mensaje=Mensaje_TEMPP};
    static EventoInt muestraTemperatura = {.evento.mensaje=MensajeInt_CALENTANDO_TEMP};
    static const EventoInt temporizaPantalla = {.evento.mensaje=MensajeInt_TEMPORIZA,.valor=1000};

    const Mensaje mensaje = Evento_obtMensaje(e);
    switch (mensaje)
    {
    case Mensaje_ENTRADA:
        enviaTemperatura.valor = controlador.temperatura;
        Mef_enviaEvento(m,&enviaTemperatura.evento);
        Mef_enviaEvento(m,&muestraCalentando);
    break;case Mensaje_RESPUESTA_OK:
        Mef_enviaEvento(m,&averiguaTemperatura);
    break;case MensajeInt_RESPUESTA_NUMERO:{
        const int t = container_of(e,const EventoInt,evento)->valor;
        muestraTemperatura.valor = t;
        Mef_enviaEvento(m,&muestraTemperatura.evento);
        if(t<controlador.temperatura){
            Mef_enviaEvento(m,&averiguaTemperatura);
        }else{
            Mef_enviaEvento(m,&temporizaPantalla.evento);
        }    
    }
    break;case Mensaje_TIEMPO_CUMPLIDO:
        Mef_transiciona(m,tratamiento); 
    break;case Mensaje_RESPUESTA_ERROR:
        Mef_transiciona(m,calienta);  
    break;case Mensaje_NUMERAL:
            Mef_transiciona(m,enfria); 
    break;default:
    break; 
    }
}


static void tratamiento(Mef *m,const Evento *e){
    static EventoInt mensajeTratamiento = {.evento.mensaje=MensajeInt_TRATAMIENTO};
    static const EventoInt temporizaMinuto = {.evento.mensaje=MensajeInt_TEMPORIZA,.valor=1000};
    

    const Mensaje mensaje = Evento_obtMensaje(e);
    switch (mensaje)
    {
    case Mensaje_ENTRADA:
        controlador.tiempoRestante = controlador.tiempo;
        mensajeTratamiento.valor = controlador.tiempoRestante;
        Mef_enviaEvento(m,&mensajeTratamiento.evento);
        Mef_enviaEvento(m,&temporizaMinuto.evento);
    break; case Mensaje_TIEMPO_CUMPLIDO:
        if(controlador.tiempoRestante != 0){
            mensajeTratamiento.valor = --controlador.tiempoRestante;
            Mef_enviaEvento(m,&mensajeTratamiento.evento);
            if(controlador.tiempoRestante == 0){
                Mef_enviaEvento(m,&temporizaPantalla.evento);
            }else{
                Mef_enviaEvento(m,&temporizaMinuto.evento);
            }
        }else{
            Mef_transiciona(m,&enfria);
        }
    break;case Mensaje_NUMERAL:
            Mef_transiciona(m,enfria); 
    break;default:
    break; 
    }
}


static void enfria(Mef *m,const Evento *e){
    static const Evento muestraEnfriamiento = {.mensaje=Mensaje_ENFRIAMIENTO};
    static EventoInt temperaturaFinal = {.evento.mensaje=MensajeInt_STEMP};
    static const Evento averiguaTemperatura = {.mensaje=Mensaje_TEMPP};
    static EventoInt muestraTempEnfriamiento = {.evento.mensaje=MensajeInt_ENFRIAMIENTO};

    const Mensaje mensaje = Evento_obtMensaje(e);
    switch (mensaje)
    {
    case Mensaje_ENTRADA:
        controlador.finPaso = false;
        temperaturaFinal.valor = 0;
        Mef_enviaEvento(m,&temperaturaFinal.evento);
        Mef_enviaEvento(m,&muestraEnfriamiento);
    break;case Mensaje_RESPUESTA_OK:
        Mef_enviaEvento(m,&averiguaTemperatura);
    break;case MensajeInt_RESPUESTA_NUMERO:{
        const int t = container_of(e,const EventoInt,evento)->valor;
        const int temperaturaFria = 40;
        muestraTempEnfriamiento.valor = t;
        Mef_enviaEvento(m,&muestraTempEnfriamiento.evento);
        if(t>temperaturaFria){
            Mef_enviaEvento(m,&averiguaTemperatura);
        }else{
            controlador.finPaso = true;
            Mef_enviaEvento(m,&temporizaPantalla.evento);
        }
    }
    break;case Mensaje_TIEMPO_CUMPLIDO:
        if(controlador.finPaso){
            Mef_transiciona(m,&descarga);
        }
    break;case Mensaje_RESPUESTA_ERROR:
        Mef_transiciona(m,enfria);  
    break;default:
    break;
    }
}


static void descarga(Mef *m,const Evento *e){
    static const Evento establecePosicionFinal = {.mensaje=Mensaje_SPOS_FUERA};
    static const Evento muestraDescargando = {.mensaje=Mensaje_DESCARGANDO};
    static const Evento muestraEnTransito = {.mensaje=Mensaje_DESCARGANDO_EN_TRANSITO};
    static const Evento muestraFuera = {.mensaje=Mensaje_DESCARGANDO_FUERA};
    static const Evento averiguaPosicion = {.mensaje=Mensaje_POSP};
    static const Evento muestraDentro = {.mensaje=Mensaje_DESCARGANDO_DENTRO};

    const Mensaje mensaje = Evento_obtMensaje(e);
    switch (mensaje)
    {
    case Mensaje_ENTRADA:
        controlador.finPaso = false;
        Mef_enviaEvento(m,&establecePosicionFinal);
        Mef_enviaEvento(m,&muestraDescargando);
    break;case Mensaje_RESPUESTA_OK:
        Mef_enviaEvento(m,&averiguaPosicion);
    break;case Mensaje_RESPUESTA_EN_TRANSITO:
        Mef_enviaEvento(m,&muestraEnTransito);
        Mef_enviaEvento(m,&averiguaPosicion);
    break;case Mensaje_RESPUESTA_FUERA:
        controlador.finPaso = true;
        Mef_enviaEvento(m,&muestraFuera);
        Mef_enviaEvento(m,&temporizaPantalla.evento);
    break;case Mensaje_TIEMPO_CUMPLIDO:
        if(controlador.finPaso){
            Mef_transiciona(m,ingresaTemperatura);
        }
    break;case Mensaje_RESPUESTA_DENTRO:
        Mef_enviaEvento(m,&muestraDentro);
        Mef_enviaEvento(m,&averiguaPosicion);
    break; case Mensaje_RESPUESTA_ERROR:
        Mef_transiciona(m,descarga);
    break;default:
    break;
    }   
}