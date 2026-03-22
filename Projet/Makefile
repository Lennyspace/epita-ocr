TARGET = main


CC = gcc
CFLAGS = -O3 -g -I/opt/homebrew/include `sdl2-config --cflags` `pkg-config --cflags gtk+-3.0`
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lm `pkg-config --libs gtk+-3.0`


SRC = main.c FichierC/to_black_and_white.c FichierC/additionnal_functions.c FichierC/net.c FichierCIA/cnn.c generateur_grille/generateur_image.c generateur_grille/generateur.c


all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)


clean:
	rm -f $(TARGET)


run: $(TARGET)
	./$(TARGET) <arguments>