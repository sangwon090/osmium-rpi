#include <types.h>
#include <drivers/gpio.h>

#ifndef UART_H
#define UART_H

void uart_init();
int8_t uart_read();
void uart_write(int8_t c);
void uart_print(int8_t *str);
void uart_printf(int8_t *str, ...);

#endif