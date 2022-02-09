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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <criterion/criterion.h>
#include "libngi/libngi.h"
#include "libngi/libngi_internal.h"

#define TEST_FILENAME "tests/test.ngi"

ngi_header_t* ngi_header = NULL;
FILE* fd = NULL;

void test_init(void)
{
    ngi_header = ngi_open(TEST_FILENAME, "r+");
    fd = ngi_get_file(ngi_header);
}

void test_fini(void)
{
    ngi_close(ngi_header);
}

TestSuite(libngi_suite, .init=test_init, .fini=test_fini);

Test(libngi_suite, base)
{
    /* Strip fuctions */
    char test[NGI_MAX_LINE_LENGTH];

    /* Strip section name */
    strcpy(test, "test section ->");
    ngi_strip_section_name(test);
    cr_expect(strcmp(test, "test section") == 0,
        "ngi_strip_section_name() needs to strip the section token");

    /* Strip property name */
    strcpy(test, "test 1: This is a sample text");
    ngi_strip_property_name(test);
    cr_expect(strcmp(test, "test 1") == 0,
        "ngi_strip_property_name() needs to strip the property name");

    /* Strip property value */
    strcpy(test, "test 2: This is a new sample text");
    ngi_strip_property_value(test);
    cr_expect(strcmp(test, "This is a new sample text") == 0,
        "ngi_strip_property_name() needs to strip the property name");


    /* Find functions */
    long res = ngi_find_section(ngi_header, "test section");
    cr_expect(res >= 0, "ngi_find_section() needs to find a section");
    res = ngi_find_section(ngi_header, "test2");
    cr_expect(res < 0, "ngi_find_section() needs to find nothing");


    test_fini();
    test_init();


    /* Recache */
    res = ngi_recache_file(ngi_header);
    cr_expect(res == 1, "ngi_recache_file() needs to recache the file");

    test_fini();
    test_init();


    /* Create */
    ngi_section_t* section =  ngi_create_section(ngi_header, "prop_sec");
    cr_expect(section != NULL, "ngi_create_section() needs to create a section");

    ngi_section_t* section2 = ngi_get_section(ngi_header, 2);
    cr_expect(section == section2, "ngi_get_section() needs to get the section");

    res = ngi_get_sections_number(ngi_header);
    cr_expect(res == 3, "ngi_get_sections_number() needs to find 3 sections");

    ngi_property_t* property = ngi_create_property(ngi_header, section, "hello", "value");
    cr_expect(property != NULL, "ngi_create_property() needs to create a property");

    ngi_property_t* property2 = ngi_get_property(section, 0);
    cr_expect(property == property2, "ngi_get_property() needs to get the property");

    res = ngi_get_properties_number(section);
    cr_expect(res == 1, "ngi_get_properties_number() needs to find 1 section");


    /* Replace */
    ngi_section_replace(ngi_header, section, "new prop");
    ngi_property_replace(ngi_header, property, "Hello", "World");
}
