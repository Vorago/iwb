SOURCES = analysis.cpp capture.cpp main.cpp presentation.cpp scroller.cpp handler.cpp drawable.cpp touchable.cpp camera.cpp application.cpp confirmation.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJECTSFOLDER)/%.o)
OBJECTSFOLDER=objects
TARGET  = iwb

LIBS	   =  
LIBCONFIG  = $(shell pkg-config --libs opencv gdk-2.0 atk)
INCLUDE    = $(shell pkg-config --cflags-only-I opencv gdk-2.0 atk) 
LDFLAGS    = $(LIBS) $(LIBCONFIG)

CC       = g++
DEBUG    = -g
CFLAGS   = -c -Wall -Wextra $(INCLUDE)

all :	$(OBJECTS)
	$(CC) $(LFLAGS) $(DEBUG) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

$(OBJECTSFOLDER)/%.o: %.cpp
	$(shell mkdir -p objects/)
	$(CC) $(CFLAGS) $(DEBUG) $< -o $@

clean:
	rm -rf $(TARGET) objects/
