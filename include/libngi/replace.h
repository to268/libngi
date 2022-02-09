/**
 * @file replace.h
 * @brief The libgni replace header
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
#ifndef REPLACE_H
#define REPLACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libngi.h"

/**
 * @brief Replaces the section name
 *
 * @param[in] ngi_header
 * @param[in] ngi_section
 * @param[in] new_name
 */
void ngi_section_replace(ngi_header_t* ngi_header, ngi_section_t* ngi_section,
                            const char* new_name);

/**
 * @brief Replaces the proprety name and/or value
 *
 * Pass NULL if you don't want to replace the name or the value
 *
 * @param[in] ngi_header
 * @param[in] ngi_property
 * @param[in] new_name
 * @param[in] new_value
 */
void ngi_property_replace(ngi_header_t* ngi_header, ngi_property_t* ngi_property,
                            const char* new_name, const char* new_value);

#ifdef __cplusplus
}
#endif

#endif /* REPLACE_H */
