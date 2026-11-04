#pragma once
// 增量式 SfM 主循环：选图 -> PnP 注册 -> 三角化新点 -> 局部 BA -> 重复。
// 对应需求：FR-M4-06（主循环）、FR-M4-07（断点续跑）、NFR-REL-01（单图失败不中断）

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "core/config.hpp"
#include "core/types.hpp"
#include "feature/feature_cache.hpp"
#include "feature/feature_extractor.hpp"
#include "geometry/pose_estimator.hpp"
#include "io/camera_model.hpp"
#include "io/dataset.hpp"
#include "sfm/reconstruction.hpp"
#include "sfm/view_graph.hpp"

namespace recon {

class BundleAdjuster;

class IncrementalMapper {
 public:
  IncrementalMapper(const AppConfig& config,
                    const CameraModel& camera,
                    FeatureCache* cache);
  ~IncrementalMapper();

  // 主入口：失败时写 error，已注册的部分结果仍保留在 reconstruction 中
  bool run(const Dataset& dataset,
           const ViewGraph& graph,
           const std::map<ViewPair, std::vector<MatchRecord>>& matches,
           const std::vector<FeatureSet>& features,
           Reconstruction* reconstruction,
           std::string* error);

  // 选下一张待注册的图像：与已注册图像的共视点数最多
  CameraId selectNextCamera(const Reconstruction& reconstruction,
                            const ViewGraph& graph) const;

  // 单张注册：2D-3D 对应 -> PnP -> 写入位姿
  bool registerCamera(CameraId camera_id,
                      const std::vector<cv::Point3d>& object_points,
                      const std::vector<cv::Point2f>& image_points,
                      Reconstruction* reconstruction) const;

  // 用新注册的相机把已有 2D 匹配三角化成新的三维点
  std::size_t triangulateNewPoints(CameraId camera_id,
                                   const std::map<ViewPair, std::vector<MatchRecord>>& matches,
                                   const std::vector<FeatureSet>& features,
                                   Reconstruction* reconstruction) const;

  bool runLocalBundleAdjustment(Reconstruction* reconstruction);
  bool runGlobalBundleAdjustment(Reconstruction* reconstruction);

  bool saveState(const std::string& path, std::string* error) const;
  bool loadState(const std::string& path, std::string* error);

 private:
  // 统计某张图与已注册图像的共视点数
  int countCovisibility(CameraId camera_id,
                        const Reconstruction& reconstruction,
                        const ViewGraph& graph) const;

  AppConfig config_;
  CameraModel camera_;
  FeatureCache* cache_ = nullptr;
  std::unique_ptr<BundleAdjuster> bundle_adjuster_;
  PoseEstimator pose_estimator_;
};

}  // namespace recon
