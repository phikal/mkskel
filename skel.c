/* (MKSKEL) skel.c: copy skeleton structure
 *
 * This file is in the public domain, to the extent possible under law,
 * published under the CC0 1.0 Universal license.
 *
 * For a full copy of the CC0 license see
 * https://creativecommons.org/publicdomain/zero/1.0/legalcode */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "mkskel.h"

static void
set_envvar(char *skel, char *dir)
{
	 size_t i;
	 struct { char *name, *value; } data[] = {
		  { .name = "SKEL", .value = skel },
		  { .name = "FULLPATH", .value = dir },
	 };

	 for (i = 0; i < sizeof(data)/sizeof(data[0]); i++) {
		  if (-1 == setenv(data[i].name, data[i].value, 1)) {
			   err(EXIT_FAILURE, "setenv");
		  }
	 }
}

void
create_skel(char *skel, char *dir)
{
	 char full[strlen(dir) + 1 + strlen(skel)];
	 sprintf(full, "%s/%s", dir, skel);
	 struct stat buf;
	 if (-1 == stat(full, &buf)) {
		  err(EXIT_FAILURE, "stat");
	 }

	 switch (buf.st_mode & S_IFMT) {
	 case S_IFDIR: {
		  char ndir[strlen(dir) + 1 + strlen(skel)];
		  sprintf(ndir, "%s/%s", dir, skel);

		  DIR *d = opendir(ndir);
		  struct dirent *ent;

		  if (!d) {
			   err(EXIT_FAILURE, "opendir");
		  }

		  errno = 0;
		  while ((ent = readdir(d))) {
			   if (!strcmp(ent->d_name, ".") ||
				   !strcmp(ent->d_name, ".."))
					continue;
			   create_skel(ent->d_name, ndir);			   
		  }
		  if (0 != errno) {
			   err(EXIT_FAILURE, "readdir");
		  }
		  
		  closedir(d);
		  break;
	 }
	 case S_IFREG: {
		  char full[strlen(dir) + 1 + strlen(skel)];
		  sprintf(full, "%s/%s", dir, skel);

		  if (!strncmp(skel, OUTPUT_PREFIX, strlen(OUTPUT_PREFIX))) {
			   char subst[strlen(output) + strlen(skel) - strlen(OUTPUT_PREFIX)];
			   if (!output) {
					fprintf(stderr, "output file required for skeleton not specified\n");
					exit(EXIT_FAILURE);
			   }
			   sprintf(subst, "%s%s", output, skel + strlen(OUTPUT_PREFIX));
			   set_envvar(subst, dir);
			   process_skel(full, subst);
		  } else {
			   set_envvar(skel, dir);
			   process_skel(full, skel);
		  }
		  break;
	 }
	 default:
		  fprintf(stderr, "encountered invalid file '%s'\n", skel);
	 }
}
