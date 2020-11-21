INC=-I./inc/

debug: CFLAGS=-D DEBUG=1
release: CFLAGS=

debug:
	gcc -c ./src/request.c $(INC)
	gcc -c ./src/service_flag.c $(INC)
	gcc -c ./src/service_population.c $(INC)
	gcc ./request.o ./service_flag.o ./service_population.o ./main.c -Wall -O3 -o ./country-info $(CFLAGS) $(INC)
release:
	gcc -c ./src/request.c $(INC)
	gcc -c ./src/service_flag.c $(INC)
	gcc -c ./src/service_population.c $(INC)
	gcc ./main.c -Wall -O3 -o ./country-info $(CFLAGS) $(INC)