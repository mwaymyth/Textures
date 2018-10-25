# HW6.c
EXE=HW6

# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall
LIBS=-lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
HW6.o: HW6.c CSCI4229.h
fatal.o: fatal.c CSCI4229.h
loadtexbmp.o: loadtexbmp.c CSCI4229.h
print.o: print.c CSCI4229.h
project.o: project.c CSCI4229.h
errcheck.o: errcheck.c CSCI4229.h
object.o: object.c CSCI4229.h

#  Create archive
CSCI4229.a:fatal.o loadtexbmp.o print.o project.o errcheck.o object.o
	ar -rcs $@ $^

# Compile rules
.c.o:
	gcc -c $(CFLG) $<
.cpp.o:
	g++ -c $(CFLG) $<

#  Link
HW6:HW6.o CSCI4229.a
	gcc -O3 -o $@ $^   $(LIBS)

#  Clean // $make clean
clean:
	$(CLEAN)
