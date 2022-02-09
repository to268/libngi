/**
 * @file caching.h
 * @brief The libgni caching header
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
#ifndef CACHING_H
#define CACHING_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ngi_header ngi_header_t;
typedef struct ngi_section ngi_section_t;
typedef struct ngi_property ngi_property_t;

/**
 * @brief Recaches the file when manual changes are made
 *
 * @param[in] ngi_header
 *
 * @return NGI_STATUS_FAILED or NGI_STATUS_SUCCESS
 */
int ngi_recache_file(ngi_header_t* ngi_header);

#ifdef __cplusplus
}
#endif

#endif /* CACHING_H */
