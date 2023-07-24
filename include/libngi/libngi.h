/**
 * @file libngi.h
 * @brief The libgni core header
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
 * This is the only file to include when using the library
 */
#ifndef LIBNGI_H
#define LIBNGI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "caching.h"
#include "create.h"
#include "replace.h"

/* Version informations */
#define NGI_MAJOR 0
#define NGI_MINOR 9
#define NGI_PATCH 0

/* Build type */
#ifndef NDEBUG
/* Debug build */
#define NGI_BUILD_OPTIONS "debug"
#else
/* Release build */
#define NGI_BUILD_OPTIONS "beta"
#endif /* NDEBUG */

/* Maximum values */
#define NGI_MAX_SECTIONS    8192
#define NGI_MAX_PROPERTIES  8192
#define NGI_MAX_NAME_LENGTH 4096
#define NGI_MAX_LINE_LENGTH 8192

/* Returned status code */
#define NGI_STATUS_FAILED  0
#define NGI_STATUS_SUCCESS 1

typedef struct ngi_header ngi_header_t;
typedef struct ngi_section ngi_section_t;
typedef struct ngi_property ngi_property_t;

/**
 * @brief Opens a file who already exists or not
 *
 * @param[in] filename
 * @param[in] mode
 *
 * @return A new ngi_header
 */
ngi_header_t* ngi_open(const char* filename, const char* mode);

/**
 * @brief Closes a file and frees the ngi_header
 *
 * @param[in] ngi_header
 */
void ngi_close(ngi_header_t* ngi_header);

/**
 * @brief Dumps the tree in memory contents in a file using the ngi syntax
 *
 * @param[in] ngi_header
 * @param[in] fd
 */
void ngi_dump_tree_to_file(ngi_header_t* ngi_header, FILE* fd);

/**
 * @brief Gets the ngi_section at the choosen index
 *
 * @param[in] ngi_header
 * @param[in] section
 *
 * @return The appropriate ngi_section
 */
ngi_section_t* ngi_get_section(const ngi_header_t* ngi_header,
                               const int section);

/**
 * @brief Gets the ngi_section by his name
 *
 * @param[in] ngi_header
 * @param[in] name
 *
 * @return The appropriate ngi_section
 */
ngi_section_t* ngi_get_section_by_name(const ngi_header_t* ngi_header,
                                       const char* name);

/**
 * @brief Gets the ngi_property at the choosen index
 *
 * @param[in] ngi_section
 * @param[in] property
 *
 * @return The appropriate ngi_property
 */
ngi_property_t* ngi_get_property(const ngi_section_t* ngi_section,
                                 const int property);

/**
 * @brief Gets the ngi_property by his name
 *
 * @param[in] ngi_section
 * @param[in] name
 *
 * @return The appropriate ngi_property
 */
ngi_property_t* ngi_get_property_by_name(const ngi_section_t* ngi_section,
                                         const char* name);

/**
 * @brief Gets the ngi_section index
 *
 * @param[in] ngi_header
 * @param[in] ngi_section
 *
 * @return The index of the ngi_section
 */
int ngi_get_section_index(const ngi_header_t* ngi_header,
                          const ngi_section_t* ngi_section);

/**
 * @brief Gets the ngi_section name
 *
 * @param[in] ngi_section
 *
 * @return The name of the ngi_section
 */
char* ngi_get_section_name(const ngi_section_t* ngi_section);

/**
 * @brief Gets the ngi_section name buffer size
 *
 * @param[in] ngi_section
 *
 * @return The name buffer size of the ngi_section
 */
int ngi_get_section_name_size(const ngi_section_t* ngi_section);

/**
 * @brief Gets the ngi_property index
 *
 * @param[in] ngi_section
 * @param[in] ngi_property
 *
 * @return The index of the ngi_property
 */
int ngi_get_property_index(const ngi_section_t* ngi_section,
                           const ngi_property_t* ngi_property);

/**
 * @brief Gets the ngi_sections number
 *
 * @param[in] ngi_header
 *
 * @return The number of ngi_sections
 */
int ngi_get_sections_number(const ngi_header_t* ngi_header);

/**
 * @brief Gets the ngi_properties number
 *
 * @param[in] ngi_section
 *
 * @return The number of ngi_properties
 */
int ngi_get_properties_number(const ngi_section_t* ngi_section);

/**
 * @brief Gets the ngi_property name
 *
 * @param[in] ngi_property
 *
 * @return The name of the ngi_property
 */
char* ngi_get_property_name(const ngi_property_t* ngi_property);

/**
 * @brief Gets the ngi_property name buffer size
 *
 * @param[in] ngi_property
 *
 * @return The name buffer size of the ngi_property
 */
int ngi_get_property_name_size(const ngi_property_t* ngi_property);

/**
 * @brief Gets the ngi_property value
 *
 * @param[in] ngi_property
 *
 * @return The value of the ngi_property
 */
char* ngi_get_property_value(const ngi_property_t* ngi_property);

/**
 * @brief Gets the ngi_property value buffer size
 *
 * @param[in] ngi_property
 *
 * @return The value buffer size of the ngi_property
 */
int ngi_get_property_value_size(const ngi_property_t* ngi_property);

/**
 * @brief Gets the ngi_property parent
 *
 * @param[in] ngi_property
 *
 * @return The ngi_section parent
 */
ngi_section_t* ngi_get_property_parent(const ngi_property_t* ngi_property);

/**
 * @brief Gets the file descriptor of the file
 *
 * @param[in] ngi_header
 *
 * @return The FILE data type for the file descriptor
 */
FILE* ngi_get_file(const ngi_header_t* ngi_header);

#ifdef __cplusplus
}
#endif

#endif /* LIBNGI_H */
