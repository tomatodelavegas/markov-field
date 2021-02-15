#ifndef METROPOLIS_HH
#define METROPOLIS_HH

#include <functional>

#include "image.hh"

void metropolis(cmkv::image<cmkv::rgb8_t> &img,
                const std::function<float(int, int)> &score,
                size_t N,
                float T_init,
                float normal_std);

#endif // METROPOLIS_HH