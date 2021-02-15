#ifndef PARAMS_HH
#define PARAMS_HH

struct params
{
    unsigned long long N_iter;
    float T_init;
    float normal_std;
};

params parse_argv(int argc, char *argv[]);

#endif // PARAMS_HH