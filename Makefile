all:
	g++ -o run.out binpatch.cpp Tiny/tinyfiledialogs.c -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
