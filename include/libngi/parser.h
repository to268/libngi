/**
 * @file parser.h
 * @brief The libgni parsing header (**internal**)
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
#ifndef PARSER_H
#define PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libngi_internal.h"

/**
 * @brief Parses all the file contents (**internal**)
 *
 * @param[in] ngi_header
 *
 * @return NGI_STATUS_FAILED or NGI_STATUS_SUCCESS
 */
int ngi_parse_file(ngi_header_t* ngi_header);

#ifdef __cplusplus
}
#endif

#endif /* PARSER_H */
