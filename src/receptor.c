#include "receptor.h"
#include "container_of.h"
#include <ctype.h> // para isgraph, isdigit
#include <string.h> // para strcmp
#include <stdlib.h> // para atoi

// typedef del objeto
typedef struct Receptor{
    Mef mef;
    int indice;
    char linea[MAX_CHAR_LINEA + 1];
}Receptor;

// instancia del objeto (static)
static Receptor receptor;

// declaración de los estados
static void nuevaRecepcion(Mef *m,const Evento *e);
static void recibeLinea(Mef *m,const Evento *e);

// inicialización
Mef * Receptor_init(void)
{
    Mef_init(&receptor.mef, nuevaRecepcion);
    return &receptor.mef;
}

// funciones auxiliares

static void borraLinea(void)
{
    receptor.indice = 0;
    receptor.linea[0] = 0;
}

static void sumaChar(char c)
{
    receptor.linea[receptor.indice] = c;
    receptor.indice++; 
}

typedef struct EntradaTabla{
    const char * clave;
    Evento valor;
}EntradaTabla; 

static void procesaLinea(Mef *m)
{
    static const EntradaTabla casosEspeciales[]={
        {.clave="ok",.valor={.mensaje=Mensaje_RESPUESTA_OK}},
        {.clave="dentro",.valor={.mensaje=Mensaje_RESPUESTA_DENTRO}},
        {.clave="fuera",.valor={.mensaje=Mensaje_RESPUESTA_FUERA}},
        {.clave="en transito",.valor={.mensaje=Mensaje_RESPUESTA_EN_TRANSITO}}
    };
    static const Evento error = {.mensaje=Mensaje_RESPUESTA_ERROR};
    static EventoInt numero = {.evento.mensaje=MensajeInt_RESPUESTA_NUMERO};
    static const unsigned numCasosEspeciales = sizeof(casosEspeciales)/sizeof(*casosEspeciales);
    for(unsigned i=0; i<numCasosEspeciales;++i){
        const char *const clave = casosEspeciales[i].clave;
        const Evento *const evento = &casosEspeciales[i].valor;
        if(0 == strcmp(receptor.linea,clave)){
            Mef_enviaEvento(m,evento);
            return;
        }
    }
    if (isdigit(receptor.linea[0])){
        numero.valor=atoi(receptor.linea);
        Mef_enviaEvento(m,&numero.evento);
        return;
    }
    Mef_enviaEvento(m,&error);
}

// definición de los estados
static void nuevaRecepcion(Mef *m,const Evento *e)
{
    const Mensaje mensaje = Evento_obtMensaje(e);
    switch (mensaje)
    {
    case Mensaje_ENTRADA:
        borraLinea();
    break;case MensajeInt_CARACTER_RECIBIDO:{
        const char c=(char) container_of(e,const EventoInt,evento)->valor;
        if (isgraph(c)){
            sumaChar(c);
            Mef_transiciona(m, recibeLinea);
        }
    }
    break;default:
    break;
    }
}
static void recibeLinea(Mef *m,const Evento *e)
{
    const Mensaje mensaje = Evento_obtMensaje(e);
    switch (mensaje)
    {
    case MensajeInt_CARACTER_RECIBIDO:{
        const char c=(char) container_of(e,const EventoInt,evento)->valor;
        if ((c != '\n') && (c != '\r')){
            if (receptor.indice < MAX_CHAR_LINEA) sumaChar(c);
        }else{
            Mef_transiciona(m, nuevaRecepcion);
        }
    }
    break;case Mensaje_SALIDA:
        sumaChar(0);
        procesaLinea(m);
    break;default:
    break;
    }
}
