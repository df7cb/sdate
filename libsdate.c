/*
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

int fakeroot_disabled = 0;

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


/* strftime-rel */

size_t strftime(char *s, size_t max, const char *format, const struct tm *tm) {
  if(!strcmp(format, "REL")) {
    time_t t = time(NULL);
    struct tm now;
    localtime_r(&t, &now);
    if(now.tm_year == tm->tm_year && now.tm_mon == tm->tm_mon && now.tm_mday == tm->tm_mday) {
      format = "%H:%M:%S";
    } else {
      format = "%y/%m/%d";
    }
  }
  return next_strftime(s, max, format, tm);
}

