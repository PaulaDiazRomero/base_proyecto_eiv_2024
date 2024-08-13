#ifndef RECEPTOR_H
#define RECEPTOR_H

#include "definicion_eventos.h"

#ifndef MAX_CHAR_LINEA
#define MAX_CHAR_LINEA 20
#endif

/**
 * @brief Inicializa y retorna la máquina de estado finito del receptor serie
 * 
 * @return Mef* 
 */
Mef * Receptor_init(void);


#endif
