OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -std=c++11

OUT_DIR=bin
BIN=$(OUT_DIR)/a1model
SRC_DIR=src
# Yes I know there are better ways to do this. Short on time.
SRCS=$(SRC_DIR)/point.cpp $(SRC_DIR)/triangle.cpp $(SRC_DIR)/model.cpp $(SRC_DIR)/main.cpp $(SRC_DIR)/gl_framework.cpp $(SRC_DIR)/shader_util.cpp
INCLUDES=$(SRC_DIR)/gl_framework.hpp $(SRC_DIR)/shader_util.hpp $(SRC_DIR)/point.hpp $(SRC_DIR)/triangle.hpp $(SRC_DIR)/model.hpp $(SRC_DIR)/main.hpp

all: $(BIN)

$(BIN): $(SRCS) $(INCLUDES)
	mkdir -p $(OUT_DIR)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN) $(OUT_DIR)/*~ $(OUT_DIR)/*.o
