# Simple SDL mini Makefile
#ImageProcessing/RLSA.c
#-Wall -Wextra -Werror

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS=  -Wall -Wextra -g -std=c99 -O3 -IImageProcessing/ -IImage_BMP/ -INeuralNetwork/
LDFLAGS=
EXEC=main
SRC= main.c ImageProcessing/BlackAndWhite.c Image_BMP/BMP.c Image_BMP/pixel_operations.c ImageProcessing/RLSA.c NeuralNetwork/NeuralCreate.c

LDLIBS= `pkg-config --libs sdl` -lSDL_image
OBJ= $(SRC:.c=.o)

all: $(EXEC)

main:$(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
