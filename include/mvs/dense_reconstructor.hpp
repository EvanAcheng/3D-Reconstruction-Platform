#pragma once
// 稠密重建调度：为每张参考帧选邻域帧 -> 估计深度 -> 反投影 -> 全局融合去噪。
// 对应需求：FR-M6-04；降级策略见需求文档 3.6 节

#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "core/config.hpp"
#include "core/types.hpp"
#include "io/dataset.hpp"
#include "mvs/depth_estimator.hpp"
#include "mvs/point_cloud.hpp"
#include "sfm/reconstruction.hpp"

namespace recon {

class DenseReconstructor {
 public:
  explicit DenseReconstructor(const AppConfig& config);

  // 邻域帧：共视点数 + 基线角，排除纯旋转/基线过小的组合
  std::vector<CameraId> selectNeighbors(const Reconstruction& reconstruction,
                                        CameraId reference) const;

  // 单个参考视角的稠密重建
  bool reconstructView(const Dataset& dataset,
                       const Reconstruction& reconstruction,
                       CameraId reference,
                       const std::vector<CameraId>& neighbors,
                       ColorCloud* cloud) const;

  // 全场景：逐帧重建 + 体素下采样 + 统计滤波
  bool reconstruct(const Dataset& dataset,
                   const Reconstruction& reconstruction,
                   ColorCloud* cloud,
                   std::string* error) const;

 private:
  // 两帧之间的基线角度估计
  double baselineAngle(const Reconstruction& reconstruction,
                       CameraId a,
                       CameraId b) const;

  AppConfig config_;
  DepthEstimator depth_estimator_;
};

}  // namespace recon
