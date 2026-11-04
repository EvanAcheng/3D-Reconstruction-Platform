#pragma once
// 三角化：线性三角化（DLT + SVD）、视差角、重投影误差。
// 对应需求：FR-M4-04；精度指标 NFR-ACC-01

#include <vector>

#include <opencv2/core.hpp>

#include "core/types.hpp"

namespace recon {

struct TriangulationResult {
  bool success = false;
  cv::Point3d point;
  double reprojection_error = 0.0;
  double parallax_deg = 0.0;
};

class Triangulator {
 public:
  // 单点三角化：P1/P2 为 3x4 投影矩阵
  static TriangulationResult triangulate(const cv::Matx34d& P1,
                                         const cv::Matx34d& P2,
                                         const cv::Point2f& point_a,
                                         const cv::Point2f& point_b);

  // 两条光线的夹角，用于剔除"远点/退化点"
  static double parallaxAngle(const cv::Vec3d& point,
                              const cv::Vec3d& center_a,
                              const cv::Vec3d& center_b);

  static double reprojectionError(const cv::Matx34d& P,
                                  const cv::Point3d& point,
                                  const cv::Point2f& observed);

  static bool isInFront(const cv::Matx34d& P, const cv::Point3d& point);

  // 批量三角化：把通过的 3D 点写入 points，对应下标写入 source_indices
  static std::size_t triangulatePair(const cv::Matx34d& P1,
                                     const cv::Matx34d& P2,
                                     const std::vector<cv::Point2f>& points_a,
                                     const std::vector<cv::Point2f>& points_b,
                                     double max_error_px,
                                     double min_angle_deg,
                                     std::vector<cv::Point3d>* points,
                                     std::vector<int>* source_indices);
};

}  // namespace recon
