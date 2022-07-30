#include <types.h>
#include <drivers/mmio.h>

#ifndef GPIO_H
#define GPIO_H

#define GPIO_BASE   PERIPHERAL_BASE + 0x00200000

#define GPFSEL0     GPIO_BASE + 0x00
#define GPFSEL1     GPIO_BASE + 0x04
#define GPFSEL2     GPIO_BASE + 0x08
#define GPFSEL3     GPIO_BASE + 0x0C
#define GPFSEL4     GPIO_BASE + 0x10
#define GPFSEL5     GPIO_BASE + 0x14
#define GPSET0      GPIO_BASE + 0x1C
#define GPSET1      GPIO_BASE + 0x20
#define GPCLR0      GPIO_BASE + 0x28
#define GPCLR1      GPIO_BASE + 0x2C
#define GPLEV0      GPIO_BASE + 0x34
#define GPLEV1      GPIO_BASE + 0x38
#define GPEDS0      GPIO_BASE + 0x40
#define GPEDS1      GPIO_BASE + 0x44
#define GPREN0      GPIO_BASE + 0x4C
#define GPREN1      GPIO_BASE + 0x50
#define GPFEN0      GPIO_BASE + 0x58
#define GPFEN1      GPIO_BASE + 0x5C
#define GPHEN0      GPIO_BASE + 0x64
#define GPHEN1      GPIO_BASE + 0x68
#define GPLEN0      GPIO_BASE + 0x70
#define GPLEN1      GPIO_BASE + 0x74
#define GPAREN0     GPIO_BASE + 0x7C
#define GPAREN1     GPIO_BASE + 0x80
#define GPAFEN0     GPIO_BASE + 0x88
#define GPAFEN1     GPIO_BASE + 0x8C
#define GPPUPPDN0   GPIO_BASE + 0xE4
#define GPPUPPDN1   GPIO_BASE + 0xE8
#define GPPUPPDN2   GPIO_BASE + 0xEC
#define GPPUPPDN3   GPIO_BASE + 0xF0

#define GPIO_MAX_PIN        53
#define GPIO_FUNCTION_ALT5  2

uint32_t gpio_call(uint32_t pin_number, uint32_t value, uint32_t base, uint32_t field_size, uint32_t field_max);

inline uint32_t gpio_set(uint32_t pin_number, uint32_t value)
{
    return gpio_call(pin_number, value, GPSET0, 1, GPIO_MAX_PIN);
}

inline uint32_t gpio_clear(uint32_t pin_number, uint32_t value)
{
    return gpio_call(pin_number, value, GPCLR0, 1, GPIO_MAX_PIN);
}

inline uint32_t gpio_pull(uint32_t pin_number, uint32_t value)
{
    return gpio_call(pin_number, value, GPPUPPDN0, 2, GPIO_MAX_PIN);
}

inline uint32_t gpio_function(uint32_t pin_number, uint32_t value)
{
    return gpio_call(pin_number, value, GPFSEL0, 3, GPIO_MAX_PIN);
}

#endif