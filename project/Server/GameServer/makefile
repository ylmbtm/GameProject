CC=g++
CC_INCLUDE=-I/usr/include
	 
MY_INCLUDE=-I$(PWD) \
		-I../ServerEngine \
					 
CC_LIB=-L/usr/lib \
	-L/lib \
	-lpthread
			 
MY_LIB=-L../Debug \
	-lServerEngine.a

TARGET=../Debug/GameServer

OBJPATH=

SRCDIRS= .\

SRCS=$(wildcard *.cpp)

OBJS=$(SRCS:.cpp=.o)

$(TARGET):$(OBJS)
	$(CC) -g -o $(TARGET) $(OBJS) $(CC_LIB) $(MY_LIB)

$(OBJS):%.o:%.cpp
	$(CC) -g -w -c $< -o $@ $(CC_INCLUDE) $(MY_INCLUDE)
	
clean:
	rm $(OBJS) $(TARGET)