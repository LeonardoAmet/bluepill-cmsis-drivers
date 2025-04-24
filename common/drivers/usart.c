// usart.c
#include "usart.h"

void usart_init(USART_TypeDef *USARTx, uint32_t baudrate) {
    // Solo soportamos USART1 por ahora
    if (USARTx != USART1) return;

    // Habilitar clocks
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;   // Funciones alternativas
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // USART1

    // Configurar PA9 como Alternate Function Push-Pull (TX)
    GPIOA->CRH &= ~(0xF << 4);         // Limpio los bits de PA9
    GPIOA->CRH |=  (0xB << 4);         // 0b1011: Output 50 MHz, AF Push-Pull

    // Configurar PA10 como entrada flotante (RX)
    GPIOA->CRH &= ~(0xF << 8);         // Limpio los bits de PA10
    GPIOA->CRH |=  (0x4 << 8);         // 0b0100: Input floating

    // Configurar baudrate
    // USARTDIV = Fclk / (16 * baudrate)
    // Con Fclk = 72 MHz
    //uint32_t usartdiv = 72000000 / (16 * baudrate);
    //USARTx->BRR = usartdiv;
    // USARTDIV = Fclk / baudrate
    USARTx->BRR = (SystemCoreClock) / (16*baudrate);


    // Habilitar USART, TX y RX
    USARTx->CR1 |= USART_CR1_UE;  // USART Enable
    USARTx->CR1 |= USART_CR1_TE;  // Transmit Enable
    USARTx->CR1 |= USART_CR1_RE;  // Receive Enable
}

void usart_write_char(USART_TypeDef *USARTx, char c) {
    while (!(USARTx->SR & USART_SR_TXE));  // Esperar que el buffer esté vacío
    USARTx->DR = c;
}

char usart_read_char(USART_TypeDef *USARTx) {
    while (!(USARTx->SR & USART_SR_RXNE)); // Esperar que llegue un dato
    return USARTx->DR;
}

void usart_write_string(USART_TypeDef *USARTx, const char *str) {
    while (*str) {
        usart_write_char(USARTx, *str++);
    }
}
