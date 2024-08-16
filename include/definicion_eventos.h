#ifndef DEFINICION_EVENTOS_H
#define DEFINICION_EVENTOS_H

#include "mef.h"

enum DefinicionMensajes{
    MensajeInt_DIGITO = Mensaje_USUARIO,
    Mensaje_NUMERAL,
    Mensaje_ASTERISCO,
    MensajeInt_INGRESA_TEMP,
    MensajeInt_INGRESA_TIEMPO,
    Mensaje_TECLA_NO_VALIDA,
    Mensaje_SPOS_DENTRO,
    Mensaje_SPOS_FUERA,
    Mensaje_POSP,
    MensajeInt_STEMP,
    Mensaje_TEMPP,
    MensajeInt_STIEMPO,
    Mensaje_RESPUESTA_OK,
    Mensaje_RESPUESTA_EN_TRANSITO,
    Mensaje_RESPUESTA_FUERA,
    Mensaje_RESPUESTA_DENTRO,
    MensajeInt_RESPUESTA_NUMERO,
    Mensaje_RESPUESTA_ERROR,
    Mensaje_CARGANDO,
    Mensaje_CARGANDO_EN_TRANSITO,
    Mensaje_CARGANDO_FUERA,
    Mensaje_CARGANDO_DENTRO,
    MensajeInt_CARACTER_RECIBIDO,
    Mensaje_CALENTANDO,
    MensajeInt_CALENTANDO_TEMP,
    Mensaje_TIEMPO_CUMPLIDO,
    MensajeInt_TEMPORIZA,
    MensajeInt_TRATAMIENTO,
    Mensaje_ENFRIAMIENTO,
    MensajeInt_ENFRIAMIENTO,
    Mensaje_DESCARGANDO,
    Mensaje_DESCARGANDO_EN_TRANSITO,
    Mensaje_DESCARGANDO_FUERA,
    Mensaje_DESCARGANDO_DENTRO
};

typedef struct EventoInt{
    const Evento evento;
    int valor;             //Transformación de mensaje de lectura a int (sólo si es int, sino no hace la conversión)
}EventoInt;


#endif
