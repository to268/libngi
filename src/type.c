/**
 * @file type.c
 * @brief The libgni typing implementation
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
 * Gets the type of the line
 * and strips functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libngi/libngi.h"
#include "libngi/libngi_internal.h"

enum ngi_type ngi_get_type(const char* buff);
void ngi_strip_section_name(char* buff);
void ngi_strip_property_name(char* buff);
void ngi_strip_property_value(char* buff);

/* Return the type of the line */
enum ngi_type ngi_get_type(const char* buff)
{
    if (strstr(buff, SECTION_TKN) > 0) {
        return SECTION;
    } else if (strstr(buff, PROPERTY_TKN) > 0) {
        return PROPERTY;
    }
    return UNKNOWN;
}

void ngi_strip_section_name(char* buff)
{
    /* Store the pattern to apply */
    char pattern[MAX_PATTERN_LENGTH] = SSCANF_PATTERN;

    /* Extract the string before the token */
    sscanf(buff, strcat(pattern, SECTION_TKN), buff);

    /* Remove the trailing space */
    buff[strlen(buff) - 1] = '\0';
}

void ngi_strip_property_name(char* buff)
{
    /* Store the pattern to apply */
    char pattern[MAX_PATTERN_LENGTH] = SSCANF_PATTERN;

    /* Extract the string before the token */
    sscanf(buff, strcat(pattern, PROPERTY_TKN), buff);
}

void ngi_strip_property_value(char* buff)
{
    /* Remove the part starting at the beginning of the line to the end of the token */
    char* tkn = strstr(buff, PROPERTY_TKN);
    tkn += strlen(PROPERTY_TKN);

    strcpy(buff, tkn);

    /* Remove the new line */
    char* ptr = buff;
    strtok_r(buff, "\n", &ptr);
}
