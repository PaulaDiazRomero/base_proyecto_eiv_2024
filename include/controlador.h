#ifndef CONTROLADOR_H
#define CONTROLADOR_H
#include "mef.h"
#include "definicion_eventos.h"
#include <stdbool.h>


/**
 * @brief Inicializa y retorna la máquina de estado finito del controlador
 * 
 * @return Mef* 
 */
Mef * Controlador_init(int temperaturaMaxima,int tiempoMaximo);

#endif