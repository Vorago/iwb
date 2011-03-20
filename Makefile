SOURCES = analysis.cpp capture.cpp main.cpp presentation.cpp scroller.cpp handler.cpp drawable.cpp touchable.cpp camera.cpp application.cpp confirmation.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJECTSFOLDER)/%.o)
OBJECTSFOLDER=objects
TARGET  = iwb

CC       = g++
DEBUG    = -g
CFLAGS   = -c -Wall -Wextra

LIBS	   =  
LIBCONFIG  = $(shell pkg-config --libs opencv)
LDFLAGS    = $(LIBS) $(LIBCONFIG)

all :	$(OBJECTS)
	$(CC) $(LFLAGS) $(DEBUG) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

$(OBJECTSFOLDER)/%.o: %.cpp
	$(shell mkdir -p objects/)
	$(CC) $(CFLAGS) $(DEBUG) $< -o $@

clean:
	rm -rf $(TARGET) objects/
