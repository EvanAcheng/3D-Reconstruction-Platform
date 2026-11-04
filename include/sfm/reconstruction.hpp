#pragma once
// 重建结果的数据中心：相机位姿 + 三维点 + 观测 + 重投影误差统计。
// 只做数据结构和容器操作，不包含任何算法 —— 算法在各 Solver 里。
// 对应需求：FR-M4-07（序列化/续跑）、FR-M8-01（指标来源）

#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "core/types.hpp"

namespace recon {

struct Camera {
  CameraId id = 0;
  std::string image_name;
  cv::Matx33d R = cv::Matx33d::eye();
  cv::Vec3d t{0.0, 0.0, 0.0};
  bool registered = false;
};

struct Point3D {
  Point3DId id = 0;
  cv::Point3d xyz;
  cv::Vec3b color{255, 255, 255};
  std::vector<Observation> observations;
};

class Reconstruction {
 public:
  Reconstruction() = default;

  CameraId addCamera(const Camera& camera);
  Point3DId addPoint(const cv::Point3d& xyz, const cv::Vec3b& color = cv::Vec3b(255, 255, 255));

  bool addObservation(Point3DId point_id, CameraId camera_id, const cv::Point2f& xy);
  bool removeObservation(Point3DId point_id, CameraId camera_id);
  bool removePoint(Point3DId point_id);
  bool markCameraRegistered(CameraId camera_id, const cv::Matx33d& R, const cv::Vec3d& t);

  const std::map<CameraId, Camera>& cameras() const;
  const std::map<Point3DId, Point3D>& points() const;
  std::map<CameraId, Camera>* mutableCameras();
  std::map<Point3DId, Point3D>* mutablePoints();

  std::size_t numRegisteredCameras() const;
  std::size_t numPoints() const;
  std::size_t numObservations() const;
  bool empty() const;

  cv::Matx34d projectionMatrix(CameraId camera_id, const cv::Mat& K) const;
  cv::Vec3d cameraCenter(CameraId camera_id) const;

  // 误差统计：两次 BA 之间用它判断是否收敛
  std::vector<double> reprojectionErrors() const;
  double meanReprojectionError() const;
  double rmseReprojectionError() const;

  // 中间结果落盘 / 续跑
  bool save(const std::string& path, std::string* error) const;
  bool load(const std::string& path, std::string* error);
  void clear();

 private:
  std::map<CameraId, Camera> cameras_;
  std::map<Point3DId, Point3D> points_;
  CameraId next_camera_id_ = 0;
  Point3DId next_point_id_ = 0;
};

}  // namespace recon
