#pragma once
// 光束法平差（BA）：Ceres 为主，g2o 为备选；含鲁棒核与外点剔除。
// 对应需求：FR-M5-01 ~ FR-M5-05
//
// 实现要点（写代码时按这个结构填）：
//   1. 为每台相机建 6 维参数块（angle-axis + t），固定内参时把内参设为常量
//   2. 为每个三维点建 3 维参数块
//   3. 每个观测加一个 ResidualBlock：ReprojectionCost，用鲁棒核包住
//   4. Solver::Options 选 SPARSE_SCHUR（Ceres 内部用 Schur 消元处理点参数）
//   5. 求解后按重投影误差阈值剔除观测，再跑一轮

#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "core/config.hpp"
#include "core/types.hpp"
#include "sfm/reconstruction.hpp"

namespace recon {

struct BundleAdjustmentStats {
  int iterations = 0;
  double initial_cost = 0.0;
  double final_cost = 0.0;
  double elapsed_seconds = 0.0;
  bool converged = false;
  int num_residuals = 0;
  int num_parameters = 0;
};

class BundleAdjuster {
 public:
  explicit BundleAdjuster(const SfmConfig& config);
  ~BundleAdjuster();

  // 全局 BA
  bool optimize(Reconstruction* reconstruction,
                bool fix_intrinsics,
                BundleAdjustmentStats* stats);

  // 局部 BA：只优化 local_cameras 及其可见点，其余固定
  bool optimizeLocal(Reconstruction* reconstruction,
                     const std::vector<CameraId>& local_cameras,
                     BundleAdjustmentStats* stats);

  // 剔除重投影误差超过阈值的观测，返回剔除数量
  int removeOutlierObservations(Reconstruction* reconstruction, double max_error_px);
  // 剔除观测数不足的三维点，返回剔除数量
  int removeWeakPoints(Reconstruction* reconstruction, int min_observations);

  // 把每轮 BA 的统计写成 CSV，供 Python 画收敛曲线（FR-M5-05）
  bool writeLog(const std::string& csv_path) const;
  const std::vector<BundleAdjustmentStats>& history() const;

 private:
  // 构建图模型：相机/点参数块 + 残差块
  bool buildProblem(Reconstruction* reconstruction,
                    bool fix_intrinsics,
                    BundleAdjustmentStats* stats);

  SfmConfig config_;
  std::vector<BundleAdjustmentStats> history_;
};

}  // namespace recon
