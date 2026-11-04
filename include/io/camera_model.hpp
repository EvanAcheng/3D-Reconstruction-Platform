#pragma once
// 相机模型：内参矩阵、畸变系数、投影与反投影。
// 对应需求：FR-M2-02（内参来源与校验）

#include <string>

#include <opencv2/core.hpp>

#include "core/types.hpp"

namespace recon {

class CameraModel {
 public:
  CameraModel() = default;
  explicit CameraModel(const Intrinsics& intrinsics);

  // 从标定文件读取（OpenCV FileStorage 格式：camera_matrix / dist_coeffs）
  static bool fromCalibrationFile(const std::string& path, CameraModel* model, std::string* error);

  // 从图像 EXIF 推断焦距（无标定文件时的兜底，精度较差）
  static bool fromExif(const std::string& image_path, int width, int height, CameraModel* model);

  bool isValid() const;
  const Intrinsics& intrinsics() const;

  cv::Mat matrix() const;      // 3x3 CV_64F
  cv::Mat distortion() const;  // 1x4 或 1x5

  // 相机坐标系点 -> 像素坐标
  cv::Point2d project(const cv::Point3d& point_in_camera) const;
  // 像素坐标 + 深度 -> 相机坐标系点
  cv::Point3d backProject(const cv::Point2d& pixel, double depth) const;

  // P = K * [R | t]
  cv::Matx34d projectionMatrix(const cv::Matx33d& R, const cv::Vec3d& t) const;

 private:
  Intrinsics intrinsics_;
};

}  // namespace recon
