# Sample program to illustrate Liquid War 6 bug #24324 on Savannah
# https://savannah.gnu.org/bugs/?24324
# Christian Mauduit <ufoot@ufoot.org>

all: main lib.so

main.o: main.c
	gcc -g -c -o $@ $<

main: main.o
	gcc -lltdl -o $@ $<

lib.o: lib.c
	gcc -fPIC -g -c -o $@ $<

lib.so: lib.o
	gcc -lSDL -shared -Wl -o $@ $<

clean:
	rm -f main lib.so *.o

