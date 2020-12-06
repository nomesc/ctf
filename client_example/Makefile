INC=-I./inc/

app1: CFLAGS=-D DEBUG=1 -lpthread
app2: CFLAGS=-D DEBUG=1 -lpthread

app1:
	mkdir -p ./bin
	gcc -c ./src/request.c -O3 -o ./bin/request.o $(INC)
	gcc ./bin/request.o ./app1.c -Wall -O3 -o ./bin/app1.o $(CFLAGS) $(INC)
app2:
	mkdir -p ./bin
	gcc -c ./src/request.c -O3 -o ./bin/request.o $(INC)
	gcc ./bin/request.o ./app2.c -Wall -O3 -o ./bin/app2.o $(CFLAGS) $(INC)
run_app1:
	./bin/app1.o
run_app2:
	./bin/app2.o