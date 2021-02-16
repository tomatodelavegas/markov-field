#include "params.hh"

#include <string>

params parse_argv(int argc, char *argv[])
{
    params params;
    params.N_iter = 1'000'000;
    params.T_init = 1'000;
    params.T_dec_factor = 0.9999;
    params.normal_std = 1.0;

    if (argc >= 4)
    {
        params.N_iter = std::stoull(argv[3]);
    }

    if (argc >= 5)
    {
        params.T_init = std::stof(argv[4]);
    }

    if (argc >= 6)
    {
        params.T_dec_factor = std::stof(argv[5]);
    }

    if (argc >= 7)
    {
        params.normal_std = std::stof(argv[6]);
    }

    return params;
}