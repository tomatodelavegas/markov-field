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

    constexpr const char *opt_args[] = {"N_iter", "T_init", "T_dec_factor", "normal_std", "cost_muls... (multiples values)"};
    for (const auto &arg : opt_args)
    {
      std::cerr << "    - " << arg << '\n';
    }

    return EXIT_FAILURE;
  }

  auto params = parse_argv(argc, argv);

  FreeImage_Initialise();

  auto input = cmkv::io::read(argv[1]);
  auto output = cmkv::minimize(input, params);
  cmkv::io::write(argv[2], output);

  FreeImage_DeInitialise();
}
