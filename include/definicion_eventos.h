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
    Mensaje_POSP,
    Mensaje_RESPUESTA_OK,
    Mensaje_RESPUESTA_EN_TRANSITO,
    Mensaje_RESPUESTA_FUERA,
    Mensaje_RESPUESTA_DENTRO,
    MensajeInt_RESPUESTA_NUMERO,
    Mensaje_RESPUESTA_ERROR,
    Mensaje_CARGANDO,
    Mensaje_CARGANDO_EN_TRANSITO,
    Mensaje_CARGANDO_FUERA,
    MensajeInt_CARACTER_RECIBIDO,
};

typedef struct EventoInt{
    Evento evento;
    int valor;             //Transformación de mensaje de lectura a int (sólo si es int, sino no hace la conversión)
}EventoInt;


#endif
