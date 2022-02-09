/**
 * @file create.h
 * @brief The libgni creation header
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
 */
#ifndef CREATE_H
#define CREATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libngi.h"

/**
 * @brief Creates a new ngi_section
 *
 * @param[in] ngi_header
 * @param[in] name
 *
 * @return The newly created ngi_section
 */
ngi_section_t* ngi_create_section(ngi_header_t* ngi_header, const char* name);

/**
 * @brief Creates a new ngi_property
 *
 * @param[in] ngi_header
 * @param[in] ngi_section
 * @param[in] name
 * @param[in] value
 *
 * @return The newly created ngi_property
 */
ngi_property_t* ngi_create_property(ngi_header_t* ngi_header, ngi_section_t* ngi_section,
                                    const char* name, const char* value);

#ifdef __cplusplus
}
#endif

#endif /* CREATE_H */
