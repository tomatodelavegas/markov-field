#include "minimize.hh"

#include "metropolis.hh"

#include <cmath>
#include <iostream>

namespace cmkv
{
    static std::uint8_t binarize_human(rgb8_t pix)
    {
        auto luminance = 0.299 * pix.r + 0.587 * pix.g + 0.114 * pix.b;
        auto is_white = luminance >= 128;

        return is_white ? 255 : 0;
    }

    static image<std::uint8_t> binarize_img(const image<cmkv::rgb8_t> &img)
    {
        auto result = image<std::uint8_t>(img.width, img.height);
        for (unsigned y = 0; y < img.height; ++y)
        {
            for (unsigned x = 0; x < img.width; ++x)
            {
                result(x, y) = binarize_human(img(x, y));
            }
        }

        return result;
    }

    static float norm2(cmkv::rgb8_t pix1, cmkv::rgb8_t pix2)
    {
        auto dr = pix1.r - pix2.r;
        auto dg = pix1.g - pix2.g;
        auto db = pix1.b - pix2.b;

        auto normsqr = dr * dr + dg * dg + db * db;
        return std::sqrt(normsqr);
    }

    image<std::uint8_t> minimize(image<cmkv::rgb8_t> &img, const params &params)
    {
        auto bin_img = binarize_img(img);

        auto score = [&img, &bin_img](int x, int y) {
            auto &pix = img(x, y);
            auto &bin_pix = bin_img(x, y);

            // Difference between current pix and basic binarized
            auto bin_score = -norm2(pix, bin_pix);

            // TODO: Other conditions scores

            return bin_score;
        };

        metropolis(img, score, params);

        return binarize_img(img);
    }

    // image<std::uint8_t> minimize(const image<std::uint8_t> &img)
    // {
    //     // FIXME: replace the code below by yours!
    //     auto result = image<std::uint8_t>(img.width, img.height);
    //     for (std::size_t y = 0; y < img.height; y++)
    //     {
    //         for (std::size_t x = 0; x < img.width; x++)
    //         {
    //             result(x, y) = img(x, y) > 127u ? 255u : 0u;
    //         }
    //     }

    //     return result;
    // }

} // namespace cmkv
