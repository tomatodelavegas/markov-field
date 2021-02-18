#include "metropolis.hh"

#include <random>

/** Create and initialize the pixel scores */
static cmkv::image<float> create_pcosts(
    unsigned width, unsigned height,
    const std::function<float(int, int)> &cost_fn)
{
    auto pcosts = std::vector<float>();
    pcosts.reserve(width * height);

    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            pcosts.emplace_back(cost_fn(x, y));
        }
    }

    return cmkv::image<float>(width, height, std::move(pcosts));
}

/** Generate a random unsigned integer in `[0, max_excl[` */
template <class G>
static uint_fast32_t randuint(G &gen, uint_fast32_t max_excl)
{
    return gen() % max_excl;
}

/** Generate a random float in [0.0, 1.0] */
template <class G>
static float randfloat(G &gen)
{
    return static_cast<float>(gen()) / G::max();
}

/** Flip the pixel value between black & white (W->B, B->W) */
static void flip_bw(std::uint8_t &value)
{
    value = 255 - value;
}

/** Implementation of the metropolis algorithm */
void metropolis(cmkv::image<std::uint8_t> &img,
                const std::function<float(int, int)> &cost_fn,
                const params &params)
{
    static std::minstd_rand gen{std::random_device{}()};

    auto T = params.T_init;
    auto pcosts = create_pcosts(img.width, img.height, cost_fn);

    for (size_t n = 0; n < params.N_iter; ++n)
    {
        auto x = randuint(gen, img.width);
        auto y = randuint(gen, img.height);

        auto &pix_loc = img(x, y);
        flip_bw(pix_loc);

        auto pcost = cost_fn(x, y);
        auto old_pcost = pcosts(x, y);

        auto delta_cost = old_pcost - pcost;
        auto accept_ratio = std::exp(delta_cost / T);
        if (accept_ratio >= 1.0f || randfloat(gen) < accept_ratio)
        {
            // Keep new pixel
            pcosts(x, y) = pcost;
        }
        else
        {
            // Keep old pixel
            flip_bw(pix_loc);
        }

        T *= params.T_dec_factor;
    }
}