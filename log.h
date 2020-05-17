#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<fcntl.h>


#define EXIT_ERROR(p)                                                               \
        do{                                                                         \
            perror(p);                                                              \
            exit(1);                                                                \
        }while(0)      


#define TIME_FORMAT "%F %T"

extern int LOGTOFILE;

#define LOG(format, ...)                                                            \
        do{                                                                         \
            time_t now = time(NULL);                                                \
            char timestr[20] = {0};                                                 \
            strftime(timestr, 20,"%F %T", localtime(&now));                         \
            if(LOGTOFILE){                                                          \
                int fd = creat(timestr,S_IRWXU);                                    \
                if(fd < 0)                                                          \
                    EXIT_ERROR("create log error!");                                \
                else{                                                               \
                    dprintf(fd," %s LOG:" format "\n", timestr,##__VA_ARGS__);      \
                }                                                                   \
            }else{                                                                  \
                fprintf(stdout, " %s LOG:" format "\n", timestr,##__VA_ARGS__);     \
            }                                                                       \
        }while(0)

                                                             


