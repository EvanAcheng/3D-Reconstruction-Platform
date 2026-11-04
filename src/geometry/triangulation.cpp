#include "geometry/triangulation.hpp"

#include <cmath>

#include <opencv2/calib3d.hpp>

namespace recon {

TriangulationResult Triangulator::triangulate(const cv::Matx34d& P1,
                                              const cv::Matx34d& P2,
                                              const cv::Point2f& point_a,
                                              const cv::Point2f& point_b) {
  (void)P1;
  (void)P2;
  (void)point_a;
  (void)point_b;
  // TODO(FR-M4-04): 线性三角化 DLT
  //   A = [u1*P1.row(2) - P1.row(0);
  //        v1*P1.row(2) - P1.row(1);
  //        u2*P2.row(2) - P2.row(0);
  //        v2*P2.row(2) - P2.row(1)]
  //   cv::SVD::compute(A, w, u, vt)，X = vt.row(3)，齐次化 X /= X(3)
  //   W <= 0 或 |X(3)| 过小时判失败
  //   顺便算 reprojection_error 与 parallax_deg 填进结果（调用方要用它筛选）
  return TriangulationResult{};
}

double Triangulator::parallaxAngle(const cv::Vec3d& point,
                                   const cv::Vec3d& center_a,
                                   const cv::Vec3d& center_b) {
  (void)point;
  (void)center_a;
  (void)center_b;
  // TODO(FR-M4-04): 返回两方向向量夹角（度）：
  //   cos = (P-Ca)·(P-Cb) / (|P-Ca| |P-Cb|)，注意 clamp 到 [-1, 1] 防止 acos 出 NaN
  return 0.0;
}

double Triangulator::reprojectionError(const cv::Matx34d& P,
                                       const cv::Point3d& point,
                                       const cv::Point2f& observed) {
  (void)P;
  (void)point;
  (void)observed;
  // TODO(FR-M4-04): p = P * [X,1]，归一化后与 observed 求欧氏距离
  return 0.0;
}

bool Triangulator::isInFront(const cv::Matx34d& P, const cv::Point3d& point) {
  (void)P;
  (void)point;
  // TODO(FR-M4-03): 投影后齐次坐标的第三分量 w 必须为正（点在相机前方）
  return false;
}

std::size_t Triangulator::triangulatePair(const cv::Matx34d& P1,
                                          const cv::Matx34d& P2,
                                          const std::vector<cv::Point2f>& points_a,
                                          const std::vector<cv::Point2f>& points_b,
                                          double max_error_px,
                                          double min_angle_deg,
                                          std::vector<cv::Point3d>* points,
                                          std::vector<int>* source_indices) {
  (void)P1;
  (void)P2;
  (void)points_a;
  (void)points_b;
  (void)max_error_px;
  (void)min_angle_deg;
  (void)points;
  (void)source_indices;
  // TODO(FR-M4-04): 逐对调用 triangulate，三重过滤：
  //   1) reprojection_error <= max_error_px（两相机都要满足）
  //   2) parallax_deg >= min_angle_deg（视差太小 -> 深度不可信）
  //   3) 两个相机都 isInFront
  //   通过的点 push 到 points，原始下标 push 到 source_indices
  return 0;
}

}  // namespace recon
