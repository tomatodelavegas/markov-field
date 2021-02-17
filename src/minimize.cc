#include "minimize.hh"

#include "metropolis.hh"
#include "image_utils.hh"

#include <iostream>

namespace cmkv
{
    template <unsigned W, unsigned H>
    image<float> cost_similar_neighbours_kernel()
    {
        auto kernel = image<float>(W, H);

        for (unsigned y = 0; y < H; ++y)
        {
            for (unsigned x = 0; x < W; ++x)
            {
                kernel(x, y) = 1;
            }
        }

        kernel(W / 2, H / 2) = -(W * H) + 1;

        return kernel;
    }

    template <unsigned W, unsigned H>
    float cost_similar_neighbours(const image<std::uint8_t> &img, int x, int y)
    {
        static auto kernel = cost_similar_neighbours_kernel<W, H>();

        return std::abs(conv2D(img, kernel, x, y));
    }

    // template <unsigned W, unsigned H>
    // image<float> cost_similar_neighbours_kernel()
    // {
    //     auto kernel = image<float>(W, H);

    //     for (unsigned y = 0; y < H; ++y)
    //     {
    //         for (unsigned x = 0; x < W; ++x)
    //         {
    //             kernel(x, y) = 1;
    //         }
    //     }

    //     kernel(W / 2, H / 2) = -8;

    //     return kernel;
    // }

    template <unsigned W, unsigned H>
    float cost_top_left_movement(const image<std::uint8_t> &img, int x, int y)
    {
        static auto kernel = image_from_array<float, W, H>(std::array<float, 9>{
            -2, -1, 0,
            -1, 0, -1,
            0, -1, -2});

        static auto kernel2 = image_from_array<float, W, H>(std::array<float, 9>{
            0, 1, 2,
            1, 0, 1,
            2, 1, 0});

        return conv2D(img, kernel, x, y) + conv2D(img, kernel2, x, y);
    }

    float cost_horizontal(const image<std::uint8_t> &img, int x, int y)
    {
        static auto kernel = image_from_array<float, 5, 1>(std::array<float, 5>{
            1, 1, -4, 1, 1});

        return conv2D(img, kernel, x, y);
    }

    /** Minimize a RGB image to B&W "artistically" */
    image<std::uint8_t> minimize(image<cmkv::rgb8_t> &img, const params &params)
    {
        auto simple_bin_img = binarize_img<std::uint8_t, cmkv::rgb8_t>(img);

        auto bin_img = image<std::uint8_t>(img.width, img.height);
        fill_with_random(bin_img);
        image_threshold<std::uint8_t>(bin_img, 128, 0, 255);

        auto cost_fn = [&bin_img, &simple_bin_img, &params](int x, int y) {
            float pix = bin_img(x, y);
            float bin_pix = simple_bin_img(x, y);

            // Difference between current pix and basic binarized
            // 1 when different, 0 when same
            float bin_cost = std::abs(pix - bin_pix);
            bin_cost *= params.cost_muls[0];

            float similar_neigh_cost = cost_similar_neighbours<3, 3>(bin_img, x, y);
            similar_neigh_cost *= params.cost_muls[1];

            float top_left_cost = cost_top_left_movement<3, 3>(bin_img, x, y);
            top_left_cost *= params.cost_muls[2];

            float hor_cost = cost_horizontal(bin_img, x, y);
            hor_cost *= params.cost_muls[3];

            return bin_cost + similar_neigh_cost + top_left_cost + hor_cost;
        };

        metropolis(bin_img, cost_fn, params);
        return bin_img;
    }
} // namespace cmkv
