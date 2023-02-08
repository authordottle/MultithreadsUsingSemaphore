all:
	gcc -o test main.c producer.c consumer.c -lrt -lpthread
