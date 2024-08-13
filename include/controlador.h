#ifndef CONTROLADOR_H
#define CONTROLADOR_H
#include "mef.h"
#include <stdbool.h>

enum MensajesControlador{
    MensajeInt_DIGITO = Mensaje_USUARIO,
    Mensaje_NUMERAL,
    Mensaje_ASTERISCO,
    MensajeInt_INGRESA_TEMP,
    MensajeInt_INGRESA_TIEMPO,
    Mensaje_TECLA_NO_VALIDA,
};

typedef struct EventoInt{
    Evento evento;
    int valor;             //Transformación de mensaje de lectura a int (sólo si es int, sino no hace la conversión)
}EventoInt;

/**
 * @brief Inicializa y retorna la máquina de estado finito del controlador
 * 
 * @return Mef* 
 */
Mef * Controlador_init(int temperaturaMaxima,int tiempoMaximo);        

#endif