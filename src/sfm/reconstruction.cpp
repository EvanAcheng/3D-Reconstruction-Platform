#include "sfm/reconstruction.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>

#include "core/logger.hpp"
#include "geometry/pose_estimator.hpp"

namespace recon {

CameraId Reconstruction::addCamera(const Camera& camera) {
  Camera copy = camera;
  copy.id = next_camera_id_++;
  cameras_.emplace(copy.id, copy);
  return copy.id;
}

Point3DId Reconstruction::addPoint(const cv::Point3d& xyz, const cv::Vec3b& color) {
  Point3D point;
  point.id = next_point_id_++;
  point.xyz = xyz;
  point.color = color;
  points_.emplace(point.id, point);
  return point.id;
}

bool Reconstruction::addObservation(Point3DId point_id, CameraId camera_id, const cv::Point2f& xy) {
  (void)point_id;
  (void)camera_id;
  (void)xy;
  // TODO(FR-M4-06): 找到 point，若同一 camera 已有观测则覆盖而非追加
  //   同一张图对同一个点的重复观测会让 BA 的雅可比维度对不上
  return false;
}

bool Reconstruction::removeObservation(Point3DId point_id, CameraId camera_id) {
  (void)point_id;
  (void)camera_id;
  // TODO(FR-M5-04): erase-remove idiom 删除该 (point, camera) 观测
  return false;
}

bool Reconstruction::removePoint(Point3DId point_id) {
  (void)point_id;
  // TODO(FR-M5-04): 先从所有相机的观测里删掉引用，再删点本身
  //   （本期观测挂在点下面，主要是删点；保留接口给后续 track 结构）
  return false;
}

bool Reconstruction::markCameraRegistered(CameraId camera_id,
                                          const cv::Matx33d& R,
                                          const cv::Vec3d& t) {
  (void)camera_id;
  (void)R;
  (void)t;
  // TODO(FR-M4-05): 写入 R/t，registered = true
  return false;
}

const std::map<CameraId, Camera>& Reconstruction::cameras() const {
  return cameras_;
}

const std::map<Point3DId, Point3D>& Reconstruction::points() const {
  return points_;
}

std::map<CameraId, Camera>* Reconstruction::mutableCameras() {
  return &cameras_;
}

std::map<Point3DId, Point3D>* Reconstruction::mutablePoints() {
  return &points_;
}

std::size_t Reconstruction::numRegisteredCameras() const {
  // TODO(FR-M8-01): count_if(cameras_, [](auto& kv){ return kv.second.registered; })
  return 0;
}

std::size_t Reconstruction::numPoints() const {
  return points_.size();
}

std::size_t Reconstruction::numObservations() const {
  // TODO(FR-M8-01): 累加每个点的 observations.size()
  return 0;
}

bool Reconstruction::empty() const {
  return cameras_.empty();
}

cv::Matx34d Reconstruction::projectionMatrix(CameraId camera_id, const cv::Mat& K) const {
  (void)camera_id;
  (void)K;
  // TODO(FR-M4-06): P = K * [R | t]
  return cv::Matx34d();
}

cv::Vec3d Reconstruction::cameraCenter(CameraId camera_id) const {
  (void)camera_id;
  // TODO(FR-M7-02): PoseEstimator::cameraCenter(R, t)，画视锥体要用
  return cv::Vec3d();
}

std::vector<double> Reconstruction::reprojectionErrors() const {
  // TODO(FR-M8-01): 遍历点与观测，用 projectionMatrix + 归一化坐标算像素距离
  //   注意要为每台相机缓存 K，避免内层循环重复构造矩阵
  return {};
}

double Reconstruction::meanReprojectionError() const {
  // TODO(FR-M8-01): 对 reprojectionErrors() 求平均
  return 0.0;
}

double Reconstruction::rmseReprojectionError() const {
  // TODO(FR-M8-01): sqrt(sum(e^2) / N)
  return 0.0;
}

bool Reconstruction::save(const std::string& path, std::string* error) const {
  (void)path;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M4-07): cv::FileStorage 写入 cameras_ / points_（含 observations）
  //   建议同时写一个 version 字段，后续格式变更时能判断能不能读
  return false;
}

bool Reconstruction::load(const std::string& path, std::string* error) {
  (void)path;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M4-07): 读回后重建 next_camera_id_ / next_point_id_，
  //   否则续跑新增的 id 会撞车
  return false;
}

void Reconstruction::clear() {
  cameras_.clear();
  points_.clear();
  next_camera_id_ = 0;
  next_point_id_ = 0;
}

}  // namespace recon
