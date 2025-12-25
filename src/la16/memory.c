/*
 * MIT License
 *
 * Copyright (c) 2024 cr4zyengineer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <la16/memory.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

la16_memory_t *la16_memory_alloc(la16_memory_size_t size)
{
    la16_memory_t *memory = malloc(sizeof(la16_memory_t));
    memory->page_cnt = (size / LA16_MEMORY_PAGE_SIZE);
    memory->memory_size = memory->page_cnt * LA16_MEMORY_PAGE_SIZE;
    memory->memory = calloc(sizeof(unsigned char), memory->memory_size);
    return memory;
}

void la16_memory_dealloc(la16_memory_t *memory)
{
    free(memory->memory);
    free(memory);
}

unsigned char la16_memory_load_image(la16_memory_t *memory,
                                     const char *image_path)
{
    /* open boot image */
    int fd = open(image_path, O_RDONLY);

    /* checking file descriptor */
    if(fd == -1)
    {
        return 0;
    }

    /* gather size of boot image */
    struct stat image_stat;
    fstat(fd, &image_stat);
    size_t image_size = image_stat.st_size;

    /* checking if memory is big enough for our memory */
    if(image_size > memory->memory_size)
    {
        printf("[bios] error: boot image is too large\n");
        return 0;
    }

    /* loading boot image into memory */
    read(fd, memory->memory, image_size);

    printf("[bios] loaded boot image: %zu bytes\n", image_size);

    close(fd);

    return 1;
}
