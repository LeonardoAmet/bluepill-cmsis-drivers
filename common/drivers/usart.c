// usart.c
#include "usart.h"

//
// Obtiene la frecuencia del reloj del bus (PCLK) para el USART especificado.
// USART1 cuelga de APB2, mientras que USART2 y USART3 cuelgan de APB1.
//
static uint32_t get_pclk(USART_TypeDef *usartx) {
    uint32_t pclk;
    uint32_t hclk = SystemCoreClock;

    if (usartx == USART1) {
        // Calcular divisor para APB2
        uint32_t ppre2 = (RCC->CFGR >> 11) & 0x7;
        uint32_t divisor = (ppre2 < 4) ? 1 : (1 << (ppre2 - 3));
        pclk = hclk / divisor;
    } else {
        // Calcular divisor para APB1 (USART2/3)
        uint32_t ppre1 = (RCC->CFGR >> 8) & 0x7;
        uint32_t divisor = (ppre1 < 4) ? 1 : (1 << (ppre1 - 3));
        pclk = hclk / divisor;
    }

    return pclk;
}

//
// Configura el registro BRR para establecer la velocidad en baudios.
// Utiliza fracción + mantisa según fórmula de la referencia.
//
static void usart_set_baudrate(USART_TypeDef *usartx, uint32_t pclk, uint32_t baudrate) {
    float usartdiv = (float)pclk / (16 * baudrate);
    uint32_t mantissa = (uint32_t)usartdiv;
    uint32_t fraction = (uint32_t)((usartdiv - mantissa) * 16 + 0.5f); // redondeo

    if (fraction > 15) {
        mantissa += 1;
        fraction = 0;
    }

    usartx->BRR = (mantissa << 4) | (fraction & 0xF);
}

//
// Inicializa el USART y sus pines asociados.
// Soporta USART1 (PA9/PA10), USART2 (PA2/PA3), USART3 (PB10/PB11).
//
void usart_init(USART_TypeDef *usartx, uint32_t baudrate) {
    if (usartx == USART1) {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;

        // PA9 = TX (salida alternate push-pull)
        GPIOA->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9);
        GPIOA->CRH |= (0b10 << GPIO_CRH_MODE9_Pos) | (0b10 << GPIO_CRH_CNF9_Pos);

        // PA10 = RX (entrada flotante)
        GPIOA->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
        GPIOA->CRH |= (0b01 << GPIO_CRH_CNF10_Pos);
    } else if (usartx == USART2) {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

        // PA2 = TX
        GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);
        GPIOA->CRL |= (0b10 << GPIO_CRL_MODE2_Pos) | (0b10 << GPIO_CRL_CNF2_Pos);

        // PA3 = RX
        GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
        GPIOA->CRL |= (0b01 << GPIO_CRL_CNF3_Pos);
    } else if (usartx == USART3) {
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

        // PB10 = TX
        GPIOB->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
        GPIOB->CRH |= (0b10 << GPIO_CRH_MODE10_Pos) | (0b10 << GPIO_CRH_CNF10_Pos);

        // PB11 = RX
        GPIOB->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_CNF11);
        GPIOB->CRH |= (0b01 << GPIO_CRH_CNF11_Pos);
    }

    uint32_t pclk = get_pclk(usartx);
    usart_set_baudrate(usartx, pclk, baudrate);

    // Habilitar transmisor, receptor y el USART
    usartx->CR1 |= USART_CR1_TE | USART_CR1_RE;
    usartx->CR1 |= USART_CR1_UE;
}

//
// Envía un carácter por el USART. Espera a que el buffer esté listo.
//
void usart_write_char(USART_TypeDef *USARTx, char c) {
    while (!(USARTx->SR & USART_SR_TXE));  // Buffer vacío
    USARTx->DR = c;
}

//
// Espera y lee un carácter recibido.
//
char usart_read_char(USART_TypeDef *USARTx) {
    while (!(USARTx->SR & USART_SR_RXNE)); // Dato recibido
    return USARTx->DR;
}

//
// Envía una cadena de texto terminada en null.
//
void usart_write_string(USART_TypeDef *USARTx, const char *str) {
    while (*str) {
        usart_write_char(USARTx, *str++);
    }
}
