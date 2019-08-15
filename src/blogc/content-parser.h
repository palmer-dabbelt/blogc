/*
 * blogc: A blog compiler.
 * Copyright (C) 2014-2019 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the BSD License.
 * See the file LICENSE.
 */

#ifndef _CONTENT_PARSER_H
#define _CONTENT_PARSER_H

#include <stddef.h>
#include <stdbool.h>

char* blogc_slugify(const char *str);
char* blogc_htmlentities(const char *str);
char* blogc_fix_description(const char *paragraph);
char* blogc_content_parse_inline(const char *src);
bool blogc_is_ordered_list_item(const char *str, size_t prefix_len);
char* blogc_content_parse(const char *src, size_t *end_excerpt,
    char **first_header, char **description, size_t header_add);

#endif /* _CONTENT_PARSER_H */
