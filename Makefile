SOURCES = analysis.cpp capture.cpp main.cpp presentation.cpp scroller.cpp  \
	handler.cpp drawable.cpp touchable.cpp camera.cpp application.cpp \
	confirmation.cpp moc_mainwindow.cpp mainwindow.cpp imageFrame.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJECTSFOLDER)/%.o)
OBJECTSFOLDER=objects
TARGET  = iwb

LIBCONFIG  = $(shell pkg-config --libs opencv QtGui QtCore)
INCLUDE    = $(shell pkg-config --cflags-only-I QtGui QtCore)
LDFLAGS    = $(LIBS) $(LIBCONFIG)

CC       = g++
MOC	 = moc
DEBUG    = -g
CFLAGS   = -c -Wall -Wextra $(INCLUDE)

$(TARGET):	$(OBJECTS)
	$(CC) $(LFLAGS) $(DEBUG) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

$(OBJECTSFOLDER)/%.o: %.cpp
	$(shell mkdir -p objects/)
	$(CC) $(CFLAGS) $(DEBUG) $< -o $@

moc_mainwindow.cpp:
	$(MOC) -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/include/QtCore -I/usr/include/QtGui -I/usr/include -I. include/mainwindow.h -o moc_mainwindow.cpp

clean:
	rm -rf $(TARGET) objects/ moc_mainwindow.cpp