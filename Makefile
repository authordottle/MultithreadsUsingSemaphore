all:
	gcc -o main main.c producer.c consumer.c -lrt -lpthread
