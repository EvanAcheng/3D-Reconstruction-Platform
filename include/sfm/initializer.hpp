#pragma once
// 初始化：挑选初始像对，分解 E、筛选四解、三角化出第一批三维点。
// 这是整条链路最容易崩的一步，单独成类以便用合成数据单测。
// 对应需求：FR-M4-01 ~ FR-M4-04

#include <map>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "core/config.hpp"
#include "core/types.hpp"
#include "feature/feature_extractor.hpp"
#include "io/camera_model.hpp"
#include "sfm/reconstruction.hpp"
#include "sfm/view_graph.hpp"

namespace recon {

struct InitialPair {
  CameraId first = 0;
  CameraId second = 0;
  int num_inliers = 0;
  double median_parallax_deg = 0.0;
  double score = 0.0;
  bool success = false;

  // 初始化后得到的相对位姿
  cv::Matx33d R = cv::Matx33d::eye();
  cv::Vec3d t{0.0, 0.0, 0.0};
};

class Initializer {
 public:
  Initializer(const AppConfig& config, const CameraModel& camera);

  // 在视图图里打分：内点数、视差、非退化，综合素质最高的一对
  InitialPair selectPair(const ViewGraph& graph,
                         const std::vector<FeatureSet>& features,
                         const std::map<ViewPair, std::vector<MatchRecord>>& matches) const;

  // 建图：写出两台相机位姿 + 三角化得到的三维点
  bool initialize(const InitialPair& pair,
                  const std::vector<FeatureSet>& features,
                  const std::map<ViewPair, std::vector<MatchRecord>>& matches,
                  Reconstruction* reconstruction,
                  std::string* error) const;

 private:
  // 平面场景判定：H 的内点明显多于 F 时判为退化
  bool isHomographyDominant(const std::vector<cv::Point2f>& points_a,
                            const std::vector<cv::Point2f>& points_b) const;

  AppConfig config_;
  CameraModel camera_;
};

}  // namespace recon
