/* (MKSKEL) mkskel.h: common functions
 *
 * This file is in the public domain, to the extent possible under law,
 * published under the CC0 1.0 Universal license.
 *
 * For a full copy of the CC0 license see
 * https://creativecommons.org/publicdomain/zero/1.0/legalcode */

#include <errno.h>
#include <string.h>

#ifndef DEFAUT_PATH_VAR
#define DEFAUT_PATH_VAR "SKELPATH"
#endif

#ifndef DEFAULT_PATH
#define DEFAULT_PATH "/usr/share/skel"
#endif

#ifndef OUTPUT_PREFIX
#define OUTPUT_PREFIX "__OUT__"
#endif

#ifdef DEBUG
#define err(code, message)						\
	 do{										\
		  fprintf(stderr, "%c:%d: %s\n",		\
				  __FILE__,						\
				  __LINE__,						\
				  strerror(errno));				\
		  exit(code);							\
	 } while (0);
#else
#define err(code, message)						\
	 do {										\
		  perror(message);						\
		  exit(code);							\
	 } while (0);
#endif

extern char *output;
extern bool force;

void create_skel(char *, char *);
void process_skel(char *, char *);
void list_skel(void);
