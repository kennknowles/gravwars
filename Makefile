all:
	make -f Makefile.dep	LIB_DIR="-L/usr/local/lib -L/usr/X11R6/lib" \
							INCLUDE_DIR="-I/usr/local/include -I/usr/X11R6/include" \
							COMMON_LIBS="-lGL -lGLU -lglut -lX11 -lXmu -lXi -lpng" \
							CCFLAGS="-ansi -g" LOCAL=1
