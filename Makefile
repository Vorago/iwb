SOURCES = analysis.cpp capture.cpp main.cpp presentation.cpp scroller.cpp handler.cpp
HEADERS = include/analysis.hpp include/capture.hpp include/presentation.hpp include/scroller.hpp include/handler.hpp
TARGET  = iwb

ARCH = $(shell uname -m)
ifeq ("$(ARCH)", "x86_64")
	ARCH = amd64
else
	ARCH = i386
endif

CC     = g++
DEBUG  = -g
CFLAGS = -Wall -Wextra -I/usr/include/opencv \
-I/usr/lib/jvm/java-6-openjdk/include \
-I/usr/lib/jvm/java-6-openjdk/include/linux \
-I/usr/lib/jvm/java-6-openjdk/jre/lib/$(ARCH)/server \
-L/usr/lib/jvm/java-6-openjdk/jre/lib/$(ARCH)/server 

LIBS      =
LIBCONFIG = $(shell pkg-config --libs opencv)
LFLAGS    = $(LIBS) $(LIBCONFIG)

all :
	$(CC) $(LFLAGS) $(CFLAGS) $(SOURCES) $(HEADERS) -o $(TARGET)

clean:
	\rm $(TARGET)
