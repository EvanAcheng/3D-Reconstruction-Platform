#include "geometry/pose_estimator.hpp"

#include <opencv2/calib3d.hpp>

namespace recon {

PoseEstimator::PoseEstimator(const MatchConfig& config) : config_(config) {}

PnpResult PoseEstimator::solve(const std::vector<cv::Point3d>& object_points,
                               const std::vector<cv::Point2f>& image_points,
                               const CameraModel& camera) const {
  (void)object_points;
  (void)image_points;
  (void)camera;
  // TODO(FR-M4-05):
  //   1) 点数 < 6 直接返回 success=false（EPnP 最少 4 点，但 6 点以下极不稳定）
  //   2) cv::solvePnPRansac(object_points, image_points, camera.matrix(), camera.distortion(),
  //                         rvec, tvec, false, 200, config_.ransac_threshold_px,
  //                         config_.ransac_confidence, inliers, cv::SOLVEPNP_EPNP)
  //   3) 失败时用 cv::SOLVEPNP_ITERATIVE 或 SOLVEPNP_SQPNP 重试一次
  //   4) cv::Rodrigues(rvec, R)；内点率低于 config_.min_inlier_ratio 时判失败
  //      但必须把 inliers/num_inliers 填回去 —— 上层要记录"为什么跳过这张图"
  return PnpResult{};
}

cv::Matx34d PoseEstimator::projectionMatrix(const cv::Matx33d& R,
                                            const cv::Vec3d& t,
                                            const cv::Mat& K) {
  (void)R;
  (void)t;
  (void)K;
  // TODO(FR-M4-05): P = K * [R | t]，返回 3x4 CV_64F
  return cv::Matx34d();
}

cv::Vec3d PoseEstimator::cameraCenter(const cv::Matx33d& R, const cv::Vec3d& t) {
  (void)R;
  (void)t;
  // TODO(FR-M4-05): C = -R^T * t
  return cv::Vec3d();
}

}  // namespace recon
