CXX = g++
CXXFLAGS = -std=c++0x
#OBJS = f.o g.o

SRCS = tsp.cpp

tsp: ${SRCS} 
	${CXX} ${CXXFLAGS} ${SRCS} -o tsp
