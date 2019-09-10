SRCS	= *.cpp
TARGET 	= client
CXX_FLAGS = -std=c++11 -g

$(TARGET) : $(SRCS)
	g++ -o $@ $^ $(CXX_FLAGS)

clean :
	rm $(TARGET) -f
