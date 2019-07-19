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
create_skel(char *skel, char *dir)
{
	 char full[strlen(dir) + 1 + strlen(skel)];
	 sprintf(full, "%s/%s", dir, skel);
	 struct stat buf;
	 if (-1 == stat(full, &buf)) {
		  perror("stat");
		  exit(EXIT_FAILURE);
	 }

	 switch (buf.st_mode & S_IFMT) {
	 case S_IFDIR: {
		  char ndir[strlen(dir) + 1 + strlen(skel)];
		  sprintf(ndir, "%s/%s", dir, skel);

		  DIR *d = opendir(ndir);
		  struct dirent *ent;

		  if (!d) {
			   perror("opendir");
			   exit(EXIT_FAILURE);
		  }

		  errno = 0;
		  while ((ent = readdir(d))) {
			   if (!strcmp(ent->d_name, ".") ||
				   !strcmp(ent->d_name, ".."))
					continue;
			   create_skel(ent->d_name, ndir);			   
		  }
		  if (0 != errno) {
			   perror("readdir");
			   exit(EXIT_FAILURE);
		  }
		  
		  closedir(d);
		  break;
	 }
	 case S_IFREG: {
		  char new[strlen(dir) + 1 + strlen(skel)];
		  sprintf(new, "%s/%s", dir, skel);
		  process_skel(new, skel);
		  break;
	 }
	 default:
		  fprintf(stderr, "encountered invalid file '%s'\n", skel);
	 }
}
