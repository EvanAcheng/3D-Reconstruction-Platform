#include "sfm/incremental_mapper.hpp"

#include <algorithm>
#include <chrono>

#include "core/logger.hpp"
#include "sfm/optimizer.hpp"

namespace recon {

IncrementalMapper::IncrementalMapper(const AppConfig& config,
                                     const CameraModel& camera,
                                     FeatureCache* cache)
    : config_(config),
      camera_(camera),
      cache_(cache),
      bundle_adjuster_(std::make_unique<BundleAdjuster>(config.sfm)),
      pose_estimator_(config.match) {}

IncrementalMapper::~IncrementalMapper() = default;

bool IncrementalMapper::run(const Dataset& dataset,
                            const ViewGraph& graph,
                            const std::map<ViewPair, std::vector<MatchRecord>>& matches,
                            const std::vector<FeatureSet>& features,
                            Reconstruction* reconstruction,
                            std::string* error) {
  (void)dataset;
  (void)graph;
  (void)matches;
  (void)features;
  (void)reconstruction;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M4-06) 主循环:
  //   for (round = 0; round < config_.sfm.max_rounds; ++round) {
  //     next = selectNextCamera(...);           // 没有可注册的 -> break
  //     收集 next 与已注册点的 2D-3D 对应（需要 track/观测关系）
  //     registerCamera(...) 失败 -> 记入 failed_cameras_，continue
  //     triangulateNewPoints(...)
  //     每轮结束跑 runLocalBundleAdjustment，每 5 轮跑一次全局 BA
  //   }
  //   结束条件: 无新相机可注册 或 迭代到上限
  //   退出时若 numRegisteredCameras() == 0 -> *error = "未能注册任何相机"
  return false;
}

CameraId IncrementalMapper::selectNextCamera(const Reconstruction& reconstruction,
                                             const ViewGraph& graph) const {
  (void)reconstruction;
  (void)graph;
  // TODO(FR-M4-06): 在未注册相机中，按"与已注册相机的共视点数"降序选第一个
  //   都没有共视时返回一个无效 id（比如 0 并配合额外标志），调用方据此结束
  return 0;
}

bool IncrementalMapper::registerCamera(CameraId camera_id,
                                       const std::vector<cv::Point3d>& object_points,
                                       const std::vector<cv::Point2f>& image_points,
                                       Reconstruction* reconstruction) const {
  (void)camera_id;
  (void)object_points;
  (void)image_points;
  (void)reconstruction;
  // TODO(FR-M4-05):
  //   PnpResult r = pose_estimator_.solve(object_points, image_points, camera_);
  //   if (!r.success) { logWarn("相机 x 注册失败，内点数 y"); return false; }
  //   reconstruction->markCameraRegistered(camera_id, r.R, r.t);
  //   只把内点对应的观测写进去（外点写进去会污染 BA）
  return false;
}

std::size_t IncrementalMapper::triangulateNewPoints(
    CameraId camera_id,
    const std::map<ViewPair, std::vector<MatchRecord>>& matches,
    const std::vector<FeatureSet>& features,
    Reconstruction* reconstruction) const {
  (void)camera_id;
  (void)matches;
  (void)features;
  (void)reconstruction;
  // TODO(FR-M4-06):
  //   for 与 camera_id 有边且已注册的相机 other:
  //     取该对的匹配，过滤掉已被现有 3D 点占用的特征索引
  //     Triangulator::triangulatePair(P_camera, P_other, ...) 得到候选点
  //     新点 addPoint + 两条观测 addObservation
  //   返回新增点数，用于日志与"每轮增益"曲线
  return 0;
}

bool IncrementalMapper::runLocalBundleAdjustment(Reconstruction* reconstruction) {
  (void)reconstruction;
  // TODO(FR-M5-02): 以最近注册的相机为中心，取 config_.sfm.local_ba_window 个共视邻居
  //   作为 local_cameras，调用 bundle_adjuster_->optimizeLocal
  //   时间预算到了就跳过（局部 BA 是为了稳定，不是每轮都值得跑）
  return false;
}

bool IncrementalMapper::runGlobalBundleAdjustment(Reconstruction* reconstruction) {
  (void)reconstruction;
  // TODO(FR-M5-02): bundle_adjuster_->optimize(reconstruction, config_.sfm.fix_intrinsics, &stats)
  //   然后 removeOutlierObservations + removeWeakPoints，再优化一次
  //   把 stats 写入 ba_log.csv（FR-M5-05）
  return false;
}

int IncrementalMapper::countCovisibility(CameraId camera_id,
                                         const Reconstruction& reconstruction,
                                         const ViewGraph& graph) const {
  (void)camera_id;
  (void)reconstruction;
  (void)graph;
  // TODO(FR-M4-06): 统计 camera_id 的邻居中已注册的数量，以及共享 3D 点的数量
  return 0;
}

bool IncrementalMapper::saveState(const std::string& path, std::string* error) const {
  (void)path;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M4-07): 写 reconstruction + 已处理相机列表，供 --resume
  return false;
}

bool IncrementalMapper::loadState(const std::string& path, std::string* error) {
  (void)path;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M4-07): 读回状态；与当前数据集图像数不一致时要拒绝并提示
  return false;
}

}  // namespace recon
