/**
 * @file libngi_internal.h
 * @brief The libgni internal header (**internal**)
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
#ifndef LIBNGI_INTERNAL_H
#define LIBNGI_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "libngi.h"
#include "find.h"
#include "parser.h"
#include "tokens.h"
#include "type.h"
#include "write.h"

#define SSCANF_PATTERN     "%[a-zA-Z0-9_ ]"
#define MAX_PATTERN_LENGTH 30

/* Core */

/**
 * @brief Allocates a new ngi_section (**internal**)
 *
 * @param[in] ngi_header
 * @param[in] name
 *
 * @return The allocated ngi_section
 */
ngi_section_t* ngi_section_alloc(ngi_header_t* ngi_header, const char* name);

/**
 * @brief Allocates a new ngi_property (**internal**)
 *
 * @param[in] ngi_section
 * @param[in] name_size
 * @param[in] value_size
 *
 * @return The allocated ngi_property
 */
ngi_property_t* ngi_property_alloc(ngi_section_t* ngi_section, int name_size,
                                   int value_size);

/**
 * @brief Reallocates a ngi_section (**internal**)
 *
 * @param[in] ngi_section
 * @param[in] new_name_size
 *
 * @return NGI_STATUS_FAILED or NGI_STATUS_SUCCESS
 */
int ngi_section_realloc(ngi_section_t* ngi_section, int new_name_size);

/**
 * @brief Reallocates a ngi_property (**internal**)
 *
 * @param[in] ngi_property
 * @param[in] new_name_size
 * @param[in] new_value_size
 *
 * @return NGI_STATUS_FAILED or NGI_STATUS_SUCCESS
 */
int ngi_property_realloc(ngi_property_t* ngi_property, int new_name_size,
                         int new_value_size);

/**
 * @brief Frees a ngi_section (**internal**)
 *
 * @param[in] ngi_header
 * @param[in] ngi_section
 */
void ngi_section_free(ngi_header_t* ngi_header, ngi_section_t* ngi_section);

/**
 * @brief Frees a ngi_property (**internal**)
 *
 * @param[in] ngi_section
 * @param[in] ngi_property
 */
void ngi_property_free(ngi_section_t* ngi_section,
                       ngi_property_t* ngi_property);

/**
 * @brief Sets the ngi_section name (**internal**)
 *
 * @param[in] ngi_section
 * @param[in] name
 */
void ngi_set_section_name(ngi_section_t* ngi_section, const char* name);

/**
 * @brief Sets the ngi_property name (**internal**)
 *
 * @param[in] ngi_property
 * @param[in] name
 */
void ngi_set_property_name(ngi_property_t* ngi_property, const char* name);

/**
 * @brief Sets the ngi_property value (**internal**)
 *
 * @param[in] ngi_property
 * @param[in] value
 */
void ngi_set_property_value(ngi_property_t* ngi_property, const char* value);

#ifndef NDEBUG
/**
 * @brief Print the tree map (**debug build only**)
 *
 * @param[in] ngi_header
 */
void ngi_print_map(const ngi_header_t* ngi_header);
#endif /* NDEBUG */

/* Caching */

/**
 * @brief Caches the file for the first time (**internal**)
 *
 * @param[in] ngi_header
 *
 * @return NGI_STATUS_FAILED or NGI_STATUS_SUCCESS
 */
int ngi_cache_file(ngi_header_t* ngi_header);

/* Create */

/**
 * @brief Creates a new empty file (**internal**)
 *
 * @param[in] filename
 *
 * @return NGI_STATUS_FAILED or NGI_STATUS_SUCCESS
 */
int ngi_create(const char* restrict filename);

/* Type */

/**
 * @brief Strips the section token (**internal**)
 *
 * @param buff
 */
void ngi_strip_section_name(char* buff);

/**
 * @brief Strips the property name (**internal**)
 *
 * @param buff
 */
void ngi_strip_property_name(char* buff);

/**
 * @brief Strips the property value (**internal**)
 *
 * @param buff
 */
void ngi_strip_property_value(char* buff);

#ifdef __cplusplus
}
#endif

#endif /* LIBNGI_INTERNAL_H */
