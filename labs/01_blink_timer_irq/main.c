#include "stm32f1xx.h"

// Variable global que cuenta cuántas veces se dispara la interrupción
volatile int counter = 0;

// --------------------------------------------------
// Rutina de interrupción del Timer 2
// Se ejecuta cada vez que el timer llega al ARR (1 Hz)
// --------------------------------------------------
void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;          // Limpiar la bandera de actualización (UIF)
        GPIOC->ODR ^= (1 << 13);          // Hacer toggle en el pin PC13 (LED on/off)
        counter++;                        // Incrementar contador
    }
}

// --------------------------------------------------
// Inicializa TIM2 para generar una interrupción cada 1 segundo
// --------------------------------------------------
void timer2_init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   // Habilitar reloj para TIM2

    TIM2->PSC = 7200 - 1;     // Prescaler: 72 MHz / 7200 = 10 kHz
    TIM2->ARR = 10000 - 1;    // Auto-reload: 10 kHz / 10000 = 1 Hz
    TIM2->EGR = TIM_EGR_UG;   // Forzar actualización del contador
    TIM2->DIER |= TIM_DIER_UIE; // Habilitar interrupción por update
    TIM2->CR1  |= TIM_CR1_CEN;  // Habilitar el contador

    NVIC_EnableIRQ(TIM2_IRQn); // Habilitar interrupción en el NVIC
}

// --------------------------------------------------
// Función principal
// --------------------------------------------------
int main(void) {
    SystemCoreClockUpdate(); // Actualizar SystemCoreClock según configuración de PLL

    // Habilitar reloj para GPIOC y AFIO
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;

    // Deshabilitar JTAG para liberar PC13
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    // Configurar PC13 como salida push-pull a 10 MHz
    GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;

    // Inicializar el timer y habilitar interrupciones globales
    timer2_init();
    __enable_irq(); // Habilita interrupciones globales (cpsie i)

    while (1) {
        // Bucle principal vacío. Todo lo hace la interrupción.
    }
}

// Función dummy para evitar conflictos con newlib
void _init(void) {}
