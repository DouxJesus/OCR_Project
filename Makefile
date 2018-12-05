#-Wall -Wextra -Werror

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD `pkg-config --cflags --libs gtk+-2.0`
CFLAGS=  -Wall -Wextra -g -std=c99 -O3 -IImageProcessing/ -IImage_BMP/ -INeuralNetwork/ -IInterface/
LDFLAGS=
EXEC=main
SRC= main.c ImageProcessing/BlackAndWhite.c Image_BMP/BMP.c Image_BMP/pixel_operations.c ImageProcessing/RLSA.c ImageProcessing/Queue.c  NeuralNetwork/NeuralCreate.c

LDLIBS= `pkg-config --libs sdl` -lSDL_image `pkg-config --cflags --libs gtk+-2.0`
OBJ= $(SRC:.c=.o)

all: $(EXEC)

main:$(OBJ)
	$(CC)  $(CFLAGS) $(CPPFLAGS) -o $@ $^ $(LDLIBS)

gtk:Interface/interface.o
	$(CC)  $(CFLAGS) $(CPPFLAGS) -o $@ $^ $(LDLIBS)
clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
