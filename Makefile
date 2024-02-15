CC =g++
FLAGS = -lpthread

make: server.cpp
	$(CC) -I ../boost_1_84_0 server.cpp -o server $(FLAGS)

clean:
	rm server
