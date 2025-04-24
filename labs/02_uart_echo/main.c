#define STM32F103xB
#include "usart.h"

int main(void) {
    usart_init(USART1, 9600);
    usart_write_string(USART1, "UART Echo Ready\r\n");
    while (1) {
        char c = usart_read_char(USART1);
        usart_write_char(USART1, c);
        usart_write_char(USART1, 'A');

    }
}

void _init(void) {}