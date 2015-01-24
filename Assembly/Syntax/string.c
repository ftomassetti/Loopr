#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MEM.h"
#include "UTL.h"
#include "EBS.h"
#include "DBG.h"
#include "Assembly.h"

#define STRING_ALLOC_SIZE       (256)

static char *st_string_literal_buffer = NULL;
static int st_string_literal_buffer_size = 0;
static int st_string_literal_buffer_alloc_size = 0;
static Edge_Boolean st_string_literal_is_wstring = False;

Edge_Boolean
Asm_get_is_wstring(void)
{
	return st_string_literal_is_wstring;
}

void
Asm_open_string_literal(Edge_Boolean is_wstring)
{
    st_string_literal_buffer_size = 0;
	st_string_literal_is_wstring = is_wstring;
}

void
Asm_add_string_literal(int letter)
{
    if (st_string_literal_buffer_size == st_string_literal_buffer_alloc_size) {
        st_string_literal_buffer_alloc_size += STRING_ALLOC_SIZE;
        st_string_literal_buffer
            = MEM_realloc(st_string_literal_buffer,
                          st_string_literal_buffer_alloc_size);
    }
    st_string_literal_buffer[st_string_literal_buffer_size] = letter;
    st_string_literal_buffer_size++;
}

void
Asm_reset_string_literal_buffer(void)
{
    MEM_free(st_string_literal_buffer);
    st_string_literal_buffer = NULL;
    st_string_literal_buffer_size = 0;
    st_string_literal_buffer_alloc_size = 0;
}

Edge_Char *
Asm_close_string_literal(void)
{
    Edge_Char *new_str;
    int new_str_len;

    Asm_add_string_literal('\0');
    new_str_len = Edge_mbstowcs_len(st_string_literal_buffer);
    if (new_str_len < 0) {
		DBG_panic(("Bad multibyte character\n"));
    }
    new_str = MEM_malloc(sizeof(Edge_Char) * (new_str_len+1));
    Edge_mbstowcs(st_string_literal_buffer, new_str);

    return new_str;
}

char *
Asm_close_string_literal_string(void)
{
    char *new_str;
    int new_str_len;

    Asm_add_string_literal('\0');
    new_str_len = strlen(st_string_literal_buffer);
    if (new_str_len < 0) {
		DBG_panic(("Too few character\n"));
    }
    new_str = MEM_malloc(sizeof(char) * (new_str_len+1));
    strcpy(new_str, st_string_literal_buffer);

    return new_str;
}

int
Asm_close_character_literal(void)
{
    Edge_Char buf[16];
    int new_str_len;

    Asm_add_string_literal('\0');
    new_str_len = Edge_mbstowcs_len(st_string_literal_buffer);
    if (new_str_len < 0) {
        DBG_panic(("Bad multibyte character\n"));
    } else if (new_str_len > 1) {
		DBG_panic(("Too long character literal\n"));
    }
    
    Edge_mbstowcs(st_string_literal_buffer, buf);

    return buf[0];
}

char *
Asm_create_identifier(char *str)
{
    char *new_str;

    new_str = ASM_malloc(strlen(str) + 1);

    strcpy(new_str, str);

    return new_str;
}