struct ip_address{
    char *ip_all;

    int ip_nums;
};

struct function 
{
    
    short ping;
    short get_ip;
    short start_up;
    short shut_up;
    short get_time;
    short usage;

};

struct hp{
    struct ip_address ip_t;
    struct function fun_t;
};


int prase_main_entry(int argc,char *argv[],struct hp *hp_p);
int prase_function(int argc,char *argv[],struct hp *hp_p);
int prase_ip(char *ip,struct ip_address * ip_p);
