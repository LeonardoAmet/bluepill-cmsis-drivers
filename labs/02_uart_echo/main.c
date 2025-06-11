#include "stm32f1xx.h"
#include "usart.h"

int main(void) {
    // Asegurarse que el valor del clock es correcto
    SystemCoreClockUpdate();

    // Inicializar USART1 con baudrate 9600
    usart_init(USART1, 9600);

    // Mensaje inicial
    usart_write_string(USART1, "UART Echo ready @ 9600 baud\r\n");

    while (1) {
        // Esperar a recibir un carácter
        char c = usart_read_char(USART1);

        if (c == '\r' || c == '\n') {
            // Si es un retorno de carro o nueva línea, enviar un salto de línea
            usart_write_char(USART1, '\n');
            usart_write_char(USART1, '\r');
        }

        // Enviar el mismo carácter (echo)
        usart_write_char(USART1, c);
    }
}

void _init(void) {}  // Evita conflictos con newlib
