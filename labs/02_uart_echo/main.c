#include "stm32f1xx.h"
#include "usart.h"

int main(void) {
    SystemCoreClockUpdate();  // ¡Refresca el valor con seguridad!

    usart_init(USART1, 9600);
    usart_write_string(USART1, "UART Echo Ready\r\n");

    while (1) {
        char c = 'A';
        usart_write_char(USART1, c);

        for (volatile int i = 0; i < 50000; i++); // fix del loop también
    }
}

void _init(void) {}
