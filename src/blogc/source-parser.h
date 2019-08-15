/*
 * blogc: A blog compiler.
 * Copyright (C) 2014-2019 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the BSD License.
 * See the file LICENSE.
 */

#ifndef _SOURCE_PARSER_H
#define _SOURCE_PARSER_H

#include <stddef.h>
#include "../common/error.h"
#include "../common/utils.h"

bc_trie_t* blogc_source_parse(const char *src, size_t src_len,
    bc_error_t **err, size_t header_add);

#endif /* _SOURCE_PARSER_H */
