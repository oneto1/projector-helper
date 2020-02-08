#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void get_ip(void )
{
    // sed 自身的()、{}需要一次转义，ｃ语言中 \ 自身也需要一次转义
    char cmd[] = " sh -c  ifconfig |grep 'inet' | sed '/inet6/d'| sed 's/netmask.*/ /g' |sed 's/\\(.\\{8\\}\\)\\(.*\\)/\\2/' |  sed '$!N;s/\\n/\\t/' |  sed '$!N;s/\\n/\\t/' " ;

    system(cmd);

    printf("please enter ENTER to return menu ");

    getchar();
    while(getchar() == '\n')
        return ;

}
