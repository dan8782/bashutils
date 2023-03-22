#include "s21_grep.h"

int main(int argc, char *argv[]) {
  int ch;
  const char *short_options = "e:ivclnhsf:";
  char pattern_e[1024] = {'\0'};
  char *mas_f[1024] = {NULL};
  int counte = 0, i = 0, countf = 0;
  while ((ch = getopt(argc, argv, short_options)) != -1) {
    switch (ch) {
      case 'e':
        counte++;
        strcat(pattern_e, optarg);
        strcat(pattern_e, "|");
        eflag = 1;
        break;
      case 'f':
        countf++;
        mas_f[i] = optarg;
        fflag = 1;
        break;
      case 'i':
        iflag = 1;
        break;
      case 'v':
        vflag = 1;
        break;
      case 'c':
        cflag = 1;
        break;
      case 'l':
        lflag = 1;
        break;
      case 'n':
        nflag = 1;
        break;
      case 'h':
        hflag = 1;
        break;
      case 's':
        hflag = 1;
        break;
      default:
        usage();
    }
    i++;
  }
  if (eflag || fflag) {
    argv += optind - 1;
  } else {
    argv += optind;
  }
  if (pattern_e[strlen(pattern_e) - 1] == '|') {
    pattern_e[strlen(pattern_e) - 1] = 0;
  }
  if (eflag) {
    argv[0] = pattern_e;
    scanfiles(argv);
  } else if (fflag) {
    compilepattern(mas_f, argv, countf);
  } else {
    scanfiles(argv);
  }
  return 0;
}

void compilepattern(char *mas_f[], char *argv[], int countf) {
  FILE *fp = {NULL};
  char line[1024] = {'\0'};
  char pattern_f[1024] = {'\0'};
  for (int i = 0; i < countf; i++) {
    int fd;
    if (mas_f[i] == NULL || strcmp(mas_f[i], "-") == 0) {
      fd = 0;
    } else {
      fd = open(mas_f[i], O_RDONLY);
    }
    fp = fdopen(fd, "r");
    if (fd < 0) {
      invalid_filename(mas_f[i]);
    } else if (fd == 0) {
      raw_grep();
    } else {
      while (fgets(line, sizeof(line), fp) != NULL) {
        if (strlen(line) == 1 && line[strlen(line) - 1] == '\n') {
          cat = 1;
        }
        if (line[strlen(line) - 1] == '\n') {
          line[strlen(line) - 1] = '|';
        }
        strcat(pattern_f, line);
      }
    }
    strcat(pattern_f, "|");
    fclose(fp);
  }
  if (pattern_f[strlen(pattern_f) - 1] == '|')
    pattern_f[strlen(pattern_f) - 1] = '\0';
  argv[0] = pattern_f;
  scanfiles(argv);
}

void scanfiles(char *argv[]) {
  FILE *fp = NULL;
  int i = 1;
  int countfile = 0;
  if (argv[0] == NULL && !sflag && !fflag) {
    usage();
  }
  int mto = (argv[2] == NULL) ? 0 : 1;
  while ((argv[i]) != NULL || i == 1) {
    int fd;
    if (argv[i] == NULL || strcmp(argv[i], "-") == 0) {
      fd = 0;
    } else {
      fd = open(argv[i], O_RDONLY);
    }
    if (fd < 0) {
      countfile++;
      invalid_filename(argv[i]);
    } else if (fd == 0) {
      raw_grep();
    } else {
      fp = fdopen(fd, "r");
      if (cat) {
        raw_cat(fp);
        break;
      }
      countfile++;
      byPattern(fp, argv, countfile, mto);
      fclose(fp);
    }
    i++;
  }
}

void byPattern(FILE *fp, char *fl[], int countfile, int mto) {
  regex_t regex;
  char line[1024] = {'\0'};
  int countline = 1, count_cur_file = 0, count = 0;
  int reg_option = iflag ? REG_ICASE : REG_EXTENDED;
  while (fgets(line, sizeof(line), fp)) {
    if (iflag && (fflag || eflag)) {
      regcomp(&regex, fl[0], REG_EXTENDED | REG_ICASE);
    } else {
      regcomp(&regex, fl[0], reg_option);
    }
    int return_value = regexec(&regex, line, 0, 0, 0);
    regfree(&regex);
    if (vflag) return_value = return_value ? 0 : 1;
    if (!return_value) {
      count_cur_file = 1;
      count++;
    }
    if (!return_value && !cflag && !lflag) {
      if (mto && !hflag) {
        printf("%s:", fl[countfile]);
      }
      if (nflag) {
        printf("%d:", countline);
      }
      printf("%s", line);
      if (line[strlen(line) - 1] != '\n') {
        printf("\n");
      }
    }
    countline++;
  }
  if (cflag) {
    if (mto && !hflag) {
      printf("%s:", fl[countfile]);
    }
    if (lflag && count) {
      count = 1;
    }
    printf("%d\n", count);
    count = 0;
  }
  if (lflag && count_cur_file == 1) {
    printf("%s\n", fl[countfile]);
    count_cur_file = 0;
  }
}

void raw_grep() {
  while (getc(stdin) != EOF)
    ;
}

void usage() {
  fprintf(stderr,
          "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
          "[-C[num]]\n[-e pattern] [-f file] [--binary-files=value] "
          "[--color=when]\n[--context[=num]] [--directories=action] [--label] "
          "[--line-buffered]\n[--null] [pattern] [file ...]\n");
  exit(1);
}

void invalid_filename(char *filename) {
  fprintf(stderr, "grep: %s: No such file or directory\n", filename);
}

void raw_cat(FILE *rfd) {
  int chr;
  while ((chr = getc(rfd)) != EOF) {
    fprintf(stdout, "%c", chr);
  }
}