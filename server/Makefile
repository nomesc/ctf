INC=-I./inc/

debug: CFLAGS=-D DEBUG=1
release: CFLAGS=

debug:
	mkdir ./bin
	gcc -c ./src/request.c -O3 -o ./bin/request.o $(INC)
	gcc -c ./src/service_flag.c -O3 -o ./bin/service_flag.o $(INC)
	gcc -c ./src/service_population.c -O3 -o ./bin/service_population.o $(INC)
	gcc ./bin/request.o ./bin/service_flag.o ./bin/service_population.o ./main.c -Wall -O3 -o ./bin/server.o $(CFLAGS) $(INC) -lpthread
release:
	mkdir ./bin
	gcc -c ./src/request.c -O3 -o ./bin/request.o $(INC)
	gcc -c ./src/service_flag.c -O3 -o ./bin/service_flag.o $(INC)
	gcc -c ./src/service_population.c -O3 -o ./bin/service_population.o $(INC)
	gcc ./bin/request.o ./bin/service_flag.o ./bin/service_population.o ./main.c -Wall -O3 -o ./bin/server.o $(CFLAGS) $(INC)
clean:
	rm ./bin/request.o ./bin/service_flag.o ./bin/service_population.o ./bin/server.o
run:
	./bin/server.o