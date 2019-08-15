/*
 * blogc: A blog compiler.
 * Copyright (C) 2014-2019 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the BSD License.
 * See the file LICENSE.
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include "../../src/common/error.h"
#include "../../src/common/utils.h"
#include "../../src/blogc/source-parser.h"


static void
test_source_parse(void **state)
{
    const char *a =
        "VAR1: asd asd\n"
        "VAR2: 123chunda\n"
        "----------\n"
        "# This is a test\n"
        "\n"
        "bola\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(err);
    assert_non_null(source);
    assert_int_equal(bc_trie_size(source), 7);
    assert_string_equal(bc_trie_lookup(source, "VAR1"), "asd asd");
    assert_string_equal(bc_trie_lookup(source, "VAR2"), "123chunda");
    assert_string_equal(bc_trie_lookup(source, "EXCERPT"),
        "<h1 id=\"this-is-a-test\">This is a test</h1>\n"
        "<p>bola</p>\n");
    assert_string_equal(bc_trie_lookup(source, "CONTENT"),
        "<h1 id=\"this-is-a-test\">This is a test</h1>\n"
        "<p>bola</p>\n");
    assert_string_equal(bc_trie_lookup(source, "RAW_CONTENT"),
        "# This is a test\n"
        "\n"
        "bola\n");
    assert_string_equal(bc_trie_lookup(source, "FIRST_HEADER"), "This is a test");
    assert_string_equal(bc_trie_lookup(source, "DESCRIPTION"), "bola");
    bc_trie_free(source);
}


static void
test_source_parse_crlf(void **state)
{
    const char *a =
        "VAR1: asd asd\r\n"
        "VAR2: 123chunda\r\n"
        "----------\r\n"
        "# This is a test\r\n"
        "\r\n"
        "bola\r\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(err);
    assert_non_null(source);
    assert_int_equal(bc_trie_size(source), 7);
    assert_string_equal(bc_trie_lookup(source, "VAR1"), "asd asd");
    assert_string_equal(bc_trie_lookup(source, "VAR2"), "123chunda");
    assert_string_equal(bc_trie_lookup(source, "EXCERPT"),
        "<h1 id=\"this-is-a-test\">This is a test</h1>\r\n"
        "<p>bola</p>\r\n");
    assert_string_equal(bc_trie_lookup(source, "CONTENT"),
        "<h1 id=\"this-is-a-test\">This is a test</h1>\r\n"
        "<p>bola</p>\r\n");
    assert_string_equal(bc_trie_lookup(source, "RAW_CONTENT"),
        "# This is a test\r\n"
        "\r\n"
        "bola\r\n");
    assert_string_equal(bc_trie_lookup(source, "FIRST_HEADER"), "This is a test");
    assert_string_equal(bc_trie_lookup(source, "DESCRIPTION"), "bola");
    bc_trie_free(source);
}


static void
test_source_parse_with_spaces(void **state)
{
    const char *a =
        "\n  \n"
        "VAR1: chunda     \t   \n"
        "\n\n"
        "BOLA: guda\n"
        "----------\n"
        "# This is a test\n"
        "\n"
        "bola\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(err);
    assert_non_null(source);
    assert_int_equal(bc_trie_size(source), 7);
    assert_string_equal(bc_trie_lookup(source, "VAR1"), "chunda");
    assert_string_equal(bc_trie_lookup(source, "BOLA"), "guda");
    assert_string_equal(bc_trie_lookup(source, "EXCERPT"),
        "<h1 id=\"this-is-a-test\">This is a test</h1>\n"
        "<p>bola</p>\n");
    assert_string_equal(bc_trie_lookup(source, "CONTENT"),
        "<h1 id=\"this-is-a-test\">This is a test</h1>\n"
        "<p>bola</p>\n");
    assert_string_equal(bc_trie_lookup(source, "RAW_CONTENT"),
        "# This is a test\n"
        "\n"
        "bola\n");
    assert_string_equal(bc_trie_lookup(source, "FIRST_HEADER"), "This is a test");
    assert_string_equal(bc_trie_lookup(source, "DESCRIPTION"), "bola");
    bc_trie_free(source);
}


static void
test_source_parse_with_excerpt(void **state)
{
    const char *a =
        "VAR1: asd asd\n"
        "VAR2: 123chunda\n"
        "----------\n"
        "# This is a test\n"
        "\n"
        "bola\n"
        "\n"
        "...\n"
        "\n"
        "guda\n"
        "yay";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(err);
    assert_non_null(source);
    assert_int_equal(bc_trie_size(source), 7);
    assert_string_equal(bc_trie_lookup(source, "VAR1"), "asd asd");
    assert_string_equal(bc_trie_lookup(source, "VAR2"), "123chunda");
    assert_string_equal(bc_trie_lookup(source, "EXCERPT"),
        "<h1 id=\"this-is-a-test\">This is a test</h1>\n"
        "<p>bola</p>\n");
    assert_string_equal(bc_trie_lookup(source, "CONTENT"),
        "<h1 id=\"this-is-a-test\">This is a test</h1>\n"
        "<p>bola</p>\n"
        "<p>guda\n"
        "yay</p>\n");
    assert_string_equal(bc_trie_lookup(source, "RAW_CONTENT"),
        "# This is a test\n"
        "\n"
        "bola\n"
        "\n"
        "...\n"
        "\n"
        "guda\n"
        "yay");
    assert_string_equal(bc_trie_lookup(source, "FIRST_HEADER"), "This is a test");
    assert_string_equal(bc_trie_lookup(source, "DESCRIPTION"), "bola");
    bc_trie_free(source);
}


static void
test_source_parse_with_first_header(void **state)
{
    const char *a =
        "VAR1: asd asd\n"
        "VAR2: 123chunda\n"
        "FIRST_HEADER: THIS IS CHUNDA!\n"
        "----------\n"
        "# This is a test\n"
        "\n"
        "bola\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(err);
    assert_non_null(source);
    assert_int_equal(bc_trie_size(source), 7);
    assert_string_equal(bc_trie_lookup(source, "VAR1"), "asd asd");
    assert_string_equal(bc_trie_lookup(source, "VAR2"), "123chunda");
    assert_string_equal(bc_trie_lookup(source, "EXCERPT"),
        "<h1 id=\"this-is-a-test\">This is a test</h1>\n"
        "<p>bola</p>\n");
    assert_string_equal(bc_trie_lookup(source, "CONTENT"),
        "<h1 id=\"this-is-a-test\">This is a test</h1>\n"
        "<p>bola</p>\n");
    assert_string_equal(bc_trie_lookup(source, "RAW_CONTENT"),
        "# This is a test\n"
        "\n"
        "bola\n");
    assert_string_equal(bc_trie_lookup(source, "FIRST_HEADER"), "THIS IS CHUNDA!");
    assert_string_equal(bc_trie_lookup(source, "DESCRIPTION"), "bola");
    bc_trie_free(source);
}


static void
test_source_parse_with_description(void **state)
{
    const char *a =
        "VAR1: asd asd\n"
        "VAR2: 123chunda\n"
        "DESCRIPTION: huehuehuebrbr\n"
        "----------\n"
        "# This is a test\n"
        "\n"
        "bola\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(err);
    assert_non_null(source);
    assert_int_equal(bc_trie_size(source), 7);
    assert_string_equal(bc_trie_lookup(source, "VAR1"), "asd asd");
    assert_string_equal(bc_trie_lookup(source, "VAR2"), "123chunda");
    assert_string_equal(bc_trie_lookup(source, "EXCERPT"),
        "<h1 id=\"this-is-a-test\">This is a test</h1>\n"
        "<p>bola</p>\n");
    assert_string_equal(bc_trie_lookup(source, "CONTENT"),
        "<h1 id=\"this-is-a-test\">This is a test</h1>\n"
        "<p>bola</p>\n");
    assert_string_equal(bc_trie_lookup(source, "RAW_CONTENT"),
        "# This is a test\n"
        "\n"
        "bola\n");
    assert_string_equal(bc_trie_lookup(source, "FIRST_HEADER"), "This is a test");
    assert_string_equal(bc_trie_lookup(source, "DESCRIPTION"), "huehuehuebrbr");
    bc_trie_free(source);
}


static void
test_source_parse_config_empty(void **state)
{
    const char *a = "";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg, "Your source file is empty.");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_invalid_key(void **state)
{
    const char *a = "bola: guda";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "Can't find a configuration key or the content separator.\n"
        "Error occurred near line 1, position 1: bola: guda");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_no_key(void **state)
{
    const char *a = "BOLa";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "Invalid configuration key.\n"
        "Error occurred near line 1, position 4: BOLa");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_no_key2(void **state)
{
    const char *a = "BOLA";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "Your last configuration key is missing ':' and the value\n"
        "Error occurred near line 1, position 5: BOLA");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_no_value(void **state)
{
    // this is a special case, not an error
    const char *a = "BOLA:\r\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_non_null(source);
    assert_null(err);
    assert_string_equal(bc_trie_lookup(source, "BOLA"), "");
    bc_trie_free(source);
}


static void
test_source_parse_config_no_value2(void **state)
{
    const char *a = "BOLA:";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "Configuration value not provided for 'BOLA'.\n"
        "Error occurred near line 1, position 6: BOLA:");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_reserved_name(void **state)
{
    const char *a = "FILENAME: asd\r\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "'FILENAME' variable is forbidden in source files. It will be set "
        "for you by the compiler.");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_reserved_name2(void **state)
{
    const char *a = "CONTENT: asd\r\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "'CONTENT' variable is forbidden in source files. It will be set "
        "for you by the compiler.");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_reserved_name3(void **state)
{
    const char *a = "DATE_FORMATTED: asd\r\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "'DATE_FORMATTED' variable is forbidden in source files. It will be set "
        "for you by the compiler.");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_reserved_name4(void **state)
{
    const char *a = "DATE_FIRST_FORMATTED: asd\r\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "'DATE_FIRST_FORMATTED' variable is forbidden in source files. It will be set "
        "for you by the compiler.");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_reserved_name5(void **state)
{
    const char *a = "DATE_LAST_FORMATTED: asd\r\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "'DATE_LAST_FORMATTED' variable is forbidden in source files. It will be set "
        "for you by the compiler.");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_reserved_name6(void **state)
{
    const char *a = "PAGE_FIRST: asd\r\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "'PAGE_FIRST' variable is forbidden in source files. It will be set "
        "for you by the compiler.");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_reserved_name7(void **state)
{
    const char *a = "PAGE_PREVIOUS: asd\r\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "'PAGE_PREVIOUS' variable is forbidden in source files. It will be set "
        "for you by the compiler.");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_reserved_name8(void **state)
{
    const char *a = "PAGE_CURRENT: asd\r\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "'PAGE_CURRENT' variable is forbidden in source files. It will be set "
        "for you by the compiler.");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_reserved_name9(void **state)
{
    const char *a = "PAGE_NEXT: asd\r\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "'PAGE_NEXT' variable is forbidden in source files. It will be set "
        "for you by the compiler.");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_reserved_name10(void **state)
{
    const char *a = "PAGE_LAST: asd\r\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "'PAGE_LAST' variable is forbidden in source files. It will be set "
        "for you by the compiler.");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_reserved_name11(void **state)
{
    const char *a = "BLOGC_VERSION: 1.0\r\n";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "'BLOGC_VERSION' variable is forbidden in source files. It will be set "
        "for you by the compiler.");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_config_value_no_line_ending(void **state)
{
    const char *a = "BOLA: asd";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "No line ending after the configuration value for 'BOLA'.\n"
        "Error occurred near line 1, position 10: BOLA: asd");
    bc_error_free(err);
    bc_trie_free(source);
}


static void
test_source_parse_invalid_separator(void **state)
{
    const char *a = "BOLA: asd\n---#";
    bc_error_t *err = NULL;
    bc_trie_t *source = blogc_source_parse(a, strlen(a), &err, 0);
    assert_null(source);
    assert_non_null(err);
    assert_int_equal(err->type, BLOGC_ERROR_SOURCE_PARSER);
    assert_string_equal(err->msg,
        "Invalid content separator. Must be more than one '-' characters.\n"
        "Error occurred near line 2, position 4: ---#");
    bc_error_free(err);
    bc_trie_free(source);
}


int
main(void)
{
    const UnitTest tests[] = {
        unit_test(test_source_parse),
        unit_test(test_source_parse_crlf),
        unit_test(test_source_parse_with_spaces),
        unit_test(test_source_parse_with_excerpt),
        unit_test(test_source_parse_with_first_header),
        unit_test(test_source_parse_with_description),
        unit_test(test_source_parse_config_empty),
        unit_test(test_source_parse_config_invalid_key),
        unit_test(test_source_parse_config_no_key),
        unit_test(test_source_parse_config_no_key2),
        unit_test(test_source_parse_config_no_value),
        unit_test(test_source_parse_config_no_value2),
        unit_test(test_source_parse_config_reserved_name),
        unit_test(test_source_parse_config_reserved_name2),
        unit_test(test_source_parse_config_reserved_name3),
        unit_test(test_source_parse_config_reserved_name4),
        unit_test(test_source_parse_config_reserved_name5),
        unit_test(test_source_parse_config_reserved_name6),
        unit_test(test_source_parse_config_reserved_name7),
        unit_test(test_source_parse_config_reserved_name8),
        unit_test(test_source_parse_config_reserved_name9),
        unit_test(test_source_parse_config_reserved_name10),
        unit_test(test_source_parse_config_reserved_name11),
        unit_test(test_source_parse_config_value_no_line_ending),
        unit_test(test_source_parse_invalid_separator),
    };
    return run_tests(tests);
}
