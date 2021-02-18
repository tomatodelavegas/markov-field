#include "params.hh"

#include <string>

params parse_argv(int argc, char *argv[])
{
    params params;
    params.N_iter = 1'000'000;
    params.T_init = 1'000'000;
    params.T_dec_factor = 0.9999;

    // Set unknown cost multipliers to 0
    for (unsigned i = 0; i < NB_MAX_COST_FN; ++i)
    {
        params.cost_muls[i] = 0;
    }

    // Default values for costs fn
    params.cost_muls[0] = 1; // Cost: abs diff with basic binarized
    params.cost_muls[1] = 1; // Cost: similar top-left/bottom-right diagonal
    // params.cost_muls[2] = 1; // Cost: similar horizontal
    // params.cost_muls[3] = 1; // Cost: similar neighbours
    // params.cost_muls[4] = 1; // Cost: corners

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
        for (unsigned i = 6; i < static_cast<unsigned>(argc) && i - 6 < NB_MAX_COST_FN; ++i)
        {
            params.cost_muls[i - 6] = std::stof(argv[i]);
        }
    }

    return params;
}