INC=-I./inc/

debug: CFLAGS=-D DEBUG=1 -lpthread
release: CFLAGS=-lpthread

debug:
	mkdir -p ./bin
	gcc -c ./src/request.c -o ./bin/request.o $(INC)
	gcc -c ./src/service_flag.c -o ./bin/service_flag.o $(INC)
	gcc -c ./src/service_population.c -o ./bin/service_population.o $(INC)
	gcc -c ./src/service_add_population.c -o ./bin/service_add_population.o $(INC)
	gcc -c ./src/service_see_reviews.c -o ./bin/service_see_reviews.o $(INC)
	gcc -c ./src/service_give_feedback.c -o ./bin/service_give_feedback.o $(INC)
	gcc ./bin/request.o ./bin/service_give_feedback.o ./bin/service_see_reviews.o ./bin/service_flag.o ./bin/service_population.o ./bin/service_add_population.o ./main.c -Wall -o ./bin/server.o $(CFLAGS) $(INC)
release:
	mkdir -p ./bin
	gcc -c ./src/request.c -o ./bin/request.o $(INC)
	gcc -c ./src/service_flag.c -o ./bin/service_flag.o $(INC)
	gcc -c ./src/service_population.c -o ./bin/service_population.o $(INC)
	gcc -c ./src/service_add_population.c -o ./bin/service_add_population.o $(INC)
	gcc -c ./src/service_see_reviews.c -o ./bin/service_see_reviews.o $(INC)
	gcc -c ./src/service_give_feedback.c -o ./bin/service_give_feedback.o $(INC)
	gcc ./bin/request.o ./bin/service_give_feedback.o ./bin/service_see_reviews.o ./bin/service_flag.o ./bin/service_population.o ./bin/service_add_population.o ./main.c -Wall -o ./bin/server.o $(CFLAGS) $(INC)
clean:
	rm ./bin/request.o ./bin/service_flag.o ./bin/service_population.o ./bin/server.o
run:
	./bin/server.o