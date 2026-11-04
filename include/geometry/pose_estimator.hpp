#pragma once
// PnP：用 2D-3D 对应估计新相机位姿（EPnP + RANSAC）。
// 对应需求：FR-M4-05；注册率指标 NFR-ACC-02

#include <vector>

#include <opencv2/core.hpp>

#include "core/config.hpp"
#include "core/types.hpp"
#include "io/camera_model.hpp"

namespace recon {

struct PnpResult {
  bool success = false;
  cv::Matx33d R = cv::Matx33d::eye();
  cv::Vec3d t{0.0, 0.0, 0.0};
  cv::Mat inliers;      // 1 x N, CV_8U
  int num_inliers = 0;
  double inlier_ratio = 0.0;
};

class PoseEstimator {
 public:
  explicit PoseEstimator(const MatchConfig& config);

  // 注册新图像：内点不足时返回 success = false，由调用方决定跳过
  PnpResult solve(const std::vector<cv::Point3d>& object_points,
                  const std::vector<cv::Point2f>& image_points,
                  const CameraModel& camera) const;

  static cv::Matx34d projectionMatrix(const cv::Matx33d& R,
                                      const cv::Vec3d& t,
                                      const cv::Mat& K);

  // 相机光心在世界坐标下的位置：C = -R^T * t
  static cv::Vec3d cameraCenter(const cv::Matx33d& R, const cv::Vec3d& t);

 private:
  MatchConfig config_;
};

}  // namespace recon
