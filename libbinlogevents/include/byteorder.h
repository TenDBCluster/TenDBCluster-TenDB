/* Copyright (c) 2014, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

/**
  @file byteorder.h

  @brief The file contains functions to convert the byte encoding of integer
  values to and from little-endian and big-endian byte order.
*/

#ifndef BYTEORDER_INCLUDED
#define BYTEORDER_INCLUDED

#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

#include "binlog_config.h"

/*
  Define types uint_t typedefs if stdint is not present
*/
#if !defined(HAVE_STDINT_H)
  #if SIZEOF_INT == 4
    typedef int int32_t;
    typedef unsigned int uint32_t;
  #elif SIZEOF_LONG == 4
    typedef long int32_t;
    typedef unsigned long uint32_t;
  #else
    #error Neither int nor long is of 4 bytes width
  #endif
  typedef unsigned short int uint16_t;
  #if SIZEOF_LONG_LONG == 8
     typedef unsigned long long int uint64_t;
  #else
     typedef unsigned long uint64_t;
  #endif
#else
  #include <stdint.h>
#endif //end ifndef HAVE_STDINT_H

/*
  Methods for reading and storing in machine independent
  format (low byte first).
*/

/*
  Checking le16toh is required because the machine may have the header
  but the functions might not be defined if the version of glibc < 2.9
*/
#ifdef HAVE_ENDIAN_CONVERSION_MACROS
  #include <endian.h>
#endif

#if !defined(le16toh)
/**
  Converting a 16 bit integer from little-endian byte order to host byteorder

  @param x  16-bit integer in little endian byte order
  @return  16-bit integer in host byte order
*/
uint16_t inline le16toh(uint16_t x)
{
  #if !(IS_BIG_ENDIAN)
    return x;
  #else
    return ((x >> 8) | (x << 8));
  #endif
}
#endif

#if !defined(le32toh)
/**
  Converting a 32 bit integer from little-endian byte order to host byteorder

  @param x  32-bit integer in little endian byte order
  @return  32-bit integer in host byte order
*/
uint32_t inline le32toh(uint32_t x)
{
  #if !(IS_BIG_ENDIAN)
    return x;
  #else
    return (((x >> 24) & 0xff) |
            ((x <<  8) & 0xff0000) |
            ((x >>  8) & 0xff00) |
            ((x << 24) & 0xff000000));
  #endif
}
#endif

#if !defined(be32toh)
/**
  Converting a 32 bit integer from big-endian byte order to host byteorder

  @param x  32-bit integer in big endian byte order
  @return  32-bit integer in host byte order
*/
uint32_t inline be32toh(uint32_t x)
{
  #if !(IS_BIG_ENDIAN)
     return (((x >> 24) & 0xff) |
             ((x <<  8) & 0xff0000) |
             ((x >>  8) & 0xff00) |
             ((x << 24) & 0xff000000));
  #else
     return x;
  #endif
}
#endif

#if !defined(le64toh)
/**
  Converting a 64 bit integer from little-endian byte order to host byteorder

  @param x  64-bit integer in little endian byte order
  @return  64-bit integer in host byte order
*/
uint64_t inline le64toh(uint64_t x)
{
  #if !(IS_BIG_ENDIAN)
    return x;
  #else
    x = ((x << 8) & 0xff00ff00ff00ff00ULL) |
        ((x >> 8) & 0x00ff00ff00ff00ffULL);
    x = ((x << 16) & 0xffff0000ffff0000ULL) |
        ((x >> 16) & 0x0000ffff0000ffffULL);
    return (x << 32) | (x >> 32);
  #endif
}
#endif

#define do_compile_time_assert(X)                                              \
  do                                                                        \
  {                                                                         \
    typedef char do_compile_time_assert[(X) ? 1 : -1] __attribute__((unused)); \
  } while(0)
#endif // BYTEORDER_INCLUDED
