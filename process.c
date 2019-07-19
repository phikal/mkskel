/* (MKSKEL) process.c: process individual skeleton file
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
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>

#include <sys/stat.h>

#include "mkskel.h"

static void
process(FILE *in, FILE *out)
{
	 char buf[BUFSIZ], *next = buf, *last, var[64] = {0}, *val;
	 size_t size, i = 0;
	 bool todo = true;

	 while (todo) {
		  if ((size = fread(buf, sizeof(char), sizeof(buf), in)) < sizeof(buf))
			   todo = false;

	 restart:
		  for (;;) {
			   last = next;
			   if (!(next = memchr(next, '$', size - (next - buf)))) {
					fwrite(last, size - (last - buf), sizeof(char), out);
					break;
			   }

			   fwrite(last, next - last, sizeof(char), out);
			   if (next + 1 >= buf + size) {
					buf[0] = next[0];
					if ((size = fread(buf + 1,
									  sizeof(char),
									  sizeof(buf) - 1,
									  in)) < sizeof(buf) - 1)
						 todo = false;
					goto restart;
			   }

			   next++;
			   if (*next == '$') {
					fputc('$', out);
			   } else {
					for (;isalpha(*(next + i)) && i < sizeof(var); i++) {
						 if (next + i > last + size) {
							  memmove(buf, next, i);
							  if ((size = fread(buf + i,
												sizeof(char),
												sizeof(buf) - i,
												in)) < sizeof(buf) - i)
								   todo = false;
							  goto restart;
						 }
					}
					memcpy(var, next, i);
					val = getenv(var);
					if (val) fputs(val, out);
					next += i;
					
					/* reset variable buffer */
					memset(var, 0, sizeof(var));
					i = 0;
			   }
		  }
	 }
}

void
process_skel(char *skel, char *new)
{
	 FILE *fo, *fi;

	 fi = fopen(skel, "r");
	 if (!fi) {
		  err(EXIT_FAILURE, "fopen");
	 }

	 if(!force && access(new, F_OK) != -1) {
		  fprintf(stderr, "output file '%s' already exists. overwrite with -f\n", new);
		  exit(EXIT_FAILURE);
	 }

	 fo = fopen(new, "w");
	 if (!fo) {
		  err(EXIT_FAILURE, "fopen");
	 }

	 process(fi, fo);

	 if (0 != fclose(fo)) {
		  err(EXIT_FAILURE, "fclose");
	 }
	 if (0 != fclose(fi)) {
		  err(EXIT_FAILURE, "fclose");
	 }
}
