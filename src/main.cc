#include <iostream>

#include "image.hh"
#include "minimize.hh"
#include "params.hh"

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    std::cerr << argv[0] << " input.png output.png [PARAMS]\n"
              << "  where PARAMS are the following optional arguments (in that order):\n";

    constexpr const char *opt_args[] = {"N_iter", "T_init", "T_dec_factor", "bin_mul", "diag_mul", "hor_mul", "neigh_mul", "corners_mul"};
    for (const auto &arg : opt_args)
    {
      std::cerr << "    - " << arg << '\n';
    }

    std::cerr << "  (for muls, 0 disable the corresponding modifier, 1 is a good starting value)\n";

    return EXIT_FAILURE;
  }

  auto params = parse_argv(argc, argv);

  FreeImage_Initialise();

  auto input = cmkv::io::read(argv[1]);
  auto output = cmkv::minimize(input, params);
  cmkv::io::write(argv[2], output);

  FreeImage_DeInitialise();
}
