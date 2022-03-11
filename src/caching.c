/**
 * @file caching.c
 * @brief The libgni caching implementation
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
 * Caching of the whole file
 * and a recache mechanism
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libngi/libngi.h"
#include "libngi/libngi_internal.h"

int ngi_cache_file(ngi_header_t* ngi_header);
int ngi_recache_file(ngi_header_t* ngi_header);

/* Recache sub functions */
static inline int
create_section_if_needeed(ngi_header_t* ngi_header,
                        ngi_section_t* ngi_section,
                        uint processed_sections, uint processed_properties,
                        uint max_sections, char* buff);
static inline void
remove_unused_sections(ngi_header_t* ngi_header,
                    uint processed_sections, uint max_sections);

static inline ngi_property_t*
create_property_if_needeed(ngi_header_t* ngi_header,
                        ngi_section_t* ngi_section,
                        uint processed_properties, int max_properties,
                        const char* name, const char* value);

static inline int
remove_unused_properties(ngi_header_t* ngi_header,
                        ngi_section_t* ngi_section,
                        uint processed_properties, int max_properties);

inline int ngi_cache_file(ngi_header_t* ngi_header)
{
    int res = ngi_parse_file(ngi_header);
    return res;
}

int ngi_recache_file(ngi_header_t* ngi_header)
{
    FILE* fd = ngi_get_file(ngi_header);
    char buff[NGI_MAX_LINE_LENGTH];

    /* Store the current location on the tree */
    uint processed_sections = 0;
    uint processed_properties = 0;

    /* Store the maximum number of expected nodes on the tree */
    uint max_sections = ngi_get_sections_number(ngi_header);
    /* The value is changed for each sections */
    uint max_properties = 0;

    /* Store the current section and the current property */
    ngi_section_t*  current_section = NULL;
    ngi_property_t* current_property = NULL;

    /* Go to the beginning of the file to cache the whole file */
    rewind(fd);
    while (!feof(fd)) {
        fgets(buff, NGI_MAX_LINE_LENGTH, fd);

        if (ngi_get_type(buff) == SECTION) {
            ngi_strip_section_name(buff);

            /* Remove old properties */
            if (remove_unused_properties(ngi_header, current_section,
                                         processed_properties, max_properties))
            {
                continue;
            }

            /* Check if we need to create a new section */
            if (create_section_if_needeed(ngi_header, current_section,
                                        processed_sections, processed_properties,
                                        max_sections, buff))
            {
                continue;
            }

            /* Core of the loop for a section */

            /* Reset properties count */
            processed_properties = 0;

            /* Get the next section to process */
            current_section = ngi_get_section(ngi_header, processed_sections);

            /* Store the max properties count */
            max_properties = ngi_get_properties_number(current_section);

            /* Check if the name has been modified */
            if (strcmp(ngi_get_section_name(current_section), buff))
                ngi_set_section_name(current_section, buff);

            /* We have processed a section, ready to process his properties */
            processed_sections++;

        } else if (ngi_get_type(buff) == PROPERTY) {
            char* name = buff;
            char value[NGI_MAX_LINE_LENGTH];

            /* Strip the freshly acquired name and value */
            strcpy(value, name);
            ngi_strip_property_name(name);
            ngi_strip_property_value(value);

            current_property = create_property_if_needeed(ngi_header, current_section,
                                                        processed_properties, max_properties,
                                                        name, value);
            if (current_property == NULL)
                /* Get the next property to process */
                current_property = ngi_get_property(current_section, processed_properties);
            else
                continue;

            /* Core of the loop for a property */

            /* Check if the name has been modified */
            if (strcmp(ngi_get_property_name(current_property), name))
                ngi_set_property_name(current_property, name);

            /* Check if the value has been modified */
            if (strcmp(ngi_get_property_value(current_property), value))
                ngi_set_property_value(current_property, value);

            /* We have processed a property */
            processed_properties++;
        }

    }

    /* Check if we need to remove unused sections */
    remove_unused_sections(ngi_header, processed_sections, max_sections);

    return 1;
}

/**
 * @brief Creates a new ngi_section if needed (**private**)
 *
 * @param[in] ngi_header
 * @param[out] ngi_section
 * @param processed
 * @param max
 * @param[in] buff
 *
 * @return NGI_STATUS_FAILED or NGI_STATUS_SUCCESS
 */
static inline int
create_section_if_needeed(ngi_header_t* ngi_header,
                        ngi_section_t* ngi_section,
                        uint processed_sections, uint processed_properties,
                        uint max_sections, char* buff)
{
    /* Check if we need to create a new section */
    if (processed_sections >= max_sections) {
        ngi_section = ngi_create_section(ngi_header, buff);
        processed_sections++;

        /* Reset properties count */
        processed_properties = 0;

        /* Return 1 to trigger the continue keyword */
        return 1;
    }

    /* Return 0 to do the rest of the loop */
    return 0;
}

/**
 * @brief Removes all unused sections/removed sections (**private**)
 *
 * @param[in] ngi_header
 * @param[in] processed
 * @param[in] max
 */
static inline void
remove_unused_sections(ngi_header_t* ngi_header,
                    uint processed_sections, uint max_sections)
{
    if (processed_sections < max_sections) {
        for (int i = processed_sections; i < max_sections; i++)
            ngi_section_free(ngi_header, ngi_get_section(ngi_header, i));
    }
}

/**
 * @brief Creates a new ngi_property if needed (**private**)
 *
 * @param[in] ngi_header
 * @param[in] ngi_section
 * @param processed
 * @param[in] max
 * @param[in] name
 * @param[in] value
 *
 * @return The newly created ngi_property
 */
static inline ngi_property_t*
create_property_if_needeed(ngi_header_t* ngi_header,
                        ngi_section_t* ngi_section,
                        uint processed_properties, int max_properties,
                        const char* name, const char* value)

{

    if (processed_properties >= max_properties) {
        ngi_property_t* ngi_property = ngi_create_property(ngi_header, ngi_section, name, value);
        processed_properties++;

        /* Return the property to trigger the continue keyword */
        return ngi_property;
    }

    /* Return 0 to do the rest of the loop */
    return NULL;
}


/**
 * @brief Removes all unused properties/removed properties (**private**)
 *
 * @param[in] ngi_header
 * @param[in] ngi_section
 * @param processed
 * @param[in] max
 *
 * @return NGI_STATUS_FAILED or NGI_STATUS_SUCCESS
 */
static inline int
remove_unused_properties(ngi_header_t* ngi_header,
                        ngi_section_t* ngi_section,
                        uint processed_properties, int max_properties)
{
    if (processed_properties < max_properties) {
        for (int i = processed_properties; i < max_properties; i++)
            ngi_property_free(ngi_section, ngi_get_property(ngi_section, i));

        /* Reset properties count */
        processed_properties = 0;

        /* Return 1 to trigger the continue keyword */
        return 1;
    }

    /* Return 0 to do the rest of the loop */
    return 0;
}
