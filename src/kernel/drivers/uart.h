#include "gpio.h"

#ifndef UART_H
#define UART_H

void uart_init();
void uart_write(char c);
void uart_print(char *str);

#endif