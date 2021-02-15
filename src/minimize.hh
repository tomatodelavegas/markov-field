#ifndef MINIMIZE_HH
#define MINIMIZE_HH

#include "image.hh"
#include "params.hh"

namespace cmkv
{
  image<std::uint8_t> minimize(image<cmkv::rgb8_t> &img, const params &params);
}

#endif
