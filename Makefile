# Linux makefile for CS 354 skeleton code

CC= g++
CFLAGS+= -O2
LIBS= -lXext -lX11 -lGL -lGLU -lglut
PROD= viewer

HDRS= $(shell ls *.h)
SRCS= $(shell ls *.cpp)
OBJS= $(patsubst %.cpp, %.o, $(SRCS))

LINUX_LIBS= -lXext -lX11 -lGL -lGLU -lglut
MAC_LIBS= -framework GLUT -framework OpenGL
LIBS=$(shell [ -e /Volumes/Macintosh\ HD ] && echo $(MAC_LIBS) || echo $(LINUX_LIBS))
CFLAGS+=$(shell [ -e /Volumes/Macintosh\ HD ] && echo -DMACINTOSH -Wno-deprecated-declarations)

all: $(PROD)

$(OBJS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $< -o $@

$(PROD): $(OBJS)
	$(CC) -o $(PROD) $^ $(LIBS)

clean:
	rm -f $(PROD)
	rm -f *.o

