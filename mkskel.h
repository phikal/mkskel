/* (MKSKEL) mkskel.h: common functions
 *
 * This file is in the public domain, to the extent possible under law,
 * published under the CC0 1.0 Universal license.
 *
 * For a full copy of the CC0 license see
 * https://creativecommons.org/publicdomain/zero/1.0/legalcode */

#ifndef DEFAUT_PATH_VAR
#define DEFAUT_PATH_VAR "SKELPATH"
#endif

#ifndef DEFAULT_PATH
#define DEFAULT_PATH "/usr/share/skel"
#endif

void create_skel(char *, char *);
void process_skel(char *, char *);
void list_skel(void);
