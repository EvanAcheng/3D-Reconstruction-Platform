#pragma once
// 深度图估计：立体校正 + SGBM（CPU/CUDA）+ 左右一致性检查。
// 对应需求：FR-M6-01 ~ FR-M6-03
//
// 3 周内的定位：不实现 PatchMatch，用"已知位姿 + 立体匹配 + 多视融合"换可靠性，
// 这一点要在报告"方案对比"里写清楚（SGBM vs PatchMatch 的取舍）。

#include <string>

#include <opencv2/core.hpp>

#include "core/config.hpp"
#include "core/types.hpp"
#include "io/camera_model.hpp"
#include "mvs/point_cloud.hpp"

namespace recon {

// 一对图像的校正参数与映射表
struct StereoRectification {
  cv::Mat R1, R2, P1, P2, Q;
  cv::Mat map_left_x, map_left_y;
  cv::Mat map_right_x, map_right_y;
  int valid_roi_x = 0;
  int valid_roi_y = 0;
  int valid_roi_width = 0;
  int valid_roi_height = 0;
};

struct DepthMap {
  cv::Mat depth;      // CV_32F，单位与场景尺度一致
  cv::Mat disparity;  // CV_16S / CV_32F
  cv::Mat mask;       // CV_8U，1 表示有效
  std::size_t valid_pixels = 0;
};

class DepthEstimator {
 public:
  explicit DepthEstimator(const MvsConfig& config);

  // 由两帧位姿算 stereoRectify 参数，并预计算两次 remap 的映射表
  bool rectify(const CameraModel& camera,
               const cv::Matx33d& R_ref, const cv::Vec3d& t_ref,
               const cv::Matx33d& R_tgt, const cv::Vec3d& t_tgt,
               StereoRectification* rectification) const;

  // 在已校正图上算视差/深度
  DepthMap estimate(const cv::Mat& rectified_left,
                    const cv::Mat& rectified_right,
                    const StereoRectification& rectification) const;

  // 左右一致性 + 中值滤波 + 唯一性检查
  bool filterByConsistency(DepthMap* depth_map, int max_diff = 1) const;

  bool toPointCloud(const DepthMap& depth_map,
                    const cv::Mat& color,
                    const StereoRectification& rectification,
                    ColorCloud* cloud) const;

 private:
  bool estimateCuda(const cv::Mat& left, const cv::Mat& right, DepthMap* depth_map) const;

  MvsConfig config_;
};

}  // namespace recon
