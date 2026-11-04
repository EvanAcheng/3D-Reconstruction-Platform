#include "mvs/depth_estimator.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/cudastereo.hpp>
#include <opencv2/imgproc.hpp>

#include "core/logger.hpp"

namespace recon {

DepthEstimator::DepthEstimator(const MvsConfig& config) : config_(config) {}

bool DepthEstimator::rectify(const CameraModel& camera,
                             const cv::Matx33d& R_ref, const cv::Vec3d& t_ref,
                             const cv::Matx33d& R_tgt, const cv::Vec3d& t_tgt,
                             StereoRectification* rectification) const {
  (void)camera;
  (void)R_ref;
  (void)t_ref;
  (void)R_tgt;
  (void)t_tgt;
  (void)rectification;
  // TODO(FR-M6-01):
  //   1) 相对位姿: R = R_tgt * R_ref^T, t = t_tgt - R * t_ref
  //   2) cv::stereoRectify(K, dist, K, dist, imageSize, R, t,
  //                        R1, R2, P1, P2, Q, cv::CALIB_ZERO_DISPARITY, 0, ...)
  //   3) cv::initUndistortRectifyMap(K, dist, R1, P1, size, CV_32FC1, mapx, mapy)
  //   4) 记录 valid ROI（stereoRectify 的第 9、10 个输出参数），
  //      后续只在这个区域内取视差，能省掉大量无效计算
  return false;
}

DepthMap DepthEstimator::estimate(const cv::Mat& rectified_left,
                                  const cv::Mat& rectified_right,
                                  const StereoRectification& rectification) const {
  (void)rectified_left;
  (void)rectified_right;
  (void)rectification;
  // TODO(FR-M6-02):
  //   CPU: cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(
  //          0, config_.num_disparities, config_.block_size,
  //          config_.p1, config_.p2, 1, 63, 10, 100, cv::StereoSGBM::MODE_SGBM_3WAY);
  //        sgbm->compute(left, right, disparity);  // CV_16S，除以 16 得亚像素视差
  //   GPU: cv::cuda::StereoSGBM（接口类似，需 upload/download）
  //   再由 disparity 算 depth: Z = f * B / d，f/B 从 P1/P2 取（P2(0,3) / P2(0,0) 等）
  //   视差 <= 0 的像素写入 mask = 0
  return DepthMap{};
}

bool DepthEstimator::estimateCuda(const cv::Mat& left,
                                  const cv::Mat& right,
                                  DepthMap* depth_map) const {
  (void)left;
  (void)right;
  (void)depth_map;
  // TODO(FR-M6-02 / NFR-PERF-03): cv::cuda::createStereoSGM 或 cv::cuda::StereoSGBM
  //   实测加速比写进 metrics，报告需要"≥3×"的支撑数据
  return false;
}

bool DepthEstimator::filterByConsistency(DepthMap* depth_map, int max_diff) const {
  (void)depth_map;
  (void)max_diff;
  // TODO(FR-M6-03):
  //   1) 左右一致性: 用右视图做参考再算一遍视差，|d_l - d_r(x - d_l)| < max_diff
  //      （若不想算两遍，可只做唯一性检查：第二小代价/最小代价 > 阈值）
  //   2) cv::medianBlur 去椒盐噪声
  //   3) 小连通域剔除（cv::connectedComponents 后按面积过滤）
  //   更新 mask 与 valid_pixels
  return false;
}

bool DepthEstimator::toPointCloud(const DepthMap& depth_map,
                                  const cv::Mat& color,
                                  const StereoRectification& rectification,
                                  ColorCloud* cloud) const {
  (void)depth_map;
  (void)color;
  (void)rectification;
  (void)cloud;
  // TODO(FR-M6-04): 直接用 PointCloudUtils::fromDepth(depth, color, rectification.Q, cloud)
  //   注意 Q 是在"校正后的坐标系"下定义的，深度的单位要与 Q 的量纲一致
  return false;
}

}  // namespace recon
