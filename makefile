compiler: driver.o
	gcc -o compiler driver.o

driver.o: driver.c
	gcc -c driver.c

clean: 
	rm -rf *.o