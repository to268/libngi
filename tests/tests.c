/*
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utest.h"
#include "libngi/libngi.h"
#include "libngi/libngi_internal.h"

#define TEST_FILENAME "tests/test.ngi"

UTEST_MAIN();

struct ngi_fixture {
    ngi_header_t* header;
    FILE* file;
};

UTEST_F_SETUP(ngi_fixture) {
    utest_fixture->header = ngi_open(TEST_FILENAME, "r+");
    utest_fixture->file = ngi_get_file(utest_fixture->header);
}

UTEST_F_TEARDOWN(ngi_fixture) { ngi_close(utest_fixture->header); }

/* Strip tests */
UTEST_F(ngi_fixture, strip_section_name) {
    char buffer[NGI_MAX_LINE_LENGTH];

    strcpy(buffer, "test section ->");
    ngi_strip_section_name(buffer);
    ASSERT_STREQ(buffer, "test section");
}

UTEST_F(ngi_fixture, strip_property_name) {
    char buffer[NGI_MAX_LINE_LENGTH];

    strcpy(buffer, "test 1: This is a sample text");
    ngi_strip_property_name(buffer);
    ASSERT_STREQ(buffer, "test 1");
}

UTEST_F(ngi_fixture, strip_property_value) {
    char buffer[NGI_MAX_LINE_LENGTH];

    strcpy(buffer, "test 2: This is a new sample text");
    ngi_strip_property_value(buffer);
    ASSERT_STREQ(buffer, "This is a new sample text");
}

/* Find tests */
UTEST_F(ngi_fixture, find_section) {
    long res = ngi_find_section(utest_fixture->header, "test section");
    ASSERT_GE(res, 0);

    res = ngi_find_section(utest_fixture->header, "test2");
    ASSERT_LT(res, 0);
}

/* TODO: Fix this test
UTEST_F(ngi_fixture, find_property) {
    long res = ngi_find_property(utest_fixture->header, "ngi format test",
"data"); ASSERT_GE(res, 0);

    res = ngi_find_property(utest_fixture->header, "ngi format test", "data");
    ASSERT_LT(res, 0);
}
*/

/* Recache test */
/* TODO: Try to improve this test */
UTEST_F(ngi_fixture, recache) {
    long res = ngi_recache_file(utest_fixture->header);
    ASSERT_TRUE(res);
}

/* Create tests */
UTEST_F(ngi_fixture, create_section) {
    ngi_section_t* section =
        ngi_create_section(utest_fixture->header, "prop_sec");
    ASSERT_STREQ(ngi_get_section_name(section), "prop_sec");

    ngi_section_t* section2 = ngi_get_section(utest_fixture->header, 2);
    ASSERT_TRUE(section == section2);

    long res = ngi_get_sections_number(utest_fixture->header);
    ASSERT_EQ(res, 3);
}

/* TODO: Fix these tests
UTEST_F(ngi_fixture, create_property) {
    ngi_section_t* section = ngi_get_section_by_name(utest_fixture->header,
"prop_sec"); ASSERT_STREQ(ngi_get_section_name(section), "prop_sec");

    ngi_property_t* property = ngi_create_property(utest_fixture->header,
section, "hello", "value"); ASSERT_STREQ(ngi_get_property_name(property),
"hello"); ASSERT_STREQ(ngi_get_property_value(property), "value");

    ngi_property_t* property2 = ngi_get_property(section, 0);
    ASSERT_TRUE(property == property2);

    long res = ngi_get_properties_number(section);
    ASSERT_EQ(res, 1);
}

// Replace tests
UTEST_F(ngi_fixture, replace_section) {
    ngi_section_t* section =  ngi_get_section_by_name(utest_fixture->header,
"prop_sec"); ASSERT_STREQ(ngi_get_section_name(section), "prop_sec");

    ngi_section_replace(utest_fixture->header, section, "new prop");
    ASSERT_STREQ(ngi_get_section_name(section), "new prop");
}

UTEST_F(ngi_fixture, replace_property) {
    ngi_section_t* section =  ngi_get_section_by_name(utest_fixture->header,
"new prop"); ASSERT_STREQ(ngi_get_section_name(section), "new prop");

    ngi_property_t* property = ngi_get_property_by_name(section, "hello");
    ASSERT_STREQ(ngi_get_property_name(property), "hello");

    ngi_property_replace(utest_fixture->header, property, "Hello", "World");
    ASSERT_STREQ(ngi_get_property_name(property), "Hello");
    ASSERT_STREQ(ngi_get_property_value(property), "World");
}
*/
