#include <iostream>

#include "image.hh"
#include "minimize.hh"
#include "params.hh"

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    std::cerr << argv[0] << " input.png output.png [N_iter [T_init [normal_std]]]\n";
    return EXIT_FAILURE;
  }

  auto params = parse_argv(argc, argv);

  FreeImage_Initialise();

  auto input = cmkv::io::read(argv[1]);
  auto output = cmkv::minimize(input, params);
  cmkv::io::write(argv[2], output);

  FreeImage_DeInitialise();
}
