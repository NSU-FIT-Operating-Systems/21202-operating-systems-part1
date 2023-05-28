all:
	export PORT=12345
	gcc -Wall udp_server.c -o server
	gcc -Wall udp_client.c -o client
clean:
	rm server
	rm client
	unset PORT
