// usart.c
#include "usart.h"

static uint32_t get_pclk(USART_TypeDef *usartx) {
    uint32_t pclk;

    uint32_t hclk = SystemCoreClock;

    if (usartx == USART1) {
        // PCLK2
        uint32_t ppre2 = (RCC->CFGR >> 11) & 0x7;
        uint32_t divisor = (ppre2 < 4) ? 1 : (1 << ((ppre2 - 3)));
        pclk = hclk / divisor;
    } else {
        // USART2 y USART3 usan PCLK1
        uint32_t ppre1 = (RCC->CFGR >> 8) & 0x7;
        uint32_t divisor = (ppre1 < 4) ? 1 : (1 << ((ppre1 - 3)));
        pclk = hclk / divisor;
    }

    return pclk;
}

static void usart_set_baudrate(USART_TypeDef *usartx, uint32_t pclk, uint32_t baudrate) {
    float usartdiv = (float)pclk / (16 * baudrate);
    uint32_t mantissa = (uint32_t)usartdiv;
    uint32_t fraction = (uint32_t)((usartdiv - mantissa) * 16 + 0.5f);

    if (fraction > 15) {
        mantissa += 1;
        fraction = 0;
    }

    usartx->BRR = (mantissa << 4) | (fraction & 0xF);
}

void usart_init(USART_TypeDef *usartx, uint32_t baudrate) {
    if (usartx == USART1) {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;

        // Configurar PA9 (TX) y PA10 (RX)
        GPIOA->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9);
        GPIOA->CRH |= (0b10 << GPIO_CRH_MODE9_Pos) | (0b10 << GPIO_CRH_CNF9_Pos);
        GPIOA->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
        GPIOA->CRH |= (0b01 << GPIO_CRH_CNF10_Pos);
    } else if (usartx == USART2) {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

        GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);
        GPIOA->CRL |= (0b10 << GPIO_CRL_MODE2_Pos) | (0b10 << GPIO_CRL_CNF2_Pos);
        GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
        GPIOA->CRL |= (0b01 << GPIO_CRL_CNF3_Pos);
    } else if (usartx == USART3) {
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

        GPIOB->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
        GPIOB->CRH |= (0b10 << GPIO_CRH_MODE10_Pos) | (0b10 << GPIO_CRH_CNF10_Pos);
        GPIOB->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_CNF11);
        GPIOB->CRH |= (0b01 << GPIO_CRH_CNF11_Pos);
    }

    uint32_t pclk = get_pclk(usartx);
    usart_set_baudrate(usartx, pclk, baudrate);

    usartx->CR1 |= USART_CR1_TE | USART_CR1_RE;
    usartx->CR1 |= USART_CR1_UE;
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
