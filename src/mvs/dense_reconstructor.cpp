#include "mvs/dense_reconstructor.hpp"

#include <algorithm>
#include <cmath>

#include "core/logger.hpp"
#include "geometry/pose_estimator.hpp"
#include "mvs/point_cloud.hpp"

namespace recon {

DenseReconstructor::DenseReconstructor(const AppConfig& config)
    : config_(config), depth_estimator_(config.mvs) {}

double DenseReconstructor::baselineAngle(const Reconstruction& reconstruction,
                                         CameraId a,
                                         CameraId b) const {
  (void)reconstruction;
  (void)a;
  (void)b;
  // TODO(FR-M6-01): 用两相机光心连线与各自主光轴夹角，估算基线有效性
  //   纯旋转（基线接近 0）时立体匹配必然失败，必须先剔除
  return 0.0;
}

std::vector<CameraId> DenseReconstructor::selectNeighbors(const Reconstruction& reconstruction,
                                                          CameraId reference) const {
  (void)reconstruction;
  (void)reference;
  // TODO(FR-M6-01):
  //   1) 遍历已注册相机，算与 reference 的共视点数（用 reconstruction 的观测）
  //   2) 过滤基线角过小/过大的
  //   3) 按共视点数取前 config_.mvs.neighbor_views 个
  //   4) 一个都没有时返回空，调用方应跳过该参考帧并计数
  return {};
}

bool DenseReconstructor::reconstructView(const Dataset& dataset,
                                         const Reconstruction& reconstruction,
                                         CameraId reference,
                                         const std::vector<CameraId>& neighbors,
                                         ColorCloud* cloud) const {
  (void)dataset;
  (void)reconstruction;
  (void)reference;
  (void)neighbors;
  (void)cloud;
  // TODO(FR-M6-04):
  //   for 每个 neighbor:
  //     读两张灰度图 -> rectify -> estimate -> filterByConsistency -> toPointCloud
  //     融合进 cloud
  //   读图要用与稀疏重建相同的缩放（config.feature.resize_max_edge）
  //   注意 Dataset 里的 ImageRecord 只存路径，实际读图走 ImageLoader::loadGray
  return false;
}

bool DenseReconstructor::reconstruct(const Dataset& dataset,
                                     const Reconstruction& reconstruction,
                                     ColorCloud* cloud,
                                     std::string* error) const {
  (void)dataset;
  (void)reconstruction;
  (void)cloud;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M6-04):
  //   1) 对每张已注册相机调用 selectNeighbors + reconstructView，累加进 all_cloud
  //   2) 体素下采样（config_.mvs.voxel_leaf_size）
  //   3) 统计离群点去除（sor_mean_k / sor_std_ratio）
  //   4) 一点都没有 -> *error = "稠密重建未产生有效点"; return false
  //   降级策略: 只对初始对做一次，保证演示时有输出（详见需求文档 3.6）
  return false;
}

}  // namespace recon
