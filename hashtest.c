/********************************************************************\
 *
 *      FILE:     hashtest.c
 *
 *      CONTENTS: test file for sample C-implementation of
 *                RIPEMD-160 and RIPEMD128
 *        * command line arguments:                                         
 *           filename  -- compute hash code of file read binary     
 *           -sstring  -- print string & hashcode                   
 *           -t        -- perform time trial                        
 *           -x        -- execute standard test suite, ASCII input
 *        * for linkage with rmd128.c: define RMDsize as 128
 *          for linkage with rmd160.c: define RMDsize as 160 (default)
 *      TARGET:   any computer with an ANSI C compiler
 *
 *      AUTHOR:   Antoon Bosselaers, ESAT-COSIC
 *      DATE:     18 April 1996
 *      VERSION:  1.1
 *      HISTORY:  bug in RMDonemillion() corrected
 *
 *      Copyright (c) 1996 Katholieke Universiteit Leuven
 *
 *      Permission is hereby granted, free of charge, to any person
 *      obtaining a copy of this software and associated documentation
 *      files (the "Software"), to deal in the Software without restriction,
 *      including without limitation the rights to use, copy, modify, merge,
 *      publish, distribute, sublicense, and/or sell copies of the Software,
 *      and to permit persons to whom the Software is furnished to do so,
 *      subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be 
 *      included in all copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 *      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 *      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 *      IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
 *      CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 *      TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *      SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
\********************************************************************/
#ifndef RMDsize
#define RMDsize 160
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#if RMDsize == 128
#include "rmd128.h"
#elif RMDsize == 160
#include "rmd160.h"
#endif

#define TEST_BLOCK_SIZE 8000
#define TEST_BLOCKS 1250
#define TEST_BYTES ((long)TEST_BLOCK_SIZE * (long)TEST_BLOCKS)

/********************************************************************/

byte *RMD(byte *message)
/*
 * returns RMD(message)
 * message should be a string terminated by '\0'
 */
{
   dword         MDbuf[RMDsize/32];   /* contains (A, B, C, D(, E))   */
   static byte   hashcode[RMDsize/8]; /* for final hash-value         */
   dword         X[16];               /* current 16-word chunk        */
   unsigned int  i;                   /* counter                      */
   dword         length;              /* length in bytes of message   */
   dword         nbytes;              /* # of bytes not yet processed */

   /* initialize */
   MDinit(MDbuf);
   length = (dword)strlen((char *)message);

   /* process message in 16-word chunks */
   for (nbytes=length; nbytes > 63; nbytes-=64) {
      for (i=0; i<16; i++) {
         X[i] = BYTES_TO_DWORD(message);
         message += 4;
      }
      compress(MDbuf, X);
   }                                    /* length mod 64 bytes left */

   /* finish: */
   MDfinish(MDbuf, message, length, 0);

   for (i=0; i<RMDsize/8; i+=4) {
      hashcode[i]   =  MDbuf[i>>2];         /* implicit cast to byte  */
      hashcode[i+1] = (MDbuf[i>>2] >>  8);  /*  extracts the 8 least  */
      hashcode[i+2] = (MDbuf[i>>2] >> 16);  /*  significant bits.     */
      hashcode[i+3] = (MDbuf[i>>2] >> 24);
   }

   return (byte *)hashcode;
}

void RMDstring(char *message, char *print)
{
   unsigned int  i;
   byte         *hashcode;

   hashcode = RMD((byte *)message);
   printf("\n* message: %s\n  hashcode: ", print);
   for (i=0; i<RMDsize/8; i++)
      printf("%02x", hashcode[i]);
}

void testsuite (void) {
   printf("\n\nRIPEMD-%u test suite results (ASCII):\n", RMDsize);

   RMDstring("", "\"\" (empty string)");
   RMDstring("a", "\"a\"");
   RMDstring("abc", "\"abc\"");
   RMDstring("message digest", "\"message digest\"");
   RMDstring("abcdefghijklmnopqrstuvwxyz", "\"abcdefghijklmnopqrstuvwxyz\"");
   RMDstring("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
             "\"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq\"");
   RMDstring("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
             "\"A...Za...z0...9\"");
   RMDstring("1234567890123456789012345678901234567890"
             "1234567890123456789012345678901234567890", 
             "8 times \"1234567890\"");
   printf("\n");

   return;
}

/********************************************************************/

int main () {
  testsuite ();
  printf("\n");
  return 0;
}
