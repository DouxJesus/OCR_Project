# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -IImageProcessing/ -IImage_BMP/ 
LDFLAGS=
EXEC=main
SRC= main.c ImageProcessing/BlackAndWhite.c ImageProcessing/RSLA.c Image_BMP/BMP.c Image_BMP/pixel_operations.c ImageProcessing/PixelProcessing.c
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
