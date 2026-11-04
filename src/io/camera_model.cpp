#include "io/camera_model.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

namespace recon {

CameraModel::CameraModel(const Intrinsics& intrinsics) : intrinsics_(intrinsics) {}

bool CameraModel::fromCalibrationFile(const std::string& path, CameraModel* model, std::string* error) {
  (void)path;
  (void)model;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M2-02): cv::FileStorage fs(path, cv::FileStorage::READ)
  //   fs["camera_matrix"] >> K;      // 3x3
  //   fs["dist_coeffs"]   >> dist;   // 1x4 或 1x5
  //   fs["image_width"] / fs["image_height"] >> 分辨率，缺省时由加载图像时补
  //   拆出 fx/fy/cx/cy/k1/k2/p1/p2 填进 Intrinsics
  //   校验: fx<=0 || fy<=0 -> *error = "内参非法"; return false
  return false;
}

bool CameraModel::fromExif(const std::string& image_path, int width, int height, CameraModel* model) {
  (void)image_path;
  (void)width;
  (void)height;
  (void)model;
  // TODO(FR-M2-02): 打开同目录的 EXIF（cv::imread 不直接给 EXIF，可读原始字节或用
  //   cv::imdecode 后自行解析；简化版: 读 config 里的 focal_length_mm + sensor_width_mm 估算）
  //   兜底方案：假设水平 FOV ≈ 60°，fx = 0.5 * width / tan(FOV/2)，cy = height/2
  //   注意：EXIF 推断精度差，日志里必须 WARN 提示"建议先标定"
  return false;
}

bool CameraModel::isValid() const {
  // TODO(FR-M2-02): fx > 0 && fy > 0 && width > 0 && height > 0
  return intrinsics_.fx > 0.0 && intrinsics_.fy > 0.0;
}

const Intrinsics& CameraModel::intrinsics() const {
  return intrinsics_;
}

cv::Mat CameraModel::matrix() const {
  // TODO(FR-M2-02): cv::Mat(3, 3, CV_64F) 填入 fx 0 cx / 0 fy cy / 0 0 1
  return cv::Mat();
}

cv::Mat CameraModel::distortion() const {
  // TODO(FR-M2-02): 填入 (k1, k2, p1, p2)，全零时返回空 Mat 也可以
  return cv::Mat();
}

cv::Point2d CameraModel::project(const cv::Point3d& point_in_camera) const {
  (void)point_in_camera;
  // TODO(FR-M2-02): 优先用 cv::projectPoints（含畸变），或手写
  //   u = fx * X/Z + cx, v = fy * Y/Z + cy
  return cv::Point2d();
}

cv::Point3d CameraModel::backProject(const cv::Point2d& pixel, double depth) const {
  (void)pixel;
  (void)depth;
  // TODO(FR-M6-04): cv::undistortPoints 单点畸变校正后再反投影
  //   X = (u - cx) * depth / fx, Y = (v - cy) * depth / fy, Z = depth
  return cv::Point3d();
}

cv::Matx34d CameraModel::projectionMatrix(const cv::Matx33d& R, const cv::Vec3d& t) const {
  (void)R;
  (void)t;
  // TODO(FR-M4-04): P = K * [R | t]，K 用 matrix()，返回 3x4
  return cv::Matx34d();
}

}  // namespace recon
