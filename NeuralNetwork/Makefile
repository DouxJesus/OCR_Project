# Simple SDL mini Makefile
#-Wall -Wextra -Werror

CC=gcc

CPPFLAGS= 
CFLAGS=-Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
EXEC=main
SRC= NeuralTrain.c NeuralCreate.c
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
