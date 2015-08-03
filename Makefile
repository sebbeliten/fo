foview_linux: main.c
	gcc -D ON_OS_LINUX -o foview main.c -lSDL -lSDL_image -lSDL_ttf
foview_windows: main.c
	gcc -D ON_OS_WINDOWS -o foview main.c -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_ttf
clean:
	rm foview
