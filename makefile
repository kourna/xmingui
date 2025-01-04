main:
	g++ -o app main.cpp components/gui.h components/layout.h components/drawutils.h components/deserializer.h -lX11 -Wall
