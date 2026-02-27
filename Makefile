CXX = g++
CXXFLAGS = -Wall -Werror -std=c++17

SRC = src/main.cpp src/Request.cpp src/WebServer.cpp src/LoadBalancer.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = build/loadbalancer

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)