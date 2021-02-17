#ifndef PARAMS_HH
#define PARAMS_HH

constexpr unsigned NB_MAX_COST_FN = 10;

struct params
{
    /** The number of iterations */
    unsigned long long N_iter;

    /** The initial temperature.
     * Must be positive.
     */
    float T_init;

    /** The decreasing factor of the temperature where:
     * `T(n+1) = T_dec_factor * T(n)`.
     *
     * Must be in range [0.0, 1.0]
     */
    float T_dec_factor;

    /** The standard deviation when modifing a pixel.
     * A higher value means greater changes.
     */
    float normal_std;

    /** The cost functions multipliers */
    float cost_muls[NB_MAX_COST_FN];
};

/** Extract the parameters from the program arguments
 * or provide default values for those not provided.
 */
params parse_argv(int argc, char *argv[]);

#endif // PARAMS_HH