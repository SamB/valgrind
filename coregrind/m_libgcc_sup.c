/* -*- mode: C; c-basic-offset: 3; -*- */

/*--------------------------------------------------------------------*/
/*--- Supplemental functions for libgcc normally provided by glibc ---*/
/*--------------------------------------------------------------------*/

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2020 Mark Wielaard
      mark@klomp.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.

   The GNU General Public License is contained in the file COPYING.
*/

#include "config.h"
#include "pub_core_basics.h"
#include "pub_core_clientstate.h"

/*====================================================================*/
/*=== arm64 libgcc support function for init_have_lse_atomics      ===*/
/*====================================================================*/

#if defined(VGP_arm64_linux)
#if defined(VGPV_arm64_linux_android)
// (Note: recent NDK versions use LLVM's compiler-rt builtins instead of
// libgcc.)
//
// This gets pretty fiddly due to the initial Android 8.0 kernel on Galaxy
// S9/S9+ giving results for ARMv8.2 even though the processor has a mix of
// ARMv8.0 and ARMv8.2 cores.
//
// Rather than trying to stub __system_property_get (which is the Bionic libc
// equivalent of the "getprop" command, and apparently can be used to detect the
// cpu model on the buggy Galaxy firmwares) and __stack_chk_fail (which seems to
// be there due to -fstack-protector), just disable LSE here for now.
//
// We could consider adding our own reimplementation of init_have_lse_atomics
// when __ANDROID_MIN_SDK_VERSION__ > 26 (26 is the api level for O, which is
// the code letter for Android 8.0) that just uses __getauxval(), though nothing
// would stop it running on an older device since we don't use libc in this
// binary anyway...
_Bool __aarch64_have_lse_atomics = 0;
#endif

struct auxv
{
   Word a_type;
   union {
      void *a_ptr;
      Word a_val;
   } u;
};
#define AT_NULL 0

unsigned long int __getauxval (unsigned long int type);
unsigned long int __getauxval (unsigned long int type)
{
   struct auxv *p;
   for (p = (struct auxv *) VG_(client_auxv);
        p != NULL && p->a_type != AT_NULL;
        p++)
      if (p->a_type == type)
         return p->u.a_val;

   return 0;
}
#endif

#if defined(VGO_solaris)
/* At least on Solaris 11.3 ar does not like
   empty .ar files */
int some_symbol_to_keep_ar_happy;
#endif
