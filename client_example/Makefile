INC=-I./inc/

app1: CFLAGS=-D DEBUG=1 -lpthread
app2: CFLAGS=-D DEBUG=1 -lpthread
app3: CFLAGS=-D DEBUG=1 -lpthread
app4: CFLAGS=-D DEBUG=1 -lpthread
app5: CFLAGS=-D DEBUG=1 -lpthread

app1:
	mkdir -p ./bin
	gcc -c ./src/request.c -O3 -o ./bin/request.o $(INC)
	gcc ./bin/request.o ./app1.c -Wall -O3 -o ./bin/app1.o $(CFLAGS) $(INC)
app2:
	mkdir -p ./bin
	gcc -c ./src/request.c -O3 -o ./bin/request.o $(INC)
	gcc ./bin/request.o ./app2.c -Wall -O3 -o ./bin/app2.o $(CFLAGS) $(INC)
app3:
	mkdir -p ./bin
	gcc -c ./src/request.c -O3 -o ./bin/request.o $(INC)
	gcc ./bin/request.o ./app3.c -Wall -O3 -o ./bin/app3.o $(CFLAGS) $(INC)
app4:
	mkdir -p ./bin
	gcc -c ./src/request.c -O3 -o ./bin/request.o $(INC)
	gcc ./bin/request.o ./app4.c -Wall -O3 -o ./bin/app4.o $(CFLAGS) $(INC)
app5:
	mkdir -p ./bin
	gcc -c ./src/request.c -O3 -o ./bin/request.o $(INC)
	gcc ./bin/request.o ./app5.c -Wall -O3 -o ./bin/app5.o $(CFLAGS) $(INC)

run_app1:
	./bin/app1.o
run_app2:
	./bin/app2.o
run_app3:
	./bin/app3.o
run_app4:
	./bin/app4.o
run_app5:
	./bin/app5.o
