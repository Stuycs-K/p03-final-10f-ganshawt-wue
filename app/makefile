qc_compile qc_program: questioncreation.o
	gcc -Wall -o qc_program questioncreation.o

qc_run: qc_program
	@./qc_program

compile: server client

server: server.o networking.o questioncreation.o
	gcc -o server server.o networking.o questioncreation.o

client: client.o networking.o
	gcc -o client client.o networking.o

server.o: server.c networking.h questioncreation.h
	gcc -c -Wno-format-truncation server.c

client.o: client.c networking.h
	gcc -c client.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm -f *.o server client qc_program .server_ip
