DBFLAG = -DDEBUG=1
CXX = g++
CPPFLAGS = -g -O0 -fPIC $(DBFLAG)
LDFLAGS = -g `pkg-config --libs allegro`

INTERPRETER = lua-allegro

$(INTERPRETER): interpreter.o wrapper.o
	gcc -o $(INTERPRETER) interpreter.o wrapper.o -lm -Wl,-E -ldl -llua5.3 $(LDFLAGS)
	
interpreter.o: interpreter.c
	gcc -std=gnu99 -g -O0 -Wall -Wextra -DLUA_COMPAT_5_2 -DLUA_USE_LINUX -c -o interpreter.o interpreter.c -I../lua-5.3.4/src/

wrapper.o: wrapper.c
	gcc -std=gnu99 -g -O0 -Wall -Wextra -c -o wrapper.o wrapper.c -I../lua-5.3.4/src/

clean:
	(rm -f *.o $(LIBRARY))

