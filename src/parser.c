/**
 * @file parser.c
 * @brief The libgni parsing implementation
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
 * Parsing functions to parse sections and properties
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libngi/libngi.h"
#include "libngi/libngi_internal.h"

inline int ngi_parse_file(ngi_header_t* ngi_header);
static int ngi_parse_sections(ngi_header_t* ngi_header);
static ngi_section_t* ngi_parse_section(ngi_header_t* ngi_header, char* buff);
static int ngi_parse_properties(ngi_header_t* ngi_header, ngi_section_t* ngi_section);
static ngi_property_t* ngi_parse_property(ngi_section_t* ngi_section, char* buff);

inline int ngi_parse_file(ngi_header_t* ngi_header)
{
    return ngi_parse_sections(ngi_header);
}

/**
 * @brief Parses all the sections (**private**)
 *
 * @param[in] ngi_header
 *
 * @return NGI_STATUS_FAILED or NGI_STATUS_SUCCESS
 */
static int ngi_parse_sections(ngi_header_t* ngi_header)
{
    FILE* fd = ngi_get_file(ngi_header);
    if (fd == NULL) return 0;

    char buff[NGI_MAX_LINE_LENGTH];
    long offset = ngi_find_next_section(ngi_header);

    /* Loop on all sections */
    while (offset != -1) {
        fseek(fd, offset, SEEK_SET);
        fgets(buff, NGI_MAX_LINE_LENGTH, fd);

        ngi_section_t* ngi_section = ngi_parse_section(ngi_header, buff);

        if (ngi_section == NULL)
            return 0;

        /* Get next section offset */
        offset = ngi_find_next_section(ngi_header);
    }

#ifndef NDEBUG
        ngi_print_map(ngi_header);
#endif

    return 1;
}

/**
 * @brief Parses a section (**private**)
 *
 * @param[in] ngi_header
 * @param[in] buff
 *
 * @return The parsed ngi_section
 */
static ngi_section_t* ngi_parse_section(ngi_header_t* ngi_header, char* buff)
{
    /* Strip the token section name */
    ngi_strip_section_name(buff);
    ngi_section_t* ngi_section = ngi_section_alloc(ngi_header, buff);

    if (ngi_section == NULL)
        return NULL;


    /* Parse all the properties of the section */
    if(!ngi_parse_properties(ngi_header, ngi_section)) {
        /* Free the section if we have encounter an error */
        ngi_section_free(ngi_header, ngi_section);
        return NULL;
    }

    return ngi_section;
}


/**
 * @brief Parses all the properties (**private**)
 *
 * @param[in] ngi_header
 * @param[in] ngi_section
 *
 * @return NGI_STATUS_FAILED or NGI_STATUS_SUCCESS
 */
static int ngi_parse_properties(ngi_header_t* ngi_header, ngi_section_t* ngi_section)
{
    FILE* fd = ngi_get_file(ngi_header);
    char buff[NGI_MAX_LINE_LENGTH];
    char* section_name = ngi_get_section_name(ngi_section);
    const long saved_offset = ftell(fd);
    long offset = ngi_find_next_property(ngi_header, section_name, 0);

    /* Loop on all sections */
    while (offset != -1) {
        fseek(fd, offset, SEEK_SET);
        fgets(buff, NGI_MAX_LINE_LENGTH, fd);

        ngi_property_t* ngi_property = ngi_parse_property(ngi_section, buff);

        if (ngi_property == NULL)
            return 0;

        /* Get next section offset */
        offset = ngi_find_next_property(ngi_header, section_name, offset);
    }

    /* Go to the saved offset */
    fseek(fd, saved_offset, SEEK_SET);
    return 1;
}

/**
 * @brief Parses a property (**private**)
 *
 * @param[in] ngi_section
 * @param[in] buff
 *
 * @return The parsed ngi_property
 */
static ngi_property_t* ngi_parse_property(ngi_section_t* ngi_section, char* buff)
{
    char* name = buff;
    char value[NGI_MAX_LINE_LENGTH];

    /* Make a copy of the line to the value buffer */
    strncpy(value, name, NGI_MAX_LINE_LENGTH);

    /* Strip the name and the value */
    ngi_strip_property_name(name);
    ngi_strip_property_value(value);

    /* Allocate a new property and add the name and the value */
    ngi_property_t* ngi_property = ngi_property_alloc(ngi_section, strlen(name), strlen(value));
    ngi_set_property_name(ngi_property, name);
    ngi_set_property_value(ngi_property, value);

    if (ngi_property == NULL)
        return NULL;

    return ngi_property;
}
