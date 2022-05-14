CC=gcc

RM=rm


TOPTIONS = -Wall -march=native -mtune=native -Ofast  -g3

all : branch bruteforce

branch:
	gcc $(TOPTIONS) branch.c -o branch

bruteforce:
	gcc $(TOPTIONS) bruteforce.c -o bruteforce

clean:
	$(RM) branch bruteforce
	
run:
	./branch -n 12 
	./bruteforce -n 12
