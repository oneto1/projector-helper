#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

extern void exit_error(char *);

int ping_alive(void)
{
    char buf[50] = {0};

	printf("please input a ip address :");

	if(scanf("%s",buf) < 0)
		exit_error("input error ");

    int ret = fork();

    if(ret){
        waitpid(ret,NULL,0);
		printf("please enter ENTER to return menu ");
		getchar();
		while(getchar() == '\n')
			return 0;
    }else{
        char *exec_argv[3];
        char cmd[100] ;

		//获取当前绝对路径
		getcwd(cmd,sizeof(cmd));

        // 拼接完整路径
		// ping-helper为ping.c单独编译出来的		
		// 具体使用方法   ping-helper [ip address]  
		char p[] = "/ping-helper";
		memcpy(cmd+strlen(cmd),p,sizeof(p));

		exec_argv[0] =cmd ;
        exec_argv[1] = buf;
		exec_argv[2] = (char *)0;

        if(execv(cmd,exec_argv)  == -1)
			exit_error("exec error ");

    }

    return 0;

}
