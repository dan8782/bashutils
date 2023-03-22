#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bflag, eflag, nflag, sflag, tflag, vflag;

const struct option long_options[] = {{"number", no_argument, NULL, 'n'},
                                      {"squeeze-blank", no_argument, NULL, 's'},
                                      {"T", no_argument, NULL, 'T'},
                                      {"E", no_argument, NULL, 'E'},
                                      {NULL, 0, NULL, 0}};

void openfiles(char *argv[], int flagstrue);
void flags_cat(FILE *);
void cat(FILE *);