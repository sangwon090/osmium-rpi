#include "gpio.h"

#ifndef UART_H
#define UART_H

void uart_init();
char uart_read();
void uart_write(char c);
void uart_print(char *str);
void uart_hex(unsigned int hex);

#endif