# Simple SDL mini Makefile
#ImageProcessing/RLSA.c
#-Wall -Wextra -Werror

CC=gcc

CPPFLAGS= 
CFLAGS=-Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
EXEC=main
SRC= main.c learning.c
LDLIBS= -lm
OBJ= $(SRC:.c=.o)

all: $(EXEC)

main:$(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
