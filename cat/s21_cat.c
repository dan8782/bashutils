#include "s21_cat.h"

int main(int argc, char *argv[]) {
  int ch;
  const char *short_options = "benstv";
  while ((ch = getopt_long(argc, argv, short_options, long_options, 0)) != -1) {
    switch (ch) {
      case 'b':
        bflag = nflag = 1;
        break;
      case 'e':
        eflag = vflag = 1;
        break;
      case 'E':
        eflag = 1;
        break;
      case 'n':
        nflag = 1;
        break;
      case 's':
        sflag = 1;
        break;
      case 't':
        tflag = vflag = 1;
        break;
      case 'T':
        tflag = 1;
        break;
      case 'v':
        vflag = 1;
        break;
      default:
        fprintf(stderr, "usage: cat [-benstv] [file ...]\n");
        exit(1);
    }
  }
  argv += optind;
  if (bflag || eflag || nflag || sflag || tflag || vflag)
    openfiles(argv, 1);
  else
    openfiles(argv, 0);
}

void openfiles(char *argv[], int flagstrue) {
  FILE *fp = NULL;
  int i = 0;
  while ((argv[i]) != NULL || i == 0) {
    int fd;
    if (argv[i] == NULL || strcmp(argv[i], "-") == 0) {
      fd = 0;
    } else {
      fd = open(argv[i], O_RDONLY);
    }
    if (fd < 0) {
      warn("%s", argv[i]);
    } else if (flagstrue) {
      fp = fdopen(fd, "r");
      flags_cat(fp);
      fclose(fp);
    } else {
      fp = fdopen(fd, "r");
      cat(fp);
      fclose(fp);
    }
    i++;
  }
}

void flags_cat(FILE *fp) {
  int ch, count_s, line = 1, previos;
  for (previos = '\n'; (ch = getc(fp)) != EOF; previos = ch) {
    if (previos == '\n') {
      if (sflag) {
        if (ch == '\n') {
          if (count_s == 1) {
            continue;
          }
          count_s = 1;
        } else
          count_s = 0;
      }
      if (nflag && (!bflag || ch != '\n')) {
        printf("%6d\t", line++);
      }
    }
    if (ch == '\n') {
      if (eflag) {
        putchar('$');
        if (ch == EOF) {
          break;
        }
      }

    } else if (ch == '\t') {
      if (tflag) {
        putchar('^');
        if (putchar('I') == EOF) {
          break;
        }
        continue;
      }
    } else if (vflag) {
      if (!isascii(ch) && !isprint(ch)) {
        putchar('M');
        putchar('-');
        if ('M' == EOF || '-' == EOF) {
          break;
        }
      }
      if (iscntrl(ch)) {
        if (putchar('^') == EOF || putchar(ch == 127 ? '?' : ch + 64) == EOF) {
          break;
        }
        continue;
      }
    }
    putchar(ch);
    if (ch == EOF) {
      break;
    }
  }
}

void cat(FILE *rfd) {
  int chr;
  while ((chr = getc(rfd)) != EOF) {
    fprintf(stdout, "%c", chr);
  }
}
