#ifndef METROPOLIS_HH
#define METROPOLIS_HH

#include <functional>

#include "image.hh"
#include "params.hh"

void metropolis(cmkv::image<cmkv::rgb8_t> &img,
                const std::function<float(int, int)> &score,
                const params &params);

#endif // METROPOLIS_HH