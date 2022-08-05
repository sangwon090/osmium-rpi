#include <drivers/fb.h>
#include <drivers/mailbox.h>

static uint32_t pitch;
uint8_t *buffer;

void fb_init(uint32_t width, uint32_t height)
{
    mailbox[0] = 4 * 35;        // size: 35
    mailbox[1] = 0;             // code

    mailbox[2] = 0x00048003;    // set physical width/height
    mailbox[3] = 8;             // size
    mailbox[4] = 0;             // code
    mailbox[5] = width;         // width
    mailbox[6] = height;        // height

    mailbox[7] = 0x00048004;    // set virtual width/height
    mailbox[8] = 8;             // size
    mailbox[9] = 8;             // code
    mailbox[10] = width;        // width
    mailbox[11] = height;       // height

    mailbox[12] = 0x00048009;   // set virtual offset
    mailbox[13] = 8;            // size
    mailbox[14] = 8;            // code
    mailbox[15] = 0;            // x
    mailbox[16] = 0;            // y

    mailbox[17] = 0x00048005;   // set depth
    mailbox[18] = 4;            // size
    mailbox[19] = 4;            // code
    mailbox[20] = 32;           // depth

    mailbox[21] = 0x00048006;   // set pixel order
    mailbox[22] = 4;            // size
    mailbox[23] = 4;            // code
    mailbox[24] = 1;            // RGB

    mailbox[25] = 0x00040001;   // allocate buffer
    mailbox[26] = 8;            // size
    mailbox[27] = 8;            // code
    mailbox[28] = 4096;         // buffer base address
    mailbox[29] = 0;            // buffer size

    mailbox[30] = 0x00040008;   // get pitch
    mailbox[31] = 4;            // size
    mailbox[32] = 4;            // code
    mailbox[33] = 0;            // bytes per line

    mailbox[34] = 0;            // end

    if(mailbox_call() && mailbox[20] == 32 && mailbox[28] != 0)
    {
        mailbox[28] &= 0x3FFFFFFF;
        pitch = mailbox[33];
        buffer = (uint32_t*)((long) mailbox[28]);
    }
}

inline void fb_draw_pixel(uint32_t x, uint32_t y, uint32_t color)
{
    int32_t offset = (x * 4) + (y * pitch);

    buffer[offset] = color;
    *((uint32_t*)(buffer + offset)) = color;
}

inline void fb_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color)
{
    for(uint32_t i=0; i<width; i++)
    {
        for(uint32_t j=0; j<height; j++)
        {
            fb_draw_pixel(x + i, y + j, color);
        }
    }
}