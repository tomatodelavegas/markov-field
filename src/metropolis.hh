#ifndef METROPOLIS_HH
#define METROPOLIS_HH

#include <functional>

#include "image.hh"
#include "params.hh"

void metropolis(cmkv::image<std::uint8_t> &img,
                const std::function<float(int, int)> &cost_fn,
                const params &params);

#endif // METROPOLIS_HH
