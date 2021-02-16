#include "metropolis.hh"

#include <random>
#include <algorithm>

#include <iostream>

/** Create and initialize the pixel scores */
static cmkv::image<float> create_pscores(
    int width, int height,
    const std::function<float(int, int)> &score)
{
    (void)score;
    auto pscores = cmkv::image<float>(width, height);

    for (unsigned y = 0; y < pscores.height; ++y)
    {
        for (unsigned x = 0; x < pscores.width; ++x)
        {
            // auto pscore = score(x, y);
            pscores(x, y) = 1;
        }
    }

    return pscores;
}

/** Modify a pixel value based on a random normal distribution */
// static cmkv::rgb8_t change_pixel_random(
//     cmkv::rgb8_t pix,
//     std::normal_distribution<float> &normal_dist,
//     std::mt19937 &gen)
// {
//     pix.r = std::clamp<std::uint8_t>(pix.r + normal_dist(gen), 0, 255);
//     pix.b = std::clamp<std::uint8_t>(pix.b + normal_dist(gen), 0, 255);
//     pix.g = std::clamp<std::uint8_t>(pix.g + normal_dist(gen), 0, 255);

//     return pix;
// }

/** Implementation of the metropolis algorithm */
void metropolis(cmkv::image<cmkv::rgb8_t> &img,
                const std::function<float(int, int)> &score,
                const params &params)
{
    std::random_device rd{};
    std::mt19937 gen{rd()};

    std::uniform_int_distribution<unsigned> x_dist(0, img.width - 1);
    std::uniform_int_distribution<unsigned> y_dist(0, img.height - 1);

    std::uniform_int_distribution<std::uint8_t> bw_dist(0, 1);

    std::normal_distribution<float> normal_dist(0, params.normal_std);
    std::uniform_real_distribution<float> uniform_proba_dist(0.0f, 1.0f);

    auto T = params.T_init;

    auto pscores = create_pscores(img.width, img.height, score);

    for (size_t n = 0; n < params.N_iter; ++n)
    {
        auto x = x_dist(gen);
        auto y = y_dist(gen);

        auto &pix_loc = img(x, y);

        auto old_pix = pix_loc;
        if (bw_dist(gen))
        {
            pix_loc.r = 255;
            pix_loc.g = 255;
            pix_loc.b = 255;
        }
        else
        {
            pix_loc.r = 0;
            pix_loc.g = 0;
            pix_loc.b = 0;
        }

        // pix_loc = change_pixel_random(old_pix, normal_dist, gen);

        auto pscore = score(x, y);

        auto old_pscore = pscores(x, y);
        if (std::abs(old_pscore) < 0.0001f)
        {
            std::cout << old_pscore << '\n';
            old_pscore = 0.0001f;
        }

        auto accept_ratio = T * pscore / old_pscore;
        if (accept_ratio >= 1.0f || uniform_proba_dist(gen) < accept_ratio)
        {
            // Keep new pixel
            pscores(x, y) = pscore;
        }
        else
        {
            // Keep old pixel
            pix_loc = old_pix;
        }

        T *= params.T_dec_factor;
    }
}