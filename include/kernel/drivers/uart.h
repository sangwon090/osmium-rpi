#include <types.h>
#include <drivers/gpio.h>

#ifndef UART_H
#define UART_H

void uart_init();
uint8_t uart_read();
void uart_write(uint8_t c);
void uart_print(uint8_t *str);
void uart_printf(uint8_t *str, ...);

#endif