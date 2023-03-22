TARGET_GREP = s21_grep

SRC_GREP = s21_grep.c

CFLAGS =  -Wall -Wextra -Werror -std=c11 -D_GNU_SOURCE

.PHONY: all clean s21_grep cat rebuild check memcheck test build grep_mem_test

all: build

build:  s21_grep

clean:
	rm -rf *.out  s21_grep

s21_grep:
	gcc $(SRC_GREP) $(CFLAGS) -o $(TARGET_GREP)

grep_mem_test: s21_grep
	leaks -atExit -- ./$(TARGET_GREP)