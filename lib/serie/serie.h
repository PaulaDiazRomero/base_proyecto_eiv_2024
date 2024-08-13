#ifndef SERIE_H
#define SERIE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Inicializa puerto serie 8-n-1 con el baudrate especificado
 * 
 * @param baudrate 
 * @return int 0 si tuvo exito, -1 si el baudrate no es soportado
 */
int Serie_init(int baudrate);

/**
 * @brief Bloquea hasta completar toda transmisión pendiente
 * 
 */
void Serie_flush(void);

/**
 * @brief Apaga puerto serie
 * 
 */
void Serie_deinit(void);


/**
 * @brief Recibe un caracter si está disponible, falla si no hay caracter disponible (NO BLOQUEA)
 * 
 * @param c Referencia a la variable donde guardar el caracter recibido
 * @return true Caracter recibido
 * @return false No hay caracter disponible
 */
bool Serie_intentaRecibirCaracter(char *c);

/**
 * @brief Envía un caracter si el puerto está disponible, falla si está ocupado (NO BLOQUEA)
 * 
 * @param c El caracter
 * @return true Caracter enviado
 * @return false Puerto ocupado
 */
bool Serie_intentaEnviarCaracter(char c);
#endif
