#include <drivers/gpio.h>

uint32_t gpio_call(uint32_t pin_number, uint32_t value, uint32_t base, uint32_t field_size, uint32_t field_max)
{
    uint32_t field_mask = (1 << field_size) - 1;

    if(pin_number > field_max) return 0;
    if(value > field_mask) return 0;

    uint32_t num_fields = 32 / field_size;
    uint32_t reg = base + ((pin_number / num_fields) * 4);
    uint32_t shift = (pin_number % num_fields) * field_size;

    uint32_t curval = mmio_read(reg);
    curval &= ~(field_mask << shift);
    curval |= value << shift;
    mmio_write(reg, curval);

    return 1;
}