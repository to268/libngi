/**
 * @file create.c
 * @brief The libgni creation implementation
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
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * Contents:\n
 * Creation of files/sections/properties
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libngi/libngi.h"
#include "libngi/libngi_internal.h"

int ngi_create(const char* restrict filename);
ngi_section_t* ngi_create_section(ngi_header_t* ngi_header, const char* name);
ngi_property_t* ngi_create_property(ngi_header_t* ngi_header, ngi_section_t* ngi_section,
                                    const char* name, const char* value);

int ngi_create(const char* restrict filename)
{
    if ((access(filename, F_OK)) != 0) {
        return 0;
    }

    FILE* fd = fopen(filename, "w+");

    if (fd == NULL) return 0;
    if ((fclose(fd) != 0)) return 0;

    return 1;
}

ngi_section_t* ngi_create_section(ngi_header_t* ngi_header, const char* name)
{
    if (ngi_header == NULL) return 0;

    FILE* fd = ngi_get_file(ngi_header);

    fseek(fd, 0, SEEK_END);

    /* Write the section in the file */
    ngi_write_section(fd, name);

    /* Add the section in memory */
    return ngi_section_alloc(ngi_header, name);
}

ngi_property_t* ngi_create_property(ngi_header_t* ngi_header, ngi_section_t* ngi_section,
                                    const char* name, const char* value)
{
    if (ngi_header == NULL || ngi_section == NULL) return 0;

    FILE* fd = ngi_get_file(ngi_header);

    fseek(fd, ngi_find_section(ngi_header, ngi_get_section_name(ngi_section)),
            SEEK_SET);

    /* Write the property in the file */
    ngi_write_property(fd, name, value);

    /* Add the property in memory */
    return ngi_property_alloc(ngi_section, NGI_MAX_NAME_LENGTH, NGI_MAX_LINE_LENGTH);
;
}
