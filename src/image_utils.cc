#include "image_utils.hh"

namespace cmkv
{
    std::uint8_t binarize_human(rgb8_t pix)
    {
        auto luminance = 0.299 * pix.r + 0.587 * pix.g + 0.114 * pix.b;
        auto is_white = luminance >= 128;

        return is_white ? 255 : 0;
    }

    float conv2D(const image<std::uint8_t> &img, const image<float> &kernel, int x, int y)
    {
        assert(img.height >= kernel.height);
        assert(img.width >= kernel.width);

        int mid_kernel_height = kernel.height / 2;
        int mid_kernel_width = kernel.width / 2;

        float res = 0;

        // y_cell = y + yk - mid_kernel_height
        // 0 <= y_cell < img.height
        auto yk_start = std::max<int>(0, mid_kernel_height - y);
        auto yk_end = std::min<int>(kernel.height, img.height - y + mid_kernel_height);

        // yk + y - mkh < imgh
        // yk <= imgh + mkh - y
        // yk <= kh

        // yk = min(kh, (imgh - y) + mkh)

        // Same for x
        auto xk_start = std::max<int>(0, mid_kernel_width - x);
        auto xk_end = std::min<int>(kernel.width, img.width - x + mid_kernel_width);

        assert(yk_start >= 0);
        assert(yk_start <= static_cast<int>(kernel.height));
        assert(yk_end >= 0);
        assert(yk_end <= static_cast<int>(kernel.height));
        assert(xk_start >= 0);
        assert(xk_start <= static_cast<int>(kernel.width));
        assert(xk_end >= 0);
        assert(xk_end <= static_cast<int>(kernel.width));

        for (int yk = yk_start; yk < yk_end; ++yk)
        {
            int y_cell = y + yk - mid_kernel_height;

            for (int xk = xk_start; xk < xk_end; ++xk)
            {
                int x_cell = x + xk - mid_kernel_width;

                res += kernel(xk, yk) * static_cast<float>(img(x_cell, y_cell));
            }
        }

        return res;
    }
}
