#ifndef IMAGE_UTILS_HH
#define IMAGE_UTILS_HH

#include "image.hh"

#include <array>
#include <random>

namespace cmkv
{
    /** Binarize a pixel (black or white) based on its luminance,
     * which is more representative of the human perception than
     * a simple threashold.
     */
    std::uint8_t binarize_human(rgb8_t pix);

    /** Apply a 2D convolution centered at the given pixel of the image */
    float conv2D(const image<std::uint8_t> &img, const image<float> &kernel, int x, int y);

    /** Binarize an image */
    template <typename T, typename U>
    image<T> binarize_img(const image<U> &img)
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

    /** Fill an image with either `low` or `high` randomly for each pixel,
     * each value with the same probability (of 0.5).
     */
    template <typename T>
    void fill_with_either(image<T> &img, T low, T high)
    {
        static std::minstd_rand gen{std::random_device{}()};

        for (unsigned y = 0; y < img.height; ++y)
        {
            for (unsigned x = 0; x < img.width; ++x)
            {
                img(x, y) = (gen() & 1) ? high : low;
            }
        }
    }

    /** Create an image from a row-major ordered array of values */
    template <typename T, unsigned W, unsigned H>
    image<T> image_from_array(const std::array<T, W * H> &values)
    {
        auto img = image<T>(W, H);

        for (unsigned y = 0; y < H; ++y)
        {
            for (unsigned x = 0; x < W; ++x)
            {
                img(x, y) = values[x + y * H];
            }
        }

        return img;
    }
}

#endif // IMAGE_UTILS_HH
