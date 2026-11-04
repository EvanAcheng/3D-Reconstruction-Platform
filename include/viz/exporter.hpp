#pragma once
// 结果导出：稀疏重建 JSON、点云 PLY/PCD、相机轨迹（TUM 格式）。
// 对应需求：FR-M7-04（导出）、FR-M8-01（结果可被外部工具打开）

#include <string>

#include "mvs/point_cloud.hpp"
#include "sfm/reconstruction.hpp"

namespace recon {

class Exporter {
 public:
  // 稀疏结构：相机位姿 + 三维点 + 观测，供 Python 画图或二次开发
  static bool exportSparseJson(const Reconstruction& reconstruction, const std::string& path);

  // 点云：按扩展名选 PLY 或 PCD 写入
  static bool exportPointCloud(const ColorCloud& cloud, const std::string& path);

  // TUM 轨迹格式：timestamp tx ty tz qx qy qz qw
  static bool exportTrajectoryTum(const Reconstruction& reconstruction, const std::string& path);
};

}  // namespace recon
