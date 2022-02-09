/**
 * @file write.h
 * @brief The libgni write to file header (**internal**)
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
#ifndef WRITE_H
#define WRITE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "libngi_internal.h"

/**
 * @brief Writes the section name in the file
 *
 * @param[in] fd
 * @param[in] name
 *
 * @return NGI_STATUS_FAILED or NGI_STATUS_SUCCESS
 */
int ngi_write_section(FILE* fd, const char* name);

/**
 * @brief Writes the property name and value in the file
 *
 * @param[in] fd
 * @param[in] name
 * @param[in] value
 *
 * @return NGI_STATUS_FAILED or NGI_STATUS_SUCCESS
 */
int ngi_write_property(FILE* fd, const char* name, const char* value);

#ifdef __cplusplus
}
#endif

#endif /* WRITE_H */
