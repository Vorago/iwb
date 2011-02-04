SOURCES = analysis.cpp capture.cpp main.cpp presentation.cpp
HEADERS = include/analysis.hpp include/capture.hpp include/presentation.hpp
TARGET  = iwb

CC     = g++
DEBUG  = -g
CFLAGS = -Wall -Wextra -I/usr/include/opencv \
-I/usr/lib/jvm/java-6-openjdk/include \
-I/usr/lib/jvm/java-6-openjdk/include/linux \
-I/usr/lib/jvm/java-6-openjdk/jre/lib/i386/server \
-L/usr/lib/jvm/java-6-openjdk/jre/lib/i386/server \
-ljvm

LIBS      =
LIBCONFIG = $(shell pkg-config --libs opencv)
LFLAGS    = $(LIBS) $(LIBCONFIG)

all :
	$(CC) $(LFLAGS) $(CFLAGS) $(SOURCES) $(HEADERS) -o $(TARGET)

clean:
	\rm $(TARGET)
