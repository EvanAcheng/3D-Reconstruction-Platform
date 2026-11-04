#include "sfm/optimizer.hpp"

#include <algorithm>
#include <fstream>

#include "core/logger.hpp"
#include "geometry/pose_estimator.hpp"

namespace recon {

BundleAdjuster::BundleAdjuster(const SfmConfig& config) : config_(config) {}

BundleAdjuster::~BundleAdjuster() = default;

bool BundleAdjuster::buildProblem(Reconstruction* reconstruction,
                                  bool fix_intrinsics,
                                  BundleAdjustmentStats* stats) {
  (void)reconstruction;
  (void)fix_intrinsics;
  (void)stats;
  // TODO(FR-M5-01) 这是整个项目最需要仔细写的一段，按 Ceres 官方 bundle_adjuster.cc 的结构：
  //   1) 定义残差：
  //      struct ReprojectionCost {
  //        template <typename T>
  //        bool operator()(const T* const camera, const T* const point, T* residuals) const;
  //      };  // 6 维相机（angle-axis 3 + t 3）、3 维点，输出 2 维像素残差
  //   2) problem.AddParameterBlock(camera, 6);
  //      camera 的流形：problem.SetManifold(camera, new ceres::AngleAxisRotatePoint... )
  //      说明：Ceres 2.x 用 Manifold 替换旧的 LocalParameterization；
  //      本工程固定用 angle-axis，避免四元数归一化扰动
  //   3) problem.AddParameterBlock(point, 3);
  //   4) 每个观测：problem.AddResidualBlock(
  //        new ceres::AutoDiffCostFunction<ReprojectionCost, 2, 6, 3>(new ReprojectionCost(pixel)),
  //        loss, camera_block, point_block)
  //      loss 按 config_.robust_loss 选 ceres::HuberLoss / CauchyLoss / nullptr
  //   5) fix_intrinsics 为 false 时，把 fx/fy/cx/cy/k1/k2 也做成参数块（P2）
  //   6) 固定参考相机（第一台）防止规范自由度导致的整体漂移：
  //      problem.SetParameterBlockConstant(camera_block_0)
  return false;
}

bool BundleAdjuster::optimize(Reconstruction* reconstruction,
                              bool fix_intrinsics,
                              BundleAdjustmentStats* stats) {
  (void)reconstruction;
  (void)fix_intrinsics;
  (void)stats;
  // TODO(FR-M5-01):
  //   ceres::Solver::Options options;
  //   options.linear_solver_type = ceres::SPARSE_SCHUR;  // 点参数被 Schur 消元
  //   options.max_num_iterations = config_.max_ba_iterations;
  //   options.num_threads = std::max(1, std::thread::hardware_concurrency());
  //   options.minimizer_progress_to_stdout = false;
  //   ceres::Solve(options, &problem, &summary);
  //   把 summary.iterations.size() / initial_cost / final_cost / 耗时写入 stats 与 history_
  //   备选方案: g2o（SparseOptimizer + EdgeProjectXYZ2UV + VertexSE3Expmap）
  return false;
}

bool BundleAdjuster::optimizeLocal(Reconstruction* reconstruction,
                                   const std::vector<CameraId>& local_cameras,
                                   BundleAdjustmentStats* stats) {
  (void)reconstruction;
  (void)local_cameras;
  (void)stats;
  // TODO(FR-M5-02):
  //   1) 只把 local_cameras 的位姿设为非固定
  //   2) 只加入被 local_cameras 观测到的点，以及这些点的全部观测
  //      （注意：固定相机的观测也要加，否则点没有约束）
  //   3) 其余相机 SetParameterBlockConstant
  return false;
}

int BundleAdjuster::removeOutlierObservations(Reconstruction* reconstruction, double max_error_px) {
  (void)reconstruction;
  (void)max_error_px;
  // TODO(FR-M5-04): 遍历所有观测算重投影误差，超过阈值则 removeObservation
  //   统计并 logInfo("剔除 N 条外点观测, 剩余 M")
  //   注意迭代收敛: 一轮剔除后又会有新的外点暴露，至少跑 2 轮
  return 0;
}

int BundleAdjuster::removeWeakPoints(Reconstruction* reconstruction, int min_observations) {
  (void)reconstruction;
  (void)min_observations;
  // TODO(FR-M5-04): 观测数 < min_observations 的三维点直接删掉
  return 0;
}

bool BundleAdjuster::writeLog(const std::string& csv_path) const {
  (void)csv_path;
  // TODO(FR-M5-05): 表头 iteration,initial_cost,final_cost,seconds,converged
  //   供 scripts/plot_metrics.py 画收敛曲线
  return false;
}

const std::vector<BundleAdjustmentStats>& BundleAdjuster::history() const {
  return history_;
}

}  // namespace recon
