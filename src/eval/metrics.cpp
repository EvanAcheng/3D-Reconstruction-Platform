#include "eval/metrics.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>

#include "core/config.hpp"
#include "core/logger.hpp"

namespace recon {

Metrics Evaluator::compute(const Reconstruction& reconstruction, std::size_t num_images) {
  (void)reconstruction;
  (void)num_images;
  // TODO(FR-M8-01):
  //   num_images / num_registered = reconstruction.numRegisteredCameras()
  //   num_points3d = reconstruction.numPoints()
  //   num_observations = reconstruction.numObservations()
  //   errors = reconstruction.reprojectionErrors()
  //     mean / rmse / p95（排序后取 95% 分位）
  //   mean_track_length = num_observations / num_points3d
  //   cuda_enabled = cv::cuda::getCudaEnabledDeviceCount() > 0
  return Metrics{};
}

bool Evaluator::writeJson(const Metrics& metrics, const std::string& path) {
  (void)metrics;
  (void)path;
  // TODO(FR-M8-02): 写 metrics.json，字段名与 scripts/plot_metrics.py 约定一致
  //   一次运行一个文件，文件名建议 results/<dataset>_<timestamp>.json
  return false;
}

bool Evaluator::writeCsv(const Metrics& metrics, const std::string& path) {
  (void)metrics;
  (void)path;
  // TODO(FR-M8-02): 单行 CSV（便于 pandas 直接 concat 成汇总表）
  //   表头固定，字段顺序不要随意改，否则历史数据对不上
  return false;
}

void Evaluator::print(const Metrics& metrics) {
  (void)metrics;
  // TODO(FR-M8-01): 在控制台打一张对齐的表，末行给出"注册率 x%、RMSE y px、总耗时 z s"
  //   这是答辩时最直观的一屏
}

}  // namespace recon
