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
 * @file type.h
 * @brief The libgni typing header
 * @copyright This project is released under the GNU General Public License 3.0 or later
 */
#ifndef TYPE_H
#define TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libngi.h"

/**
 * @brief Gets the type of the line, see ngi_type
 *
 * @param[in] buff
 *
 * @return The detected ngi_type
 *
 * Returned value:\n
 * SECTION: the line is a section\n
 * PROPRETY: the line is a property\n
 * UNKNOWN: the line is unknown
 */
enum ngi_type ngi_get_type(const char* buff);

enum ngi_type {
    SECTION,
    PROPERTY,
    UNKNOWN,
};

#ifdef __cplusplus
}
#endif

#endif /* TYPE_H */
