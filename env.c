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

static void name();

static void date();
static void date_mdy();
static void date_dmy();

static struct { char *name; void (*fn)(); } printers[] = {
	 { .name = "NAME", .fn = name },
	 { .name = "DATE", .fn = date },
	 { .name = "DATE_MDY", .fn = date_mdy },
	 { .name = "DATE_DMY", .fn = date_dmy },
};

bool
print_var(char *var)
{
	 size_t i;
	 for (i = 0; i < sizeof(printers)/sizeof(printers[0]); i++) {
		  if (!strcmp(printers[i].name, var)) {
			   printers[i].fn();
			   return true;
		  }
	 }
	 return false;
}

/* user information */

static void name() {
	 char name[512] = {0};
	 if (getlogin_r(name, sizeof name))
		  puts(name);
}

/* date and time functions */

#define TIMEFN(name, fmt)							\
	 void name() {									\
		  char ts[512] = {0};						\
		  time_t t = time(NULL);					\
		  struct tm *now = localtime(&t);			\
		  if (!now) return;							\
		  if (!strftime(ts, sizeof ts, fmt, now))	\
			   return;								\
		  puts(ts);									\
	 }

TIMEFN(date, "%F");
TIMEFN(date_mdy, "%B %d, %Y");
TIMEFN(date_dmy, "%D");

void
set_envvar(char *skel, char *dir)
{
	 size_t i;
	 struct { char *name, *value; } data[] = {
		  { .name = "OUTPUT", .value = output },
		  { .name = "SKEL", .value = skel },
		  { .name = "SKELDIR", .value = dir },
	 };

	 for (i = 0; i < sizeof(data)/sizeof(data[0]); i++) {
		  if (-1 == setenv(data[i].name, data[i].value, 1)) {
			   err(EXIT_FAILURE, "setenv");
		  }
	 }
}
