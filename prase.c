#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include "prase.h"
#include "log.h"

int prase_main_entry(int argc,char *argv[],struct hp *hp_p){

    if(argc < 2 || argc > 3){
        hp_p->fun_t.usage = 1;
        return -1;
    }

    int ret = prase_function(argc,argv,(hp_p));

    if(ret < 0){
        EXIT_ERROR("prase error");
    }

    return 0;
}

int prase_function(int argc , char *argv[] , struct hp *hp_p){
    
    char *optshring = "p:gs:r:t:h";

    int ret = 0;

    while((ret = getopt(argc,argv,optshring)) != -1){
        switch (ret)
        {
        case 'p':
            hp_p->fun_t.ping = 1;
            prase_ip(optarg,&(hp_p->ip_t));
            break;
        case 'g':
            hp_p->fun_t.get_ip = 1;
            break;
        case 's':
            hp_p->fun_t.start_up = 1;
            prase_ip(optarg,&(hp_p->ip_t));
            break;
        case 'r':
            hp_p->fun_t.shut_up = 1;
            prase_ip(optarg,&(hp_p->ip_t));
            break;
        case 't':
            hp_p->fun_t.get_time = 1;
            prase_ip(optarg,&(hp_p->ip_t));
            break;
        default:
            hp_p->fun_t.usage = 1;
            break;
        }
    }



    return 0;
}

int prase_ip(char * ip,struct ip_address *ip_p){
    
    int CIDR_addr = 1;
    
    if( NULL == strstr(ip,"/") ){
        CIDR_addr = 0;
    
    }

    if(CIDR_addr){
        int ip_len = strlen(ip);

        int ip_num[4] = {0};
        int ip_pos = 0;

        int pos = 0 ;
        while(pos < ip_len){
            int len = 0;
            while(ip[pos] != '.' && ip[pos] != '/'){
                len++;
                pos++;  
            }
    
            char t[4] = {0};
            strncpy(t,ip,sizeof(char)*len);
            ip_num[ip_pos++] = atoi(t);

            if(ip[pos] == '/')
                break;

            pos++;
        }

        int mask_len = atoi(ip+pos+1);
    
        // 计算子网掩码
        int mask_num[4] = {0};
        for(int i = 0 ; i < 4 ; i++){
            if(mask_len>8){
                mask_num[i] = (1<<8)-1;
                mask_len -= 8;
            }else{
                int t = 128;
                
                while(mask_len){
                    mask_num[i] += t;
                    t >>= 1;
                    mask_len--;

                }
                break;
            }

        }

        char *ip_start = malloc(sizeof(char)*16);
        int copy_pos = 0;
        //计算子网开始地址
        for(int i = 0 ; i < 4 ; i++){
            ip_num[i] =  (ip_num[i]) & (mask_num[i]); 
            if(i == 3){//最后得到的地址为网络号需加1
                ip_num[i]++;
            }

        }
        
        int len = strlen(ip_start);
        ip_start[len-1] = '\0';

        ip_p->ip_all = ip_start; 

        ip_num[3]--;//减去之前加上的值
    

        //计算子网结束地址
        char *ip_end = malloc(sizeof(char)*len);
        copy_pos = 0;

        for(int i = 0 ; i < 4 ; i++){ 
            if(mask_num[i] != 255){
                ip_num[i] = ip_num[i]+ 255 ^ mask_num[i];

            }
            //最后得到的为广播地址需减1
            if(i == 3){
                ip_num[i]--;
            }

            sprintf(ip_end+copy_pos,"%d.",ip_num[i]);
            copy_pos = strlen(ip_end);
            
        }

        int end_len = strlen(ip_end);
        ip_start[end_len-1] = '\0';

        ip_p->ip_all = ip_end;
        LOG("%s",ip_end);
        
    }else{
        ip_p->ip_all = ip;
    
    }
    
    return 0;
}