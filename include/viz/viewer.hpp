#pragma once
// 交互式点云查看器：PCL Visualizer（底层 VTK）。
// 对应需求：FR-M7-01 ~ FR-M7-06
//
// 设计约束：计算与显示分离 —— 先算完再显示，避免边算边渲染导致 VTK 卡死。
// 头文件里只前向声明 PCL 类型，把重量级 include 留在 .cpp。

#include <memory>
#include <string>

#include <opencv2/core.hpp>

#include "core/config.hpp"
#include "mvs/point_cloud.hpp"
#include "sfm/reconstruction.hpp"

namespace pcl {
namespace visualization {
class PCLVisualizer;
}  // namespace visualization
}  // namespace pcl

namespace recon {

class PointCloudViewer {
 public:
  explicit PointCloudViewer(const VizConfig& config,
                            const std::string& window_title = "Reconstruction");
  ~PointCloudViewer();

  bool showCloud(const ColorCloud::ConstPtr& cloud);
  bool showCameras(const Reconstruction& reconstruction, double frustum_scale = 0.1);
  bool showTrajectory(const Reconstruction& reconstruction);

  // 阻塞式交互循环，返回 0 表示正常退出
  int run();

  bool saveScreenshot(const std::string& path) const;

  // 拾取：屏幕坐标 -> 最近点（P1）
  bool pickPoint(int x, int y, pcl::PointXYZRGB* point) const;

 private:
  VizConfig config_;
  std::string window_title_;
  std::shared_ptr<pcl::visualization::PCLVisualizer> viewer_;
};

}  // namespace recon
