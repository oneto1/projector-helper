main: main.o ping_alive.o get_ip.o shutdowm_projector.o startup_projector.o get_lamptime.o  ping-helper
	gcc -g -o projector-helper ping_alive.o main.o get_ip.o  shutdowm_projector.o startup_projector.o  get_lamptime.o -fstack-protector -fstack-protector-all

ping.o: ping.c
	echo " 发送raw 套接字需要root权限 "
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

