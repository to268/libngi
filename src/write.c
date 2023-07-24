/**
 * @file write.c
 * @brief The libgni write to file implementation
 *
 * @section LICENSE
 *
 * Copyright © 2022 Guillot Tony <tony.guillot@protonmail.com>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * @section DESCRIPTION
 *
 * Contents:\n
 * Gets the type of the line
 * and strips functions
 */
#include <stdio.h>
#include <string.h>
#include "libngi/libngi.h"
#include "libngi/libngi_internal.h"

int ngi_write_section(FILE* fd, const char* name);
int ngi_write_property(FILE* fd, const char* name, const char* value);

int ngi_write_section(FILE* fd, const char* name) {
    if (ftell(fd) != 0)
        if ((fwrite("\n", 1, 1, fd)) <= 0)
            return 0;

    if ((fwrite(name, strlen(name), 1, fd)) <= 0)
        return 0;
    if ((fwrite(SECTION_TKN, strlen(SECTION_TKN), 1, fd)) <= 0)
        return 0;
    if ((fwrite("\n", 1, 1, fd)) <= 0)
        return 0;

    return 1;
}

int ngi_write_property(FILE* fd, const char* name, const char* value) {
    if ((fwrite(name, strlen(name), 1, fd)) <= 0)
        return 0;
    if ((fwrite(PROPERTY_TKN, strlen(PROPERTY_TKN), 1, fd)) <= 0)
        return 0;
    if ((fwrite(value, strlen(value), 1, fd)) <= 0)
        return 0;
    if ((fwrite("\n", 1, 1, fd)) <= 0)
        return 0;

    return 1;
}
