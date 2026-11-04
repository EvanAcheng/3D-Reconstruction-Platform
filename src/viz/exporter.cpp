#include "viz/exporter.hpp"

#include <fstream>

#include <opencv2/core.hpp>

#include "core/logger.hpp"
#include "geometry/pose_estimator.hpp"

namespace recon {

bool Exporter::exportSparseJson(const Reconstruction& reconstruction, const std::string& path) {
  (void)reconstruction;
  (void)path;
  // TODO(FR-M7-04): 手写 JSON（或引入 nlohmann/json，单头文件即可）
  //   结构: { "cameras": [{id, name, R: [[..]], t: [..]}], "points": [{id, xyz, rgb, observations}] }
  //   Python 端 scripts/view_pointcloud_open3d.py 要能直接读
  return false;
}

bool Exporter::exportPointCloud(const ColorCloud& cloud, const std::string& path) {
  (void)cloud;
  (void)path;
  // TODO(FR-M7-04): 按扩展名分派
  //   .ply -> pcl::io::savePLYFileBinary
  //   .pcd -> pcl::io::savePCDFileBinary
  //   其他 -> logError("不支持的格式")
  return false;
}

bool Exporter::exportTrajectoryTum(const Reconstruction& reconstruction, const std::string& path) {
  (void)reconstruction;
  (void)path;
  // TODO(FR-M7-04): TUM 格式每行 "timestamp tx ty tz qx qy qz qw"
  //   R -> 四元数用 cv::Mat(3,3) + cv::Rodrigues 或 Eigen::Quaterniond(R)
  //   timestamp 没有真值时用相机 id 代替，并在文件头注释说明
  return false;
}

}  // namespace recon
