run: main.cpp
	clang++ -std=c++20 main.cpp -o run	&& ./run

server: server.cpp 
	clang++ -std=c++20 server.cpp -o server	&& ./server

client: client.cpp
	clang++ -std=c++20 client.cpp -o client	&& ./client

clean: 
	rm server client 