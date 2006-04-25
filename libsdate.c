/*
  Copyright (C) 1993 Christoph Berg <cb@df7cb.de>
  This program is free software, see COPYING for copyright terms.
  sdate is based on fakeroot:

  Copyright: GPL. 
  Author: joost witteveen  (joostje@debian.org)
*/
/* #define _POSIX_C_SOURCE 199309L whatever that may mean...*/ 
/* #define _BSD_SOURCE             I use strdup, S_IFDIR, etc */ 

/* Roderich Schupp writes (bug #79100):
   /usr/include/dlfcn.h from libc6 2.2-5 defines RTLD_NEXT only
   when compiled with _GNU_SOURCE defined. Hence libfakeroot.c doesn't pick
   it
   up and does a dlopen("/lib/libc.so.6",...) in get_libc().
   This works most of the time, but explodes if you have an arch-optimized
   libc installed: the program now has two versions of libc.so
   (/lib/libc.so.6 and, say, /lib/i586/libc.so.6) mapped. Again for
   some programs you might get away with this, but running bash under
   fakeroot
   always bombs. Simple fix:
*/
#define _GNU_SOURCE 

#include "config.h"
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <unistd.h> 
#include <dirent.h>
#include <errno.h>

/* sdate */
#include <time.h>


/* 
   Where are those shared libraries? 
   If I knew of a configure/libtool way to find that out, I'd use it. Or
   any other way other than the method I'm using below. Does anybody know
   how I can get that location? (BTW, symply linking a programme, and running
   `ldd' on it isn't the option, as Digital Unix doesn't have ldd)
*/


/* 
   Note that LIBCPATH isn't actually used on Linux or Solaris, as RTLD_NEXT
   is defined and we use that to get the `next_*' functions

   Linux:
*/

/* OSF1 :*/
/*#define LIBCPATH "/usr/shlib/libc.so"*/

/*
// next_wrap_st:
// this structure is used in next_wrap, which is defined in
// wrapstruct.h, included below
*/
 
struct next_wrap_st{
  void **doit;
  char *name;
};

void *get_libc(){
 
#ifndef RTLD_NEXT
 void *lib=0;
 if(!lib){ 
   lib= dlopen(LIBCPATH,RTLD_LAZY);
 }
 if (NULL==lib) {
   fprintf(stderr, "Couldn't find libc at: %s\n", LIBCPATH);
   abort();
 }
 return lib;
#else
  return RTLD_NEXT;
#endif
}
void load_library_symbols(void);

#include "wrapped.h"
#include "wraptmpf.h"
#include "wrapdef.h"
#include "wrapstruct.h"


void load_library_symbols(void){
  /* this function loads all original functions from the C library.
     I ran into problems when  each function individually
     loaded it's original counterpart, as RTLD_NEXT seems to have
     a different meaning in files with different names than libtricks.c
     (I.E, dlsym(RTLD_NEXT, ...) called in vsearch.c returned funtions
     defined in libtricks */
  /* The calling of this function itself is somewhat tricky:
     the awk script wrapawk generates several .h files. In wraptmpf.h
     there are temporary definitions for tmp_*, that do the call
     to this function. The other generated .h files do even more tricky
     things :) */

  static int done=0;
  int i;
  char* msg;
  
  if(!done){
    for(i=0; next_wrap[i].doit; i++){
      *(next_wrap[i].doit)=dlsym(get_libc(), next_wrap[i].name);
      if ( (msg = dlerror()) != NULL){
	fprintf (stderr, "dlsym(%s): %s\n", next_wrap[i].name, msg);
/*	abort ();*/
      }
    }
  }
}

/* sdate */

static inline int epoch_days(int y, int m) /* days past 1970-01-01 */
{
                           /* 1  2   3   4   5    6    7    8    9    10   11   12 */
  static int mon_offset[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
  return 365 * (y - 70)
	+ (int)((y - 69) / 4)         /* leap days in previous years */
	+ ((y % 4 == 0) && (m >= 2))  /* this year's leap day */
	+ mon_offset[m];
}

static struct tm *septemberfy(struct tm *t)
{
  static int sep_offset = -1;
  static int sep_y = 93;
  static int sep_m = 8;

  if (sep_offset == -1) {
    char *e;
    if ((e = getenv("SDATE_EPOCH"))) {
      sscanf(e, "%d-%d", &sep_y, &sep_m);
      sep_m--;
    }

    if (sep_y < 70)
      sep_y += 100;
    if (sep_y > 1900)
      sep_y -= 1900;
    if (sep_y < 1 || sep_y > 199) /* range where our leap year stuff works */
      sep_y = 93;
    if (sep_m < 0 || sep_m > 11)
      sep_m = 8;

    sep_offset = epoch_days(sep_y, sep_m);
  }

  if((t->tm_year == sep_y && t->tm_mon > sep_m) || t->tm_year > sep_y) {
#ifdef DEBUG
    fprintf(stderr, "septemberfy: %d-%d-%d\n", t->tm_year, t->tm_mon, t->tm_mday);
#endif
    if(t->tm_mon >= 0 && t->tm_mon < 12)
      t->tm_mday += epoch_days(t->tm_year, t->tm_mon) - sep_offset;
    t->tm_mon = sep_m;
    t->tm_year = sep_y;
  }
  return t;
}

struct tm *gmtime(const time_t *timep) {
  struct tm *result = next_gmtime(timep);
#ifdef DEBUG
  fputs("gmtime wrapped\n", stderr);
#endif
  return septemberfy(result);
}

struct tm *gmtime_r(const time_t *timep, struct tm *result) {
#ifdef DEBUG
  fputs("gmtime_r wrapped\n", stderr);
#endif
  result = next_gmtime_r(timep, result);
  return septemberfy(result);
}

struct tm *localtime(const time_t *timep) {
  struct tm *result = next_localtime(timep);
#ifdef DEBUG
  fputs("localtime wrapped\n", stderr);
#endif
  return septemberfy(result);
}

struct tm *localtime_r(const time_t *timep, struct tm *result) {
#ifdef DEBUG
  fputs("localtime_r wrapped\n", stderr);
#endif
  result = next_localtime_r(timep, result);
  return septemberfy(result);
}

/*
time_t mktime(struct tm *tm) {
#ifdef DEBUG
  fputs("mktime wrapped\n", stderr);
#endif
  return next_mktime(tm);
}
*/
