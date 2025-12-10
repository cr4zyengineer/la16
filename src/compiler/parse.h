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

#ifndef TOKENENGINE_PARSE_H
#define TOKENENGINE_PARSE_H

enum PARSE_TYPE
{
    PARSE_TYPE_STRING    = 0b0001,
    PARSE_TYPE_NUMBER    = 0b0010,
    PARSE_TYPE_HEX       = 0b0011,
    PARSE_TYPE_BIN       = 0b0100,
    PARSE_TYPE_CHAR      = 0b0101,
    PARSE_TYPE_BUFFER    = 0b0110
};

struct parse_type_return
{
    unsigned long value;
    unsigned long len;
    unsigned char type;
};

typedef struct parse_type_return parse_type_return_t;

parse_type_return_t parse_type_lc(const char *line);

#endif /* TOKENENGINGE_PARSE_H */
