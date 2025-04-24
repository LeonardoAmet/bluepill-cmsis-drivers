#define STM32F103xB
#include "stm32f1xx.h"

volatile int counter = 0;

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;          // Limpiar bandera
        GPIOC->ODR ^= (1 << 13);          // Toggle PC13
        counter++;
    }
}

void timer2_init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC   = 7200 - 1;     // 72MHz / 7200 = 10kHz
    TIM2->ARR   = 10000 - 1;    // 10kHz / 10k = 1 Hz
    TIM2->EGR   = TIM_EGR_UG;   // Actualización inmediata
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1  |= TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM2_IRQn);
}

int main(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;
    AFIO->MAPR   |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    GPIOC->CRH   &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
    GPIOC->CRH   |= GPIO_CRH_MODE13_0;

    timer2_init();
    __enable_irq();

    while (1) {
        // main loop vacío, interrupción hace todo
    }
}

void _init(void) {}
