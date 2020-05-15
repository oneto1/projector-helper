#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "prase.h"
#include "log.h"

// ₀ ₁ ₂ ₃ ₄ ₅ ₆ ₇ ₈ ₉
#define VERSION "₁."

#define MAX_FUNCTION 6

int LOGTOFILE ;

// #define EXIT_BYE()       \
//     {                    \
//         printf("bye !"); \
//         return 0;        \
//     }

//main function  一一对应函数名.c
extern int ping_alive(void);
extern void get_ip(void);
extern void shutdowm_projector(struct ip_address *);
extern int startup_projector(struct ip_address *);
extern int get_lamptime(struct ip_address *);

void show_usage(void);
void run_function(struct hp *hp_p);

int main(int argc , char *argv[])
{
    struct hp hp_t;
    memset(&hp_t,0,sizeof(struct hp));
    
    prase_main_entry(argc,argv,&hp_t);

    run_function(&hp_t);

    return 0;
}

void show_usage()
{
    //printf("\ec");
    //printf("welcome to projector helper  %s\n", VERSION);
    printf("Usage                                               \n");
    printf("    projector-helper [options] <host>               \n");
    printf("Options:                                            \n");
    printf("    <host> a host ip address or a CIDR address      \n");
    printf("    -p ping a host                                  \n");
    printf("    -g get local ip address                         \n");  
    printf("    -s start up projector                           \n");
    printf("    -r shutdown projector                           \n");
    printf("    -t get the lamp time                            \n");
    printf("    -h show usage                                   \n");
    //printf("Hints: if [options] is NULL then default option is -p\n");
}

void run_function(struct hp *hp_p){

        if(hp_p->fun_t.shut_up == 1){
            shutdowm_projector(&(hp_p->ip_t));
        }
    
        if(hp_p->fun_t.start_up == 1)
            startup_projector(&(hp_p->ip_t));
        
        if(hp_p->fun_t.get_ip == 1){
              get_ip();
        }
          
        if(hp_p->fun_t.get_time == 1)
            get_lamptime(&(hp_p->ip_t));

        if(hp_p->fun_t.usage == 1)
            show_usage();

}
