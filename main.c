/* (MKSKEL) main.c: process environmental data and start copying
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

#include <sys/types.h>

#include "mkskel.h"

char *output;
static void
usage(char *name)
{
	 fprintf(stderr, "usage: %s [-l | -C dir | -o file] [skeleton]\n", name);
	 exit(EXIT_FAILURE);
}

static char *
find_skel(char *name, char *dir)
{
	 DIR *d = opendir(dir);
	 struct dirent *ent;
	 char *skel = NULL;

	 if (!d) {
		  perror("opendir");
		  exit(EXIT_FAILURE);
	 }

	 errno = 0;
	 while ((ent = readdir(d))) {
		  if (!strcmp(name, ent->d_name)) {
			   skel = ent->d_name;
			   break;
		  }
	 }
	 if (0 != errno) {
		  perror("readdir");
		  exit(EXIT_FAILURE);
	 }
	 
	 closedir(d);
	 return skel;
}

int
main(int argc, char **argv)
{
	 bool list = false;
	 char *cwd = NULL;

	 /* parse flags */
	 for (int opt; (opt = getopt(argc, argv, "lC:o:")) != -1; ) {
		  switch (opt) {
		  case 'l': list = true; break;
		  case 'C': cwd = optarg; break;
		  case 'o': output = optarg; break;
		  default: usage(argv[0]);
		  }
	 }

	 /* if requested, list all found skeletons */
	 if (list) {
		  list_skel();
		  return EXIT_SUCCESS;
	 }

	 /* check if skeleton is given */
	 if ((argc - optind) < 1) {
		  usage(argv[0]);
	 }
	 
	 /* search SKELPATH for matching skeleton */
	 char *skel = NULL;
	 char *path = getenv(DEFAUT_PATH_VAR);
	 if (!path) path = DEFAULT_PATH;
	 char *dir = strtok(path, ":");
	 while (dir) {
		  skel = find_skel(argv[1], dir);
		  if (skel) break;
		  dir = strtok(NULL, ":");
	 }

	 /* check if skeleton was found */
	 if (!skel) {
		  fprintf(stderr, "no skeleton under the name '%s' could be found\n", argv[1]);
	 }
	 
	 /* change directory if neccesary */
	 if (cwd && -1 == chdir(cwd)) {
		  perror("chdir");
		  exit(EXIT_FAILURE);
	 }

	 /* create skeleton in cwd */
	 create_skel(skel, dir);

	 return EXIT_SUCCESS;
}
