#include "renderer.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#ifdef __OPENCL__
#ifdef __APPLE__
#include <OpenCL/opencl.hpp>
#else
#include <CL/cl.hpp>
#endif
#endif

#include "filesystem.hpp"
#include "image/image.hpp"
#include "log.hpp"
#include "math.hpp"
#include "rand.hpp"
#include "renderer_args.hpp"
#include "scene.hpp"
#include "thread.hpp"

bool specula::render(const renderer_args_t &args) {
  fs::path file(args.file);
  std::string file_extension = file.extension().string();
  file.replace_extension("");
  if (args.sequence)
    file.append(std::to_string(args.frame));

  std::vector<std::shared_ptr<ObjectBase>> objects =
      scene::get()->get_visible();
  if (objects.size() == 0) {
    LWARN("No visible object present in scene");
  }

  std::shared_ptr<image::Image> shaded = std::make_shared<image::Image>(
                                    args.res_width, args.res_height),
                                albedo(nullptr), depth(nullptr),
                                normal(nullptr);
  if (args.render_albedo || args.denoise)
    albedo = std::make_shared<image::Image>(args.res_width, args.res_height);
  if (args.render_depth || args.denoise)
    depth = std::make_shared<image::Image>(args.res_width, args.res_height);
  if (args.render_normal || args.denoise)
    normal = std::make_shared<image::Image>(args.res_width, args.res_height);

#ifdef __OPENCL__
  if (std::any_of(objects.begin(), objects.end(),
                  [](const std::shared_ptr<ObjectBase> &o) {
                    return !o->gpu_enabled();
                  })) {
    LWARN("Not all objects are GPU enabled, defaulting to CPU renderer");
    if (std::any_of(objects.begin(), objects.end(),
                    [](const std::shared_ptr<ObjectBase> &o) {
                      return !o->cpu_enabled();
                    })) {
      LERROR("Not all objects are CPU enabled, aborting render");
      return false;
    } else {
      cpu_renderer(objects, args, {shaded, albedo, depth, normal});
    }
  } else {
    gpu_renderer(objects, args, {shaded, albedo, depth, normal});
  }
#else
  if (std::any_of(objects.begin(), objects.end(),
                  [](const std::shared_ptr<ObjectBase> &o) {
                    return !o->cpu_enabled();
                  })) {
    LERROR("Not all objects are CPU enabled, aborting render");
    return false;
  } else {
    cpu_renderer(objects, args, {shaded, albedo, depth, normal});
  }
#endif

  fs::path shaded_file = file;
  shaded->write(shaded_file.replace_extension(file_extension));
  if (args.render_albedo) {
    fs::path albedo_file = file;
    albedo->write(
        albedo_file.concat("-albedo").replace_extension(file_extension));
  }
  if (args.render_depth) {
    fs::path depth_file = file;
    depth->normalize();
    depth->write(depth_file.concat("-depth").replace_extension(file_extension));
  }
  if (args.render_normal) {
    fs::path normal_file = file;
    normal->abs();
    normal->normalize();
    normal->write(
        normal_file.concat("-normal").replace_extension(file_extension));
  }

  return true;
}

#ifdef __OPENCL__
bool specula::gpu_renderer(const std::vector<std::shared_ptr<ObjectBase>> &objs,
                           const renderer_args_t &args,
                           const buffer_t &buffers) {
  LINFO("Utilizing GPU renderer");
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  for (auto &it : platforms) {
    LINFO("CL Platform: {}", it.getInfo<CL_PLATFORM_NAME>());
    std::vector<cl::Device> devices;
    it.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    for (auto &dev : devices) {
      LINFO("  CL Device: {} ({})", dev.getInfo<CL_DEVICE_NAME>(),
            dev.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>());
    }
  }
}
#endif

bool specula::cpu_renderer(const std::vector<std::shared_ptr<ObjectBase>> &objs,
                           const renderer_args_t &args,
                           const buffer_t &buffers) {
  LINFO("Utilizing CPU renderer");
  std::size_t htiles = static_cast<std::size_t>(
      std::ceil(args.res_width / static_cast<double>(args.tile_size)));
  std::size_t vtiles = static_cast<std::size_t>(
      std::ceil(args.res_height / static_cast<double>(args.tile_size)));
  std::size_t tiles = htiles * vtiles;
  thread::Pool pool(args.threads);
  std::vector<std::future<std::size_t>> pool_results;
  for (std::size_t tile_id = 0; tile_id < tiles; ++tile_id) {
    pool_results.push_back(pool.enqueue(
        cpu_render_tile, tile_id, glm::uvec2{args.res_width, args.res_height},
        glm::uvec4{
            (tile_id % htiles) * args.tile_size,
            (tile_id / htiles) * args.tile_size,
            ((tile_id % htiles) * args.tile_size) +
                std::min(static_cast<std::size_t>(((tile_id % htiles) + 1) *
                                                  args.tile_size),
                         args.res_width) -
                (tile_id % htiles) * args.tile_size,
            ((tile_id / htiles) * args.tile_size) +
                std::min(static_cast<std::size_t>(((tile_id / htiles) + 1) *
                                                  args.tile_size),
                         args.res_height) -
                (tile_id / htiles) * args.tile_size,
        },
        buffers));
  }

  std::size_t processed = 0;
  std::size_t len_tiles = std::to_string(tiles).size();
  double total_time = 0.0f;
  auto start = std::chrono::high_resolution_clock::now();
  while (pool_results.size() != 0) {
    for (std::size_t i = 0; i < pool_results.size(); ++i) {
      if (pool_results[i].wait_for(std::chrono::milliseconds(10)) !=
          std::future_status::ready)
        continue;
      std::size_t id = pool_results[i].get();
      processed++;
      double delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                              std::chrono::high_resolution_clock::now() - start)
                              .count() /
                          1e9;
      total_time += delta_time;
      unsigned long elapsed_hour_count =
          static_cast<unsigned long>(total_time) / 3600;
      unsigned long elapsed_minute_count =
          (static_cast<unsigned long>(total_time) % 3600) / 60;
      unsigned long elapsed_second_count =
          (static_cast<unsigned long>(total_time) % 60);
      double remaining = (total_time / processed) * (tiles - processed);
      unsigned long remaining_hour_count =
          static_cast<unsigned long>(remaining) / 3600;
      unsigned long remaining_minute_count =
          (static_cast<unsigned long>(remaining) % 3600) / 60;
      unsigned long remaining_second_count =
          (static_cast<unsigned long>(remaining) % 60);
      LINFO("Rendered Tile {:>{}} [{:>{}}/{}] ({:7.3f}%) "
            "Elapsed: {:02}:{:02}:{:02} Remaining: {:02}:{:02}:{:02}",
            id, len_tiles, processed, len_tiles, tiles,
            100.0f * static_cast<float>(processed) / static_cast<float>(tiles),
            elapsed_hour_count, elapsed_minute_count, elapsed_second_count,
            remaining_hour_count, remaining_minute_count,
            remaining_second_count);
      start = std::chrono::high_resolution_clock::now();
      pool_results.erase(pool_results.begin() + i);
    }
    pool_results.erase(std::remove_if(pool_results.begin(), pool_results.end(),
                                      [](const std::future<std::size_t> &it) {
                                        return !it.valid();
                                      }),
                       pool_results.end());
  }
  return true;
}

std::size_t specula::cpu_render_tile(const std::size_t &tile_id,
                                     const glm::uvec2 &img_bounds,
                                     const glm::uvec4 &tile_bounds,
                                     const buffer_t &buffers) {
  const float r = frand(), g = frand(), b = frand();
  for (std::size_t x = tile_bounds.x; x < tile_bounds.z; ++x) {
    for (std::size_t y = tile_bounds.y; y < tile_bounds.w; ++y) {
      buffers.img->set(x, y, {r, g, b});
    }
  }
  return tile_id;
}
