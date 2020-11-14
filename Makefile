INC=-I./inc/

debug: CFLAGS=-D DEBUG=1
release: CFLAGS=

debug:
	gcc ./main.c -Wall -O3 -o ./country-info $(CFLAGS) $(INC)
release:
	gcc ./main.c -Wall -O3 -o ./country-info $(CFLAGS) $(INC)