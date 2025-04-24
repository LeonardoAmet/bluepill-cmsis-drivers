// usart.h
#ifndef USART_H
#define USART_H
#include "stm32f1xx.h"  // Incluye los typedef como USART_TypeDef
#include <stdint.h>

void usart_init(USART_TypeDef *USARTx, uint32_t baudrate);
void usart_write_string(USART_TypeDef *USARTx, const char *str);
void usart_write_char(USART_TypeDef *USARTx, char c);
char usart_read_char(USART_TypeDef *USARTx);

#endif
