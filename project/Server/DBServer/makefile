CC=g++
CC_INCLUDE=-I/usr/include
	 
MY_INCLUDE=-I$(PWD) \
		-I../ServerEngine \
		-I../../Common \
		-I../../Common/Utility \
		-I../../Common/PacketDef \
		-I../../Common/DataBuff \
		-I../../Common/CmdHandler \
		-I../../Common/Mysql
					 
CC_LIB=-L/usr/lib \
	-L/lib \
	-lpthread
			 
MY_LIB=-L../Debug \
	-lServerEngine

TARGET=../Linux/DBServer

SRCS=$(wildcard *.cpp)

OBJS=$(SRCS:.cpp=.o)

$(TARGET):$(OBJS)
	$(CC) -g -o $(TARGET) $(OBJS) $(CC_LIB) $(MY_LIB)
	mv $(OBJS) ../Linux

$(OBJS):%.o:%.cpp
	$(CC) -g -w -c $< -o $@ $(CC_INCLUDE) $(MY_INCLUDE)
	
clean:
	rm $(OBJS) $(TARGET)