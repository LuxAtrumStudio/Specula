#ifndef SPECULA_BIN_CLI_HPP_
#define SPECULA_BIN_CLI_HPP_

#include <regex>
#include <string>
#ifdef __APPLE__
#include <cli/cli.hpp>
#else
#include <CLI/CLI.hpp>
#endif

struct args_t {
  bool valid;
  std::string script_path, output_path;
  std::size_t verbosity, spp, min_bounces, tile_size, threads, res_width,
      res_height;
  bool render_albido, render_normal, render_depth, denoise;
  float fov;
};

args_t parse_args(int argc, char *argv[]);

#endif // SPECULA_BIN_CLI_HPP_