#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ₀ ₁ ₂ ₃ ₄ ₅ ₆ ₇ ₈ ₉
#define VERSION "₁.₀"

//由于暂时缺少原始抓包数据,未能实现
#define get_lamptime()                                        \
    printf("×  This is a furture function ,not work yet × "); \
    printf("\nplease enter ENTER to return menu ");           \
    getchar();                                                \
    while (getchar() != '\n')                                 \
        ;

#define EXIT_BYE()       \
    {                    \
        printf("bye !"); \
        return 0;        \
    }

//main function  一一对应函数名.c
extern int ping_alive(void);
extern void get_ip(void);
extern void shutdowm_projector(void);
extern int startup_projector(void);
//extern int get_lamptime(void );

void show_menu(void);
void exit_error(char *);

int main()
{
    while (1)
    {

        show_menu();

        int choicenum = 0;

        if (scanf("%d", &choicenum) == 1)

            switch (choicenum)
            {
            case 1:
                get_ip();
                break;
            case 2:
                startup_projector();
                break;
            case 3:
                shutdowm_projector();
                break;
            case 4:
                ping_alive();
                break;
            case 5:
                get_lamptime();
                break;
            case 6:
                EXIT_BYE(); //宏函数
            default:
                printf("invaild num,please try again !\n");
                sleep(1);
            }
        else
        {
            printf("invaild num,please try again !\n");
            //防止执行太快
            sleep(1);

            //刷新缓存区
            getchar();
        }
    }

    return 0;
}

void show_menu()
{
    printf("\ec");
    printf("\t\twelcome to projector helper  %s\n", VERSION);
    printf("#**************************************************************#\n");
    printf("#    1.get local ip address				       #\n");
    printf("#    2.start up projector				       #\n");
    printf("#    3.shutdown projector				       #\n");
    printf("#    4.ping a projector or more projectors		       #\n");
    printf("#    5.get the lamp time				       #\n");
    printf("#    6.exit and goodbye					       #\n");
    printf("#**************************************************************#\n");
    printf("please chose a funtion to run: ");
}

void exit_error(char *p)
{
    perror(p);
    exit(1);
}
