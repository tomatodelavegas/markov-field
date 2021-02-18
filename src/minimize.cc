#include "minimize.hh"

#include "metropolis.hh"
#include "image_utils.hh"

namespace cmkv
{
    // static float cost_top_left_movement(const image<std::uint8_t> &img, int x, int y)
    // {
    //     static auto kernel = image_from_array<float, 3, 3>(std::array<float, 9>{
    //         -2, -1, 0,
    //         -1, 0, -1,
    //         0, -1, -2});

    //     static auto kernel2 = image_from_array<float, 3, 3>(std::array<float, 9>{
    //         0, 1, 2,
    //         1, 0, 1,
    //         2, 1, 0});

    //     return conv2D(img, kernel, x, y) + conv2D(img, kernel2, x, y);
    // }

    static float cost_horizontal(const image<std::uint8_t> &img, int x, int y)
    {
        static auto kernel = image_from_array<float, 5, 1>({
            1, 1, -4, 1, 1 //
        });

        return std::abs(conv2D(img, kernel, x, y));
    }

    static float cost_neighbours(const image<std::uint8_t> &img, int x, int y)
    {
        static auto kernel = image_from_array<float, 3, 3>({
            1, 1, 1,
            1, -8, 1,
            1, 1, 1 //
        });

        return std::abs(conv2D(img, kernel, x, y));
    }

    static float cost_diag(const image<std::uint8_t> &img, int x, int y)
    {
        static auto kernel = image_from_array<float, 5, 5>({
            0, 0, 0, 0, 1,
            0, 0, 0, 1, 0,
            0, 0, -4, 0, 0,
            0, 1, 0, 0, 0,
            1, 0, 0, 0, 0 //
        });

        return std::abs(conv2D(img, kernel, x, y));
    }

    // static float cost_spir(const image<std::uint8_t> &img, int x, int y)
    // {
    //     static auto kernel = image_from_array<float, 7, 7>({
    //         0, 0, 1, 1, 1, 1, 0,
    //         0, 1, 1, 0, 0, 0, 0,
    //         1, 0, 0, 1, 1, 1, 0,
    //         1, 0, 0, -24, 0, 1, 0,
    //         1, 1, 0, 0, 0, 1, 1,
    //         0, 1, 1, 1, 1, 1, 0,
    //         0, 0, 1, 1, 0, 0, 0, //
    //     });

    //     return std::abs(conv2D(img, kernel, x, y));
    // }

    // static float cost_ring(const image<std::uint8_t> &img, int x, int y)
    // {
    //     static auto kernel = image_from_array<float, 7, 7>({
    //         1, 1, 1, 1, 1, 1, 1,
    //         1, -1, -1, -1, -1, -1, 1,
    //         1, -1, 0, 0, 0, -1, 1,
    //         1, -1, 0, 0, 0, -1, 1,
    //         1, -1, 0, 0, 0, -1, 1,
    //         1, -1, -1, -1, -1, -1, 1,
    //         1, 1, 1, 1, 1, 1, 1 //
    //     });

    //     return std::abs(conv2D(img, kernel, x, y));
    // }

    /** Minimize a RGB image to B&W "artistically" */
    image<std::uint8_t> minimize(image<cmkv::rgb8_t> &img, const params &params)
    {
        auto simple_bin_img = binarize_img<std::uint8_t, cmkv::rgb8_t>(img);

        auto bin_img = image<std::uint8_t>(img.width, img.height);
        fill_with_either<std::uint8_t>(bin_img, 0, 255);

        auto cost_fn = [&bin_img, &simple_bin_img, &params](int x, int y) {
            // Difference between current pix and basic binarized
            // 1 when different, 0 when same
            float pix = bin_img(x, y);
            float bin_pix = simple_bin_img(x, y);
            float bin_cost = std::abs(pix - bin_pix);
            bin_cost *= params.cost_muls[0];

            // Similar top-left/bottom-right diagonal pixels
            float diag_cost = cost_diag(bin_img, x, y);
            diag_cost *= params.cost_muls[1];

            // Similar horizontal pixels
            float hor_cost = cost_horizontal(bin_img, x, y);
            hor_cost *= params.cost_muls[2];

            // Similar neighbours (all directions)
            float neigh_cost = cost_neighbours(bin_img, x, y);
            neigh_cost *= params.cost_muls[3];

            // float top_left_cost = cost_top_left_movement<3, 3>(bin_img, x, y);
            // top_left_cost *= params.cost_muls[2];

            // float spir_cost = cost_spir(bin_img, x, y);
            // spir_cost *= params.cost_muls[4];

            // float ring_cost = cost_ring(bin_img, x, y);
            // ring_cost *= params.cost_muls[5];

            return bin_cost + diag_cost + hor_cost + neigh_cost;
        };

        metropolis(bin_img, cost_fn, params);
        return bin_img;
    }
} // namespace cmkv
