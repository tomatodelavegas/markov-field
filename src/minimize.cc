#include "minimize.hh"

#include "metropolis.hh"

#include <cmath>
#include <random>
#include <limits>

namespace cmkv
{
    /** Binarize a pixel (black or white) based on its luminance,
     * which is more representative of the human perception than
     * a simple threashold.
     */
    static std::uint8_t binarize_human(rgb8_t pix)
    {
        auto luminance = 0.299 * pix.r + 0.587 * pix.g + 0.114 * pix.b;
        auto is_white = luminance >= 128;

        return is_white ? 255 : 0;
    }

    /** Binarize an image */
    template <typename T, typename U>
    static image<T> binarize_img(const image<U> &img)
    {
        auto result = image<T>(img.width, img.height);
        for (unsigned y = 0; y < img.height; ++y)
        {
            for (unsigned x = 0; x < img.width; ++x)
            {
                result(x, y) = binarize_human(img(x, y));
            }
        }

        return result;
    }

    /** Fill an image with rnadom values */
    template <typename T>
    static void fill_with_random(image<T> &img)
    {
        std::random_device rd{};
        std::mt19937 gen{rd()};

        std::uniform_int_distribution<T> dist(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

        for (unsigned y = 0; y < img.height; ++y)
        {
            for (unsigned x = 0; x < img.width; ++x)
            {
                img(x, y) = dist(gen);
            }
        }
    }

    float conv2D(const image<cmkv::rgb8_t> &img, const image<float> &kernel, int x, int y)
    {
        int radius_x = std::min(kernel., img.width);
        radius = std::min(radius, img.height);

        int mid_radius = radius / 2;

        x = std::max(x - mid_radius, 0);
        y = std::max(y - mid_radius, 0);

    }

    float cost_similar_neighbours(const image<cmkv::rgb8_t> &img, int x, int y, unsigned radius)
    {
    }

    /** Minimize a RGB image to B&W "artistically" */
    image<std::uint8_t> minimize(image<cmkv::rgb8_t> &img, const params &params)
    {
        auto simple_bin_img = binarize_img<std::uint8_t, cmkv::rgb8_t>(img);

        auto bin_img = image<std::uint8_t>(img.width, img.height);
        fill_with_random(bin_img);

        auto cost_fn = [&bin_img, &simple_bin_img, &params](int x, int y) {
            float pix = bin_img(x, y);
            float bin_pix = simple_bin_img(x, y);

            // Difference between current pix and basic binarized
            // 1 when different, 0 when same
            float bin_cost = std::abs(pix - bin_pix);

            // TODO: Other conditions scores

            return params.cost_muls[0] * bin_cost;
        };

        metropolis(bin_img, cost_fn, params);
        return bin_img;
    }
} // namespace cmkv
