#include "serie.h"
#include "gpio.h"
#include <stm32f1xx.h>
#include <stdbool.h>
#include <ctype.h>


int Serie_init(int baudrate){
    int error = -1;
    SystemCoreClockUpdate();

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;    
    Pin_configuraSalida(PA9,PUSH_PULL | FUNCION_ALTERNATIVA,V_BAJA);
    Pin_configuraEntrada(PA10,PULL_UP);

    const uint32_t divisor = SystemCoreClock/baudrate;

    if (divisor > 0 && divisor < 0x10000){
        USART1->BRR = (SystemCoreClock + (baudrate+1)/2) / baudrate;
        error = 0;
    }else if(divisor){
        USART1->BRR = 0xFFFF;
    }else{
        USART1->BRR = 1;
    }
    USART1->CR2 = 0;
    USART1->CR3 = 0;
    USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
    return error;
}

bool Serie_intentaEnviarCaracter(char c)
{
    if(!(USART1->SR & USART_SR_TXE)) return false;
    USART1->DR = c;
    return true;
}

bool Serie_intentaRecibirCaracter(char *c)
{
    if(!(USART1->SR & USART_SR_RXNE)) return false;
    *c = (char) USART1->DR;
    return true;
}

void Serie_flush(void){
    while(!(USART1->SR & USART_SR_TC));
}


void Serie_deinit(void){
    Serie_flush();
    USART1->CR1 = 0;
    Pin_configuraEntrada(PA9,FLOTANTE);
    Pin_configuraEntrada(PA10,FLOTANTE);
}