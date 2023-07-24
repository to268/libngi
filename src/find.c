/**
 * @file find.c
 * @brief The libgni find implementation
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
 * finds a section or a property by name in the file
 * and the next section/property in the file
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libngi/libngi.h"
#include "libngi/libngi_internal.h"

long ngi_find_section(ngi_header_t* ngi_header, const char* name);
long ngi_find_next_section(ngi_header_t* ngi_header);
long ngi_find_property(ngi_header_t* ngi_header, const char* section,
                       const char* name);
long ngi_find_next_property(ngi_header_t* ngi_header, const char* section,
                            long previous_offset);

long ngi_find_section(ngi_header_t* ngi_header, const char* name) {
    FILE* fd = ngi_get_file(ngi_header);
    if (fd == NULL)
        return -1;

    char buff[NGI_MAX_LINE_LENGTH];
    long previous_line_offset = 0;

    rewind(fd);
    while (!feof(fd)) {
        fgets(buff, NGI_MAX_LINE_LENGTH, fd);

        /* Check if the line is a section
         * and if the name is the requested one
         */
        if (ngi_get_type(buff) == SECTION) {
            ngi_strip_section_name(buff);

            /* Return the previous line offset */
            if (!strcmp(buff, name))
                return previous_line_offset;
        }

        previous_line_offset = ftell(fd);
    }
    return -1;
}

long ngi_find_next_section(ngi_header_t* ngi_header) {
    FILE* fd = ngi_get_file(ngi_header);
    if (fd == NULL)
        return -1;

    char buff[NGI_MAX_LINE_LENGTH];
    long previous_line_offset = 0;

    while (!feof(fd)) {
        fgets(buff, NGI_MAX_LINE_LENGTH, fd);

        /* Return the previous line offset */
        if (ngi_get_type(buff) == SECTION)
            return previous_line_offset;

        previous_line_offset = ftell(fd);
    }
    return -1;
}

long ngi_find_property(ngi_header_t* ngi_header, const char* section,
                       const char* name) {
    FILE* fd = ngi_get_file(ngi_header);
    if (fd == NULL)
        return -1;

    long offset = ngi_find_section(ngi_header, section);
    long previous_line_offset = 0;

    if (offset < 0)
        return -1;

    /* Make sure to be at the start of the section */
    fseek(fd, offset, SEEK_SET);

    char buff[NGI_MAX_LINE_LENGTH];

    rewind(fd);
    while (!feof(fd)) {
        fgets(buff, NGI_MAX_LINE_LENGTH, fd);

        /* Check if the line is a section
         * and if the name is the requested one
         */
        if (ngi_get_type(buff) == PROPERTY) {
            ngi_strip_property_name(buff);

            /* Return the previous line offset */
            if (!strcmp(buff, name))
                return previous_line_offset;
        }

        /* Check if we are in a new section */
        if (ngi_get_type(buff) == SECTION)
            return -1;

        previous_line_offset = ftell(fd);
    }
    return -1;
}

long ngi_find_next_property(ngi_header_t* ngi_header, const char* section,
                            long previous_offset) {
    FILE* fd = ngi_get_file(ngi_header);
    if (fd == NULL)
        return -1;

    /* Check if a valid offset is provided */
    long offset = previous_offset <= 0 ? ngi_find_section(ngi_header, section)
                                       : previous_offset;

    if (offset < 0)
        return -1;

    /* Make sure to be at the start of the section */
    fseek(fd, offset, SEEK_SET);

    char buff[NGI_MAX_LINE_LENGTH];

    /* Skip the first line */
    fgets(buff, NGI_MAX_LINE_LENGTH, fd);
    /* Clear the buffer */
    memset(buff, 0, NGI_MAX_LINE_LENGTH);
    long previous_line_offset = ftell(fd);

    while (!feof(fd)) {
        fgets(buff, NGI_MAX_LINE_LENGTH, fd);

        /* Return the previous line offset */
        if (ngi_get_type(buff) == PROPERTY)
            return previous_line_offset;

        /* Check if we are in a new section */
        if (ngi_get_type(buff) == SECTION)
            return -1;

        previous_line_offset = ftell(fd);
    }
    return -1;
}
