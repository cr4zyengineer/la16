/*
 * MIT License
 *
 * Copyright (c) 2024 cr4zyengineer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <compiler/parse.h>

/*
 * Checking symbols
 */
static bool parse_type_is_hex(const char *line,
                              unsigned long *num)
{
    // First check is.. if the first character is a 0
    if(line[0] != '0') return false;

    // Second check is.. if the 2nd character is a x or a X
    if(line[1] != 'x' && line[1] != 'X') return false;

    // Third check is to check if the rest of the string is 0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f,A,B,C,D,E,F
    // To do this we just check for bounds.. each character has to be either in 0 - 9, a - f or A - F
    for(unsigned long i = 2;; i++)
    {
        if(line[i] == '\0')
        {
            if(num)
            {
                *num = strtoul(line, NULL, 16);
            }
            return true;
        }

        if((line[i] < '0' || line[i] > '9') &&
            (line[i] < 'a' || line[i] > 'f') &&
            (line[i] < 'A' || line[i] > 'F'))
        {
            return false;
        }
    }

    // If it passed all its a hexadecimal string
    return false;
}

static bool parse_type_is_bin(const char *line,
                              unsigned long *num)
{
    // First check is.. if the first character is a 0
    if(line[0] != '0') return false;

    // Second check is.. if the 2nd character is a x or a X
    if(line[1] != 'b' && line[1] != 'B') return false;

    // Third check is to check if the rest of the string is 0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f,A,B,C,D,E,F
    // To do this we just check for bounds.. each character has to be either in 0 - 9, a - f or A - F
    for(unsigned long i = 2;; i++)
    {
        if(line[i] == '\0')
        {
            if(num)
            {
                *num = strtoul(line + 2, NULL, 2);
            }
            return true;
        }

        if(line[i] < '0' || line[i] > '1')
        {
            return false;
        }
    }

    // If it passed all its a hexadecimal string
    return false;
}

static bool parse_type_is_dec(const char *line,
                              unsigned long *num)
{
    for(unsigned long i = 0;; i++)
    {
        if(line[i] == '\0')
        {
            if(num)
            {
                *num = strtoul(line, NULL, 10);
            }
            return true;
        }

        if(line[i] < '0' || line[i] > '9')
        {
            return false;
        }
    }

    // If it passed all its a hexadecimal string
    return false;
}

static bool parse_type_is_char(const char *line, unsigned long *num)
{
    // Must start and end with single quotes
    if (line[0] != '\'' || line[2] == '\0')
        return false;

    // Find closing quote
    size_t len = strlen(line);
    if (len < 3 || line[len - 1] != '\'')
        return false;

    char c;

    if (line[1] == '\\') {
        // Handle escape sequences like '\n', '\0', '\\', '\t'
        switch (line[2]) {
            case 'n': c = '\n'; break;
            case 't': c = '\t'; break;
            case 'r': c = '\r'; break;
            case 'b': c = '\b'; break;
            case '0': c = '\0'; break;
            case '\\': c = '\\'; break;
            case '\'': c = '\''; break;
            default:
                return false; // Unknown escape
        }
    } else {
        // Just a normal character like 'a'
        if (len != 3)
            return false; // Should be like 'a'
            c = line[1];
    }

    if (num)
        *num = (unsigned long)(unsigned char)c;

    return true;
}

static bool parse_type_is_buffer(const char *line, unsigned long *num, unsigned long *blen)
{
    // Must start and end with single quotes
    size_t len = strlen(line);
    if (len < 3 || line[0] != '\"' || line[len - 1] != '\"')
    {
        return false;
    }

    // Allocate temporary buffer (worst case: no escapes)
    char *buf = malloc(len - 1);
    if (!buf)
        return false;

    size_t out = 0;
    for (size_t i = 1; i < len - 1; i++) {
        char c = line[i];
        if (c == '\\') {
            // Handle escape sequences
            if (i + 1 >= len - 1) {
                free(buf);
                return false;
            }
            char esc = line[++i];
            switch (esc) {
                case 'n':  buf[out++] = '\n'; break;
                case 't':  buf[out++] = '\t'; break;
                case 'r':  buf[out++] = '\r'; break;
                case 'b':  buf[out++] = '\b'; break;
                case '0':  buf[out++] = '\0'; break;
                case '\\': buf[out++] = '\\'; break;
                case '\'': buf[out++] = '\''; break;
                default:
                    free(buf);
                    return false; // Unknown escape
            }
        } else {
            buf[out++] = c;
        }
    }

    // Null-terminate
    buf[out] = '\0';
    *blen = out;

    if (num)
        *num = (unsigned long)buf;

    return true;
}

/*
 * Main parser
 */
unsigned char parse_type(const char *line,
                         unsigned long *fastdec,
                         unsigned long *len)
{
    if(parse_type_is_hex(line, fastdec))
    {
        return PARSE_TYPE_HEX;
    }
    else if(parse_type_is_bin(line, fastdec))
    {
        return PARSE_TYPE_BIN;
    }
    else if(parse_type_is_dec(line, fastdec))
    {
        return PARSE_TYPE_NUMBER;
    }
    else if(parse_type_is_char(line, fastdec))
    {
        return PARSE_TYPE_CHAR;
    }
    else if(parse_type_is_buffer(line, fastdec, len))
    {
        return PARSE_TYPE_BUFFER;
    }
    return PARSE_TYPE_STRING;
}

parse_type_return_t parse_type_lc(const char *line)
{
    parse_type_return_t retval ={};
    retval.type = parse_type(line, &(retval.value), &(retval.len));
    return retval;
}
