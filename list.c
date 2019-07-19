/* (MKSKEL) list.c: create listing of skeletons
 *
 * This file is in the public domain, to the extent possible under law,
 * published under the CC0 1.0 Universal license.
 *
 * For a full copy of the CC0 license see
 * https://creativecommons.org/publicdomain/zero/1.0/legal code */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#include "mkskel.h"

static void
find_skels(char *dir)
{
	 DIR *d = opendir(dir);
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
		  printf("%s: %s\n", dir, ent->d_name);
	 }
	 if (0 != errno) {
		  perror("readdir");
		  exit(EXIT_FAILURE);
	 }
	 
	 closedir(d);
}

void
list_skel(void)
{
	 char *path = getenv(DEFAUT_PATH_VAR);
	 if (!path) path = DEFAULT_PATH;
	 char *dir = strtok(path, ":");
	 
	 while (dir) {
		  find_skels(dir);
		  dir = strtok(NULL, ":");
	 }
}
