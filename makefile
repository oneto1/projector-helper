OBJ=main.o startup_projector.o shutdowm_projector.o prase.o get_ip.o get_lamptime.o
CFLAGS= -O -fstack-protector -fstack-protector-all

main: $(OBJ)
	gcc -o projector-helper $(OBJ) $(CFLAGS)

prase.o: prase.c
	gcc -c prase.c

ping.o: ping.c
	sudo gcc -c ping.c

ping-helper: ping.o
	sudo gcc -o ping-helper ping.o
	sudo chmod +s ping-helper

main.o: main.c
	gcc -c main.c

get_ip.o: get_ip.c
	gcc -c get_ip.c

ping_main.o: ping_alive.c
	gcc -c ping_alive.c

shutdowm_projector.o: shutdowm_projector.c
	gcc -c shutdowm_projector.c

startup_projector.o: startup_projector.c
	gcc -c startup_projector.c

get_lamptime.o: get_lamptime.c
	gcc -c get_lamptime.c

clean: 
	rm *.o 

