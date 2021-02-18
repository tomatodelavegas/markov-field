#include "image_utils.hh"

namespace cmkv
{
    constexpr auto DEBUG_ASSERTS = false;

    std::uint8_t binarize_human(rgb8_t pix)
    {
        auto luminance = 0.299 * pix.r + 0.587 * pix.g + 0.114 * pix.b;
        auto is_white = luminance >= 128;

        return is_white ? 255 : 0;
    }

    float conv2D(const image<std::uint8_t> &img, const image<float> &kernel, int x, int y)
    {
        // Make sure the kernel is not larger than the image
        // Asserting these conditions also makes the compiled code faster!
        assert(img.height >= kernel.height);
        assert(img.width >= kernel.width);

        unsigned mid_kernel_height = kernel.height / 2;
        unsigned mid_kernel_width = kernel.width / 2;

        // y_cell = y + yk - mid_kernel_height
        // 0 <= y_cell < img.height
        // 0 <= yk <= kernel.height
        // These conditions can be used to find yk_start & yk_end

        // The min/max must be done with type `int` to avoid unsigned underflow
        unsigned yk_start = std::max<int>(0, mid_kernel_height - y);
        unsigned yk_end = std::min(kernel.height, img.height - y + mid_kernel_height);

        // Same for x
        unsigned xk_start = std::max<int>(0, mid_kernel_width - x);
        unsigned xk_end = std::min(kernel.width, img.width - x + mid_kernel_width);

        // Some debug asserts to make sure the formulas are correct
        // However, these slow down the hotpath so they can be turned off
        if constexpr (DEBUG_ASSERTS)
        {
            assert(yk_start <= kernel.height);
            assert(yk_end <= kernel.height);
            assert(xk_start <= kernel.width);
            assert(xk_end <= kernel.width);
        }

        float res = 0;
        unsigned y_cell = y - mid_kernel_height + yk_start;
        for (unsigned yk = yk_start; yk < yk_end; ++yk, ++y_cell)
        {
            float sub_res = 0;
            unsigned x_cell = x - mid_kernel_width + xk_start;
            for (unsigned xk = xk_start; xk < xk_end; ++xk, ++x_cell)
            {
                res += kernel(xk, yk) * static_cast<float>(img(x_cell, y_cell));
            }
            res += sub_res;
        }

        return res;
    }
}
