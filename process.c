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
	 char /* static buffer with file contents + offset for variable */
		  buf[BUFSIZ + DEFAULT_VAR_LENGTH],
		  /* dynamic partition of buffer `buf' currently being
		   * processed */
		  *next,
		  /* previous value of `next' */
		  *last;

	 size_t /* number of currently valid characters * in `buf' */
		  size,
		  /* auxiliary variable */
		  i = 0;

	 /* read file buffer-by-buffer until nothing is being read in
	  * anymore. */
	 while ((size = fread(next = buf, sizeof(char), BUFSIZ, in)) > 0) {

		  /* with file in memory, attempt to process all $-signs */
		  for (;;) {
			   last = next;

			   /* attempt to find next $-sign */
			   next = memchr(next, '$', size - (size_t) (next - buf));

			   /* check if $-sign found */
			   if (!next) {
					/* if none could be found, print entire buffer */
					fwrite(last, size - (size_t) (last - buf), sizeof(char), out);
					break;
			   }

			   /* write out buffer until $-sign */
			   fwrite(last, (size_t) (next - last), sizeof(char), out);

			   /* skip $-sign */
			   next++;

			   /* check if have reached end of buffer */
			   if (next >= buf + size) {
					clearerr(in);
					size += fread(buf + BUFSIZ, sizeof(char), DEFAULT_VAR_LENGTH, in);
					if (ferror(in)) err(EXIT_FAILURE, "fread");
			   }

			   /* handle next char after $-sign */
			   switch (*next) {
			   case '$':		/* LITERAL $-SIGN */
					fputc('$', out);
					next++;
					break;
			   default:			/* ENVIRONMENTAL VARIABLE */
					/* seek forward until first non alnum || '_' char
					 * could be found OR maximal variable length has
					 * been reached. */
					for (i = 0; (isalnum(*(next + i)) || *(next + 1) == '_') &&
							  i < DEFAULT_VAR_LENGTH; i++) {
						 /* check if valid buffer has been overflown */
						 if (next + i > buf + size) {
							  /* if necessary, read in additional
							   * characters */
							  clearerr(in);
							  size += fread(buf + BUFSIZ, sizeof(char), DEFAULT_VAR_LENGTH, in);
							  if (ferror(in)) err(EXIT_FAILURE, "fread");

							  /* stop reading if EOF reached */
							  if (feof(in)) break;
						 }
					}

					/* find and output variable */
					char var[DEFAULT_VAR_LENGTH + 1] = {0}, *val;
					strncpy(var, next, i);
					val = getenv(var);
					if (!val) fprintf(stderr, "warning: variable '%s' is empty.\n", var);
					else fputs(val, out);

					/* skip over variable */
					next += i;
			   }
		  }
	 }

	 if (ferror(out)) err(EXIT_FAILURE, "fread");
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
		  return;
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
