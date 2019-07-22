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

void
create_skel(char *file, char *dir, char *rel)
{
	 char rfile[strlen(dir) + 1 + strlen(file)];
	 sprintf(rfile, "%s/%s", dir, file);
	 struct stat buf;
	 if (-1 == stat(rfile, &buf)) {
		  err(EXIT_FAILURE, "stat");
	 }

	 switch (buf.st_mode & S_IFMT) {
	 case S_IFDIR: {
		  char ndir[strlen(dir) + 1 + strlen(file)];
		  sprintf(ndir, "%s/%s", dir, file);

		  char nrel[(rel ? strlen(rel) : 1) + 1 + strlen(file)];
		  if (rel == NULL) {
			   sprintf(nrel, ".");
		  } else {
			   sprintf(nrel, "%s/%s", rel, file);
		  }

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
			   if (-1 == mkdir(nrel, S_IRWXU)) {
					if (errno == EEXIST) {
						 struct stat sb;
						 stat(nrel, &sb);
						 if (!S_ISDIR(sb.st_mode))
							  err(EXIT_FAILURE, "mkdir");
					} else err(EXIT_FAILURE, "mkdir");
			   }

			   create_skel(ent->d_name, ndir, nrel);
			   errno = 0;
		  }
		  if (0 != errno) {
			   err(EXIT_FAILURE, "readdir");
		  }

		  closedir(d);
		  break;
	 }
	 case S_IFREG: {
		  if (!strncmp(file, OUTPUT_PREFIX, strlen(OUTPUT_PREFIX))) {
			   char subst[strlen(file) + strlen(output) - strlen(OUTPUT_PREFIX)];
			   sprintf(subst, "%s%s", output, file + strlen(OUTPUT_PREFIX));

			   char skel[strlen(dir) + 1 + strlen(file)];
			   sprintf(skel, "%s/%s", dir, file);

			   char nfile[strlen(rel) + 1 + strlen(subst)];
			   sprintf(nfile, "%s/%s", rel, subst);

			   set_envvar(subst, dir);
			   process_skel(skel, nfile);
		  } else {
			   char skel[strlen(dir) + 1 + strlen(file)];
			   sprintf(skel, "%s/%s", dir, file);

			   char nfile[strlen(rel) + 1 + strlen(file)];
			   sprintf(nfile, "%s/%s", rel, file);

			   set_envvar(file, dir);
			   process_skel(skel, nfile);
		  }
		  break;
	 }
	 default:
		  fprintf(stderr, "encountered invalid file '%s'\n", file);
	 }
}
