/**
 * @file libngi.c
 * @brief The libgni core implementation
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
 * Contents:\n
 * Tree with dynamic allocation
 * and the basic API calls
 *
 * Brief tags used in the library:\n
 * (**internal**):  only available in the library\n
 * (**private**):   only available in the current file\n
 * (*):             specify when the symbol is available\n
 * *nothing*:       available everywhere (API interface, exported symbol)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libngi/libngi.h"
#include "libngi/libngi_internal.h"

/**
 * @brief Contains the data of a property
 *
 * The ngi_property contains:
 * - the name of the property
 * - the value of the property
 * - the size of the name buffer
 * - the size of the value buffer
 * - a pointer to the parent ngi_section of the property
 */
typedef struct ngi_property {
    char* name;
    char* value;
    int name_size;
    int value_size;
    ngi_section_t* parent;
} ngi_property_t;

/**
 * @brief Contains the data of a section
 *
 * The ngi_section contains:
 * - the name of the section
 * - the size of the name buffer
 * - an array of pointers pointing a ngi_property
 * - the current length of the properties array
 */
typedef struct ngi_section {
    char* name;
    int name_size;
    ngi_property_t* properties[NGI_MAX_PROPERTIES];
    int properties_len;
} ngi_section_t;

/**
 * @brief Contains the file data
 *
 * The ngi_header contains:
 * - an array of pointers pointing a ngi_section
 * - the current length of the sections array
 * - the file descriptor as a FILE*
 */
typedef struct ngi_header {
    ngi_section_t* sections[NGI_MAX_SECTIONS];
    int sections_len;
    FILE* fd;
} ngi_header_t;

/* Private methods */
void ngi_header_free(ngi_header_t* ngi_header);
static ngi_header_t* ngi_header_alloc(void);
static void ngi_balance_sections(ngi_header_t* ngi_header);
static void ngi_balance_properties(ngi_section_t* ngi_section);
static void ngi_sections_free(ngi_header_t* ngi_header);
static void ngi_properties_free(ngi_section_t* ngi_section);

ngi_header_t* ngi_open(const char* restrict filename, const char* mode)
{
    FILE* fd = NULL;

    /* Check if the file exists */
    if ((access(filename, F_OK)) == 0) {
        fd = fopen(filename, mode);

        /* Check if an error as occured when opening the file */
        if (fd == NULL) return NULL;

    } else {
        ngi_create(filename);
        fd = fopen(filename, mode);

        /* Check if an error as occured when opening the file */
        if (fd == NULL) return NULL;
    }

    /* Allocate the header */
    ngi_header_t* ngi_header = ngi_header_alloc();

    /* Add the file pointer */
    ngi_header->fd = fd;

    /* Cache the file */
    ngi_cache_file(ngi_header);

    return ngi_header;
}

void ngi_close(ngi_header_t* ngi_header)
{
    ngi_header_free(ngi_header);
}

void ngi_dump_tree_to_file(ngi_header_t* ngi_header, FILE* fd)
{
    /* Dump all the tree in memory in a file */
    for (int i = 0; i < ngi_header->sections_len; i++) {
        ngi_section_t* ngi_section = ngi_header->sections[i];
        ngi_write_section(fd, ngi_section->name);

        for (int j = 0; j < ngi_section->properties_len; j++) {
            ngi_property_t* ngi_property = ngi_section->properties[j];
            ngi_write_property(fd, ngi_property->name, ngi_property->value);
        }
    }
}

/* Getters */

ngi_section_t* ngi_get_section(const ngi_header_t* ngi_header, const int section_num)
{
    /* Check if the index is valid */
    if (section_num > ngi_header->sections_len)
        return NULL;

    return ngi_header->sections[section_num];
}

ngi_section_t* ngi_get_section_by_name(const ngi_header_t* ngi_header,
                                    const char* name)
{
    ngi_section_t* ngi_section = NULL;

    for (int i = 0; i < ngi_header->sections_len; i++) {
        ngi_section = ngi_header->sections[i];

        /* Check if the section is NULL */
        if (ngi_section == NULL)
            return NULL;

        if (!strcmp(ngi_section->name, name)) {
            return ngi_section;
        }

    }

    return NULL;
}

ngi_property_t* ngi_get_property(const ngi_section_t* ngi_section, const int property_num)
{
    /* Check if the index is valid */
    if (property_num > ngi_section->properties_len)
        return NULL;

    return ngi_section->properties[property_num];
}

ngi_property_t* ngi_get_property_by_name(const ngi_section_t* ngi_section,
                                        const char* name)
{

    ngi_property_t* ngi_property = NULL;

    for (int i = 0; i < ngi_section->properties_len; i++) {
        ngi_property = ngi_section->properties[i];

        /* Check if the section is NULL */
        if (ngi_property == NULL)
            return NULL;

        if (!strcmp(ngi_property->name, name)) {
            return ngi_property;
        }

    }

    return NULL;
}

int ngi_get_section_index(const ngi_header_t* ngi_header,
                        const ngi_section_t* ngi_section)
{
    for (int i = 0; i < ngi_header->sections_len; i++) {
        if (ngi_header->sections[i] == ngi_section) {
            return i;
        }
    }

    return -1;
}

char* ngi_get_section_name(const ngi_section_t* ngi_section)
{
    return ngi_section->name;
}

int ngi_get_section_name_size(const ngi_section_t* ngi_section)
{
    return ngi_section->name_size;
}

int ngi_get_property_index(const ngi_section_t* ngi_section,
                        const ngi_property_t* ngi_property)
{
    for (int i = 0; i < ngi_section->properties_len; i++) {
        if (ngi_section->properties[i] == ngi_property) {
            return i;
        }
    }

    return -1;
}

int ngi_get_sections_number(const ngi_header_t* ngi_header)
{
    return ngi_header->sections_len;
}

int ngi_get_properties_number(const ngi_section_t* ngi_section)
{
    return ngi_section->properties_len;
}

char* ngi_get_property_name(const ngi_property_t* ngi_property)
{
    return ngi_property->name;
}

int ngi_get_property_name_size(const ngi_property_t* ngi_property)
{
    return ngi_property->name_size;
}

char* ngi_get_property_value(const ngi_property_t* ngi_property)
{
    return ngi_property->value;
}

int ngi_get_property_value_size(const ngi_property_t* ngi_property)
{
    return ngi_property->value_size;
}

ngi_section_t* ngi_get_property_parent(const ngi_property_t* ngi_property)
{
    return ngi_property->parent;
}

FILE* ngi_get_file(const ngi_header_t* ngi_header)
{
    return ngi_header->fd;
}

/* Setters */

void ngi_set_section_name(ngi_section_t* ngi_section, const char* name)
{
    if (name == NULL)
        return;

    size_t new_name_size = strlen(name);

    /* Check if the new name can fit in the section buffer */
    if (new_name_size <= ngi_section->name_size) {
        strncpy(ngi_section->name, name, ngi_section->name_size);
    } else {
        /* Realloc the name buffer */
        ngi_section_realloc(ngi_section, new_name_size);
    }

}

void ngi_set_property_name(ngi_property_t* ngi_property, const char* name)
{
    if (name == NULL)
        return;

    size_t new_name_size = strlen(name);

    /* Check if the new name can fit in the property buffer */
    if (new_name_size > ngi_property->name_size) {
        /* Realloc the name buffer */
        ngi_property_realloc(ngi_property, new_name_size, 0);
    }

    /* Copy the new name */
    strcpy(ngi_property->name, name);
}

void ngi_set_property_value(ngi_property_t* ngi_property, const char* value)
{
    if (value == NULL)
        return;

    size_t new_value_size = strlen(value);

    /* Check if the new value can fit in the property buffer */
    if (new_value_size > ngi_property->value_size) {
        /* Realloc the value buffer */
        ngi_property_realloc(ngi_property, 0, new_value_size);
    }

    /* Copy the new name */
    strcpy(ngi_property->value, value);
}


/**
 * @brief Allocates a new ngi_header (**private**)
 *
 * @return The allocated ngi_header
 */
static ngi_header_t* ngi_header_alloc(void)
{
    /* Allocate the header */
    ngi_header_t* ngi_header = malloc(sizeof(ngi_header_t));

    if (ngi_header == NULL)
        return NULL;

    /* Initialize the header */
    ngi_header->sections_len = 0;
    ngi_header->fd = NULL;

    return ngi_header;
}

/* Allocate a section */
ngi_section_t* ngi_section_alloc(ngi_header_t* ngi_header, const char* name)
{
    ngi_section_t* ngi_section = malloc(sizeof(ngi_section_t));

    if (ngi_section == NULL)
        return NULL;

    /* Initialize the section */
    ngi_section->properties_len = 0;

    /* Add the section */
    ngi_header->sections[ngi_header->sections_len] = ngi_section;
    ngi_header->sections_len++;

    /* Add the name */
    ngi_section->name_size = strlen(name);
    ngi_section->name = malloc(sizeof(char) * ngi_section->name_size);

    if (ngi_section->name == NULL)
        return NULL;

    strcpy(ngi_section->name, name);

    return ngi_section;
}

/* Allocate a property */
ngi_property_t* ngi_property_alloc(ngi_section_t* ngi_section, int name_size, int value_size)
{
    ngi_property_t* ngi_property = malloc(sizeof(ngi_property_t));

    if (ngi_property == NULL)
        return NULL;

    /* Initialize the section */
    ngi_property->name = malloc(name_size);
    ngi_property->value = malloc(value_size);
    ngi_property->name_size = name_size;
    ngi_property->value_size = value_size;

    if (ngi_property->name == NULL || ngi_property->value == NULL)
        return NULL;

    /* Set the parent as the section pointer */
    ngi_property->parent = ngi_section;

    /* Add the property */
    ngi_section->properties[ngi_section->properties_len] = ngi_property;
    ngi_section->properties_len++;

    return ngi_property;
}

/* Realloc the section name */
int ngi_section_realloc(ngi_section_t* ngi_section, int new_name_size)
{
    /* Check if the value is above zero */
    if (new_name_size > 0) {
        ngi_section->name = realloc(ngi_section->name, new_name_size);
        ngi_section->name_size = new_name_size;
    }

    /* Check for allocation errors */
    if (ngi_section->name == NULL)
        return 0;

    return 1;
}

/* Realloc a property */
int ngi_property_realloc(ngi_property_t* ngi_property, int new_name_size,
                            int new_value_size)
{
    /* Check if the value is above zero or it's the actual size */
    if (new_name_size > 0 ||  ngi_property->name_size == new_name_size ) {
        ngi_property->name = realloc(ngi_property->name, new_name_size);
        ngi_property->name_size = new_name_size;
    }

    /* Check if the value is above zero or it's the actual size */
    if (new_value_size > 0 || ngi_property->value_size == new_value_size ) {
        ngi_property->value = realloc(ngi_property->value, new_name_size);
        ngi_property->value_size = new_value_size;
    }

    /* Check for allocation errors */
    if (ngi_property->name == NULL || ngi_property->value == NULL)
        return 0;

    return 1;
}

/**
 * @brief Frees a ngi_header (**private**)
 *
 * @param[in] ngi_header
 */
void ngi_header_free(ngi_header_t* ngi_header)
{
    /* Check for childs */
    if (ngi_header->sections_len != 0) {
        ngi_sections_free(ngi_header);
        fclose(ngi_header->fd);
        free(ngi_header);
    } else {
        fclose(ngi_header->fd);
        free(ngi_header);
    }
}

void ngi_section_free(ngi_header_t* ngi_header, ngi_section_t* ngi_section)
{
    /* Check if the property is NULL */
    if (ngi_section == NULL) return;

    /* Check if the pointer is the last one */
    if (ngi_header->sections[ngi_header->sections_len] == ngi_section) {

        if (ngi_section->properties_len != 0)
            ngi_properties_free(ngi_section);

        free(ngi_section->name);
        free(ngi_section);

        /* Ensure to remove the pointer on the array */
        ngi_header->sections[ngi_header->sections_len] = NULL;

        ngi_header->sections_len--;
        return;
    }

    for (int i = 0; i < ngi_header->sections_len; i++) {
        if (ngi_header->sections[i] == ngi_section) {

            if (ngi_section->properties_len != 0)
                ngi_properties_free(ngi_section);

            free(ngi_section->name);
            free(ngi_section);

            /* Ensure to remove the pointer on the array */
            ngi_header->sections[i] = NULL;

            ngi_balance_sections(ngi_header);
            return;
        }
    }
}

void ngi_property_free(ngi_section_t* ngi_section, ngi_property_t* ngi_property)
{
    /* Check if the property is NULL */
    if (ngi_property == NULL) return;

    /* Check if the pointer is the last one */
    if (ngi_section->properties[ngi_section->properties_len] == ngi_property) {
        free(ngi_property->value);
        free(ngi_property->name);
        free(ngi_property);

        /* Ensure to remove the pointer on the array */
        ngi_section->properties[ngi_section->properties_len] = NULL;

        ngi_section->properties_len--;
        return;
    }

    for (int i = 0; i < ngi_section->properties_len; i++) {
        if (ngi_section->properties[i] == ngi_property) {
            free(ngi_property->value);
            free(ngi_property->name);
            free(ngi_property);

            /* Ensure to remove the pointer on the array */
            ngi_section->properties[i] = NULL;

            ngi_balance_properties(ngi_section);
            return;
        }
    }
}

/**
 * @brief Frees all the ngi_sections (**private**)
 *
 * @param[in] ngi_header
 */
static void ngi_sections_free(ngi_header_t* ngi_header)
{
    for (int i = 0; i < ngi_header->sections_len; i++) {
        ngi_section_t* ngi_section = ngi_header->sections[i];

        if (ngi_section == NULL)
            break;

        /* Check for childs */
        if (ngi_section->properties_len != 0)
            ngi_properties_free(ngi_section);

        /* Free the name buffer and the section itself */
        free(ngi_section->name);
        free(ngi_section);

        /* Ensure to remove the pointer on the array */
        ngi_header->sections[i] = NULL;
    }

    ngi_header->sections_len = 0;
}

/**
 * @brief Frees all the ngi_properties (**private**)
 *
 * @param[in] ngi_section
 */
static void ngi_properties_free(ngi_section_t* ngi_section)
{
    for (int i = 0; i < ngi_section->properties_len; i++) {
        ngi_property_t* ngi_property = ngi_section->properties[i];

        if (ngi_property == NULL)
            break;

        if (ngi_section != NULL) {
            /* Free the name and value buffer and the property itself */
            free(ngi_property->name);
            free(ngi_property->value);
            free(ngi_property);

            /* Ensure to remove the pointer on the array */
            ngi_section->properties[i] = NULL;
        }
    }

    ngi_section->properties_len = 0;
}

/**
 * @brief Balances all the ngi_sections (**private**)
 *
 * Removes the ngi_sections pointers pointing to NULL in the sections array
 *
 * @param[in] ngi_header
 */
static void ngi_balance_sections(ngi_header_t* ngi_header)
{
    /* Alloc a new list */
    ngi_section_t* new_sections[ngi_header->sections_len];
    for (int i = 0; i < ngi_header->sections_len; i++) {
        if (ngi_header->sections[i] != NULL) {
            new_sections[i] = ngi_header->sections[i];
        }
    }

    ngi_header->sections_len--;

    /* Copy the list */
    for (int i = 0; i < ngi_header->sections_len; i++)
        ngi_header->sections[i] = new_sections[i];
}

/**
 * @brief Balances all the ngi_properties (**private**)
 *
 * Removes the ngi_properties pointers pointing to NULL in the properties array
 *
 * @param[in] ngi_section
 */
static void ngi_balance_properties(ngi_section_t* ngi_section)
{
    /* Alloc a new list */
    ngi_property_t* new_properties[ngi_section->properties_len];
    for (int i = 0; i < ngi_section->properties_len; i++) {
        if (ngi_section->properties[i] != NULL) {
            new_properties[i] = ngi_section->properties[i];
        }
    }

    ngi_section->properties_len--;

    /* Copy the list */
    for (int i = 0; i < ngi_section->properties_len; i++)
        ngi_section->properties[i] = new_properties[i];
}

#ifndef NDEBUG
void ngi_print_map(const ngi_header_t* ngi_header)
{
    printf("Header with %d sections\n", ngi_header->sections_len);

    for (int s = 0; s < ngi_header->sections_len; s++) {
        ngi_section_t* ngi_section = ngi_header->sections[s];
        printf("├── ");
        printf("Section \"%s\" with %d propreties\n", ngi_section->name, ngi_section->properties_len);

        for (int p = 0; p < ngi_section->properties_len; p++) {
            ngi_property_t* ngi_proprety = ngi_section->properties[p];
            printf("│   ├── ");
            printf("%s -> %s\n", ngi_proprety->name, ngi_proprety->value);
        }
    }
    printf("\n");
}
#endif
