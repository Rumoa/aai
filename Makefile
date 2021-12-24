# Project: Attax Grafico
# Makefile created by Dev-C++ 5.11

CPP      = g++ -D__DEBUG__ -Wno-write-strings
CC       = gcc -D__DEBUG__ -Wno-write-strings
OBJ      = attax_graf.o 
LINKOBJ  = attax_graf.o
#LIBS     = -Lsdl-bgi/build -lSDL_bgi -lSDL2
LIBS     = -Lxbgi-364/src -lXbgi -lX11
INCS     = 
CXXINCS  = -I xbgi-364/src
BIN      = attax_graf
CXXFLAGS = $(CXXINCS) 
CFLAGS   = $(INCS) 
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)
	make -C xbgi-364/src clean

$(BIN): $(OBJ) xbgi-386/src/libXbgi.a
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

attax_graf.o: attax_graf.cpp
	$(CPP) -c attax_graf.cpp -o attax_graf.o $(CXXFLAGS) $(CXXINCS)

xbgi-386/src/libXbgi.a:
	make -C xbgi-364/src


