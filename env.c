/* (MKSKEL) env.c: manage skeleton variables
 *
 * This file is in the public domain, to the extent possible under law,
 * published under the CC0 1.0 Universal license.
 *
 * For a full copy of the CC0 license see
 * https://creativecommons.org/publicdomain/zero/1.0/legalcode */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "mkskel.h"

#define TIME(fmt, var)								\
	 do {											\
		  time_t t = time(NULL);					\
		  struct tm *now = localtime(&t);			\
		  if (!now) break;							\
		  if (!strftime(var, sizeof var, fmt, now))	\
			   break;								\
	 } while (0)

void
set_envvar(char *skel, char *dir)
{
	 size_t i;
	 char date[64], date_mdy[64], date_dmy[64];
	 struct { char *name, *value; } data[] = {
		  { .name = "OUTPUT", .value = output },
		  { .name = "SKEL", .value = skel },
		  { .name = "SKELDIR", .value = dir },
		  { .name = "DATE", .value = dir },
		  { .name = "DATE_MDY", .value = dir },
		  { .name = "DATE_DMY", .value = dir },
	 };

	 TIME("%F", date);
	 TIME("%B %d, %Y", date_mdy);
	 TIME("%D", date_dmy);

	 for (i = 0; i < sizeof(data)/sizeof(data[0]); i++) {
		  if (-1 == setenv(data[i].name, data[i].value, 1)) {
			   err(EXIT_FAILURE, "setenv");
		  }
	 }
}
