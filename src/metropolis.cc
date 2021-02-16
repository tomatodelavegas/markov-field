#include "metropolis.hh"

#include <random>
#include <algorithm>

/** Create and initialize the pixel scores */
static cmkv::image<float> create_pcosts(
    int width, int height,
    const std::function<float(int, int)> &cost_fn)
{
    auto pcosts = cmkv::image<float>(width, height);

    for (unsigned y = 0; y < pcosts.height; ++y)
    {
        for (unsigned x = 0; x < pcosts.width; ++x)
        {
            auto pcost = cost_fn(x, y);
            pcosts(x, y) = pcost;
        }
    }

    return pcosts;
}

/** Implementation of the metropolis algorithm */
void metropolis(cmkv::image<std::uint8_t> &img,
                const std::function<float(int, int)> &cost_fn,
                const params &params)
{
    std::random_device rd{};
    std::mt19937 gen{rd()};

    std::uniform_int_distribution<unsigned> x_dist(0, img.width - 1);
    std::uniform_int_distribution<unsigned> y_dist(0, img.height - 1);

    std::uniform_int_distribution<std::uint_fast8_t> bw_dist(0, 1);
    std::uniform_real_distribution<float> uniform_proba_dist(0.0f, 1.0f);

    auto T = params.T_init;

    auto pcosts = create_pcosts(img.width, img.height, cost_fn);

    for (size_t n = 0; n < params.N_iter; ++n)
    {
        auto x = x_dist(gen);
        auto y = y_dist(gen);

        auto &pix_loc = img(x, y);

        auto old_pix = pix_loc;
        pix_loc = bw_dist(gen) ? 255 : 0;

        auto pcost = cost_fn(x, y);
        auto old_pcost = pcosts(x, y);

        // pcost > old -> accept with random
        // pcost < old -> accept all
        // T high -> accept more
        // T low -> accept less

        auto delta_cost = old_pcost - pcost;
        auto accept_ratio = std::exp(delta_cost / T);
        if (accept_ratio >= 1.0f || uniform_proba_dist(gen) < accept_ratio)
        {
            // Keep new pixel
            pcosts(x, y) = pcost;
        }
        else
        {
            // Keep old pixel
            pix_loc = old_pix;
        }

        T *= params.T_dec_factor;
    }
}