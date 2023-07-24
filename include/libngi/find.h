/**
 * @file find.h
 * @brief The libgni find header (**internal**)
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
#ifndef FIND_H
#define FIND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libngi_internal.h"

/**
 * @brief Finds the ngi_section by name in the file (**internal**)
 *
 * @param[in] ngi_header
 * @param[in] name
 *
 * @return The file offset
 */
long ngi_find_section(ngi_header_t* ngi_header, const char* name);

/**
 * @brief Finds the next ngi_section in the file (**internal**)
 *
 * @param[in] ngi_header
 *
 * @return The file offset
 */
long ngi_find_next_section(ngi_header_t* ngi_header);

/**
 * @brief Finds the ngi_property by name in the file (**internal**)
 *
 * @param[in] ngi_header
 * @param[in] section
 * @param[in] name
 *
 * @return The file offset
 */
long ngi_find_property(ngi_header_t* ngi_header, const char* section,
                       const char* name);

/**
 * @brief Finds the next ngi_property in the file (**internal**)
 *
 * @param[in] ngi_header
 * @param[in] section
 * @param[in] previous_offset
 *
 * @return The file offset
 */
long ngi_find_next_property(ngi_header_t* ngi_header, const char* section,
                            long previous_offset);

#ifdef __cplusplus
}
#endif

#endif /* FIND_H */
