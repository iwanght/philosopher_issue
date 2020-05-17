CC ?= gcc
all: kaifan
	./kaifan 5
kaifan: main.c chopstick.c philosopher.c philosopher
	$(CC) main.c chopstick.c philosopher.c -lpthread -o kaifan
philosopher: chopstick.c philosopher_main.c
	$(CC) chopstick.c philosopher_main.c -lpthread -o philosopher
clean:
	rm kaifan philosopher 

