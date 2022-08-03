#include <mm.h>

static uint16_t memory_map[MM_PAGING_PAGES] = { 0, };
static uint32_t page_count = 0;

uint64_t mm_get_free_page()
{
    for(int i=0; i<MM_PAGING_PAGES; i++)
    {
        if(memory_map[i] == 0)
        {
            memory_map[i] = 1;
            page_count ++;

            return MM_MEMORY_LOW + MM_PAGE_SIZE * i;
        }
    }

    return 0;
}

void mm_free_page(uint64_t ptr)
{
    memory_map[(ptr - MM_MEMORY_LOW) / MM_PAGE_SIZE] = 0;
    page_count --;
}

void mm_get_count()
{
    return page_count;
}