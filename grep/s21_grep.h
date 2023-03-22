#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int eflag, iflag, vflag, cflag, lflag, nflag, hflag, sflag, fflag, cat;

void scanfiles(char *argv[]);
void scanfiles_ef(char *argv[], char dest[], int countind);
void raw_grep();
void usage();
void byPattern(FILE *fp, char *fl[], int countfile, int mto);
void invalid_filename(char *filename);
void pattern_ef(char *argv[], char dest[1024], int mto);
void compilepattern(char *mas_f[], char *argv[], int countf);
void raw_cat(FILE *);