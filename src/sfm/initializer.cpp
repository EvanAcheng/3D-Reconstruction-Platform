#include "sfm/initializer.hpp"

#include <algorithm>
#include <cmath>

#include <opencv2/calib3d.hpp>

#include "core/logger.hpp"
#include "geometry/epipolar.hpp"
#include "geometry/triangulation.hpp"

namespace recon {

Initializer::Initializer(const AppConfig& config, const CameraModel& camera)
    : config_(config), camera_(camera) {}

bool Initializer::isHomographyDominant(const std::vector<cv::Point2f>& points_a,
                                       const std::vector<cv::Point2f>& points_b) const {
  (void)points_a;
  (void)points_b;
  // TODO(FR-M4-01 退化检测):
  //   cv::findHomography(points_a, points_b, cv::RANSAC, 3.0, h_mask)
  //   cv::findFundamentalMat(..., f_mask)
  //   若 H 内点数 > F 内点数 * config_.match.homography_dominance_ratio，判为平面场景
  //   平面场景下 E 分解出的 t 不可信，必须跳过这对
  return false;
}

InitialPair Initializer::selectPair(const ViewGraph& graph,
                                    const std::vector<FeatureSet>& features,
                                    const std::map<ViewPair, std::vector<MatchRecord>>& matches) const {
  (void)graph;
  (void)features;
  (void)matches;
  // TODO(FR-M4-01) 打分策略:
  //   for 每条边:
  //     1) edge.degenerate -> 跳过
  //     2) 取该对的匹配点，用已有 F 做本质矩阵分解 + 三角化，求视差中位数
  //     3) 视差 < 1.5° 或 > 30°（基线过大，匹配率低）都降权
  //     4) score = num_inliers * f(median_parallax)，取最高分
  //   返回的 InitialPair 里要带 num_inliers / median_parallax_deg / score 便于写日志
  return InitialPair{};
}

bool Initializer::initialize(const InitialPair& pair,
                             const std::vector<FeatureSet>& features,
                             const std::map<ViewPair, std::vector<MatchRecord>>& matches,
                             Reconstruction* reconstruction,
                             std::string* error) const {
  (void)pair;
  (void)features;
  (void)matches;
  (void)reconstruction;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M4-02 ~ FR-M4-04):
  //   1) 从 matches 取该对的匹配，转成两组 cv::Point2f
  //   2) F -> EssentialEstimator::fromFundamental -> decompose -> selectByCheirality
  //   3) 冗余校验: 用 cv::recoverPose（它内部做 cheirality）对照一次，不一致要 WARN
  //   4) Triangulator::triangulatePair 得到第一批 3D 点
  //   5) 建两台 Camera（第一台 R=I, t=0），addPoint + addObservation
  //   6) 一个点都没三角化出来 -> *error = "初始对退化"; return false
  return false;
}

}  // namespace recon
