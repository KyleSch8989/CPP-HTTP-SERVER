CC =g++

make: server.cpp
	$(CC) server.cpp -o server

clean:
	rm server
