#pragma once
// 精度评估：把"看起来对"变成"数据上对"，输出 metrics.json / metrics.csv。
// 对应需求：FR-M8-01、FR-M8-02；报告中的消融实验依赖这里

#include <cstddef>
#include <string>
#include <vector>

#include "sfm/reconstruction.hpp"

namespace recon {

struct StageTiming {
  std::string stage;
  double seconds = 0.0;
};

struct Metrics {
  std::size_t num_images = 0;
  std::size_t num_registered = 0;
  std::size_t num_points3d = 0;
  std::size_t num_observations = 0;

  double mean_reproj_error = 0.0;
  double rmse_reproj_error = 0.0;
  double p95_reproj_error = 0.0;
  double mean_track_length = 0.0;

  std::size_t num_dense_points = 0;
  bool cuda_enabled = false;
  std::vector<StageTiming> timings;
};

class Evaluator {
 public:
  static Metrics compute(const Reconstruction& reconstruction, std::size_t num_images);

  static bool writeJson(const Metrics& metrics, const std::string& path);
  static bool writeCsv(const Metrics& metrics, const std::string& path);
  static void print(const Metrics& metrics);
};

}  // namespace recon
