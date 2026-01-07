qc_compile qc_program: questioncreation.o
	@gcc -Wall -o qc_program questioncreation.o

questioncreation.o: questioncreation.c questioncreation.h
	@gcc -c questioncreation.c

qc_run: qc_program
	@./qc_program
