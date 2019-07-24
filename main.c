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
#include <libgen.h>

#include <sys/types.h>

#include "mkskel.h"

char *output;
bool force;

static void
usage(char *name)
{
	 fprintf(stderr, "usage: %s [-lf | -C dir | -o file] [skeleton]\n", name);
	 exit(EXIT_FAILURE);
}

static char *
find_skel(char *name, char *dir)
{
	 DIR *d = opendir(dir);
	 struct dirent *ent;
	 char *skel = NULL;

	 if (!d) {
		  err(EXIT_FAILURE, "opendir");
	 }

	 errno = 0;
	 while ((ent = readdir(d))) {
		  if (!strcmp(name, ent->d_name)) {
			   skel = ent->d_name;
			   break;
		  }
	 }
	 if (0 != errno) {
		  err(EXIT_FAILURE, "readdir");
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
	 for (int opt; (opt = getopt(argc, argv, "flC:o:")) != -1; ) {
		  switch (opt) {
		  case 'f': force = true; break;
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

	 while ((argc - optind) >= 1) {
		  /* search SKELPATH for matching skeleton */
		  char *skel = NULL;
		  char *path = getenv(DEFAUT_PATH_VAR);
		  if (!path) path = DEFAULT_PATH;
		  char *dir = strtok(path, ":");
		  while (dir) {
			   skel = find_skel(argv[optind], dir);
			   if (skel) break;
			   dir = strtok(NULL, ":");
		  }

		  /* check if skeleton was found */
		  if (!skel) {
			   fprintf(stderr, "no skeleton under the name '%s' could be found\n", argv[optind]);
			   exit(EXIT_FAILURE);
		  }

		  /* change directory if neccesary */
		  if (cwd && -1 == chdir(cwd)) {
			   err(EXIT_FAILURE, "chdir");
		  }

		  /* default output to directory name */
		  if (!output) {
			   char dir_cwd[PATH_MAX];
			   if (getcwd(dir_cwd, sizeof(dir_cwd))) {
					output = basename(dir_cwd);
			   } else {
					err(EXIT_FAILURE, "getcwd");
			   }

			   if (!strcmp(output, "/")) {
			   		fprintf(stderr,
			   				"No default output name could be generated. "
			   				"Please specify a name with -o\n");
			   		return EXIT_FAILURE;
			   }
		  }

		  /* create skeleton in cwd */
		  create_skel(skel, dir, NULL);
		  optind++;
	 }

	 return EXIT_SUCCESS;
}
