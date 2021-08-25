/*
 * Copyright © 2021 Guillot Tony <tony.guillot@protonmail.com>
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
 */
/**
 * @file replace.c
 * @brief The libgni replace implementation
 * @copyright This project is released under the GNU General Public License 3.0 or later
 *
 * Contents:\n
 * Replace functions for a section/property
 */
#include <stdio.h>
#include "libngi/libngi.h"
#include "libngi/replace.h"
#include "libngi/libngi_internal.h"

void ngi_section_replace(ngi_header_t* ngi_header, ngi_section_t* ngi_section,
                            const char* new_name);
void ngi_property_replace(ngi_header_t* ngi_header, ngi_property_t* ngi_property,
                            const char* new_name, const char* new_value);

void ngi_section_replace(ngi_header_t* ngi_header, ngi_section_t* ngi_section,
                            const char* new_name)
{
    /* Get the file descriptor */
    FILE* fd = ngi_get_file(ngi_header);

    /* Change the name of the section in memory */
    ngi_set_section_name(ngi_section, new_name);

    /*
     *  Change the name of the section in the file
     *  by overwriting the whole file
     */
    rewind(fd);
    ngi_dump_tree_to_file(ngi_header, fd);
}

void ngi_property_replace(ngi_header_t* ngi_header, ngi_property_t* ngi_property,
                            const char* new_name, const char* new_value)
{
    /* Get the file descriptor */
    FILE* fd = ngi_get_file(ngi_header);

    /* Change the name of the property in memory */
    ngi_set_property_name(ngi_property, new_name);
    ngi_set_property_value(ngi_property, new_value);

    /*
     *  Change the name of the property in the file
     *  by overwriting the whole file
     */
    rewind(fd);
    ngi_dump_tree_to_file(ngi_header, fd);
}
