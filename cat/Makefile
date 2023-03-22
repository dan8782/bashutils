TARGET_CAT = s21_cat

SRC_CAT = s21_cat.c

CFLAGS = -Wall -Wextra -Werror -std=c11 -D_GNU_SOURCE

.PHONY: all clean s21_cat cat rebuild check memcheck test build

all: build

build:  s21_cat

clean:
	rm -rf *.out s21_cat

s21_cat: 
	gcc $(SRC_CAT) $(CFLAGS) -o $(TARGET_CAT)
