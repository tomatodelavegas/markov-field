#include "metropolis.hh"

#include <random>
#include <algorithm>

/** Create and initialize the pixel scores */
static cmkv::image<float> create_pscores(
    int width, int height,
    const std::function<float(int, int)> &score)
{
    auto pscores = cmkv::image<float>(width, height);

    for (unsigned y = 0; y < pscores.height; ++y)
    {
        for (unsigned x = 0; x < pscores.width; ++x)
        {
            auto pscore = score(x, y);
            pscores(x, y) = pscore;
        }
    }

    return pscores;
}

/** Modify a pixel value based on a random normal distribution */
static cmkv::rgb8_t change_pixel_random(
    cmkv::rgb8_t pix,
    std::normal_distribution<float> &normal_dist,
    std::mt19937 &gen)
{
    pix.r = std::clamp<std::uint8_t>(pix.r + normal_dist(gen), 0, 255);
    pix.b = std::clamp<std::uint8_t>(pix.b + normal_dist(gen), 0, 255);
    pix.g = std::clamp<std::uint8_t>(pix.g + normal_dist(gen), 0, 255);

    return pix;
}

/** Implementation of the metropolis algorithm */
void metropolis(cmkv::image<cmkv::rgb8_t> &img,
                const std::function<float(int, int)> &score,
                const params &params)
{
    // TODO; params.T_init

    std::random_device rd{};
    std::mt19937 gen{rd()};

    std::uniform_int_distribution<unsigned> x_dist(0, img.width - 1);
    std::uniform_int_distribution<unsigned> y_dist(0, img.height - 1);

    std::normal_distribution<float> normal_dist(0, params.normal_std);
    std::uniform_real_distribution<float> uniform_proba_dist(0.0f, 1.0f);

    auto pscores = create_pscores(img.width, img.height, score);

    for (size_t n = 0; n < params.N_iter; ++n)
    {
        auto x = x_dist(gen);
        auto y = y_dist(gen);

        auto &pix_loc = img(x, y);

        auto old_pix = pix_loc;
        pix_loc = change_pixel_random(old_pix, normal_dist, gen);

        auto pscore = score(x, y);

        auto accept_ratio = pscore / pscores(x, y);
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
    }
}