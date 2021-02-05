#include "minimize.hh"

namespace cmkv
{

  image<std::uint8_t> minimize(const image<std::uint8_t>& img)
  {
    // FIXME: replace the code below by yours!
    auto result = image<std::uint8_t>(img.width, img.height);
    for (std::size_t y = 0; y < img.height; y++)
      for (std::size_t x = 0; x < img.width; x++)
	result(x, y) = img(x, y) > 127u ? 255u : 0u;
    return result;
  }

} // cmkv
