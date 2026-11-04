#include "viz/viewer.hpp"

#include <pcl/visualization/pcl_visualizer.h>

#include "core/logger.hpp"
#include "geometry/pose_estimator.hpp"

namespace recon {

PointCloudViewer::PointCloudViewer(const VizConfig& config, const std::string& window_title)
    : config_(config), window_title_(window_title.empty() ? config.window_title : window_title) {
  // TODO(FR-M7-01):
  //   viewer_ = std::make_shared<pcl::visualization::PCLVisualizer>(window_title_);
  //   viewer_->setBackgroundColor(0.05, 0.05, 0.08);
  //   viewer_->addCoordinateSystem(0.1);
  //   viewer_->initCameraParameters();
  //   viewer_->setSize(config_.screen_width, config_.screen_height);
}

PointCloudViewer::~PointCloudViewer() = default;

bool PointCloudViewer::showCloud(const ColorCloud::ConstPtr& cloud) {
  (void)cloud;
  // TODO(FR-M7-01):
  //   viewer_->removeAllPointClouds();
  //   pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(cloud);
  //   viewer_->addPointCloud<pcl::PointXYZRGB>(cloud, rgb, "cloud");
  //   viewer_->setPointCloudRenderingProperties(PCL_VISUALIZER_POINT_SIZE, config_.point_size, "cloud");
  return false;
}

bool PointCloudViewer::showCameras(const Reconstruction& reconstruction, double frustum_scale) {
  (void)reconstruction;
  (void)frustum_scale;
  // TODO(FR-M7-02):
  //   对每台已注册相机：用 4 个角点 + 光心构造视锥的 8 条线段，
  //   viewer_->addLine<pcl::PointXYZ>(a, b, r, g, b, "cam_<id>_<k>")
  //   光心位置用 PoseEstimator::cameraCenter
  return false;
}

bool PointCloudViewer::showTrajectory(const Reconstruction& reconstruction) {
  (void)reconstruction;
  // TODO(FR-M7-02): 按相机 id 顺序连成折线（addPolygon），相机构型杂乱时也好看
  return false;
}

int PointCloudViewer::run() {
  // TODO(FR-M7-03): while (!viewer_->wasStopped()) { viewer_->spinOnce(30); }
  //   键盘回调: r 复位视角、s 截图、c 切换相机显隐、b 切换包围盒
  return 0;
}

bool PointCloudViewer::saveScreenshot(const std::string& path) const {
  (void)path;
  // TODO(FR-M7-03): viewer_->saveScreenshot(path)，注意窗口必须是当前活动窗口
  return false;
}

bool PointCloudViewer::pickPoint(int x, int y, pcl::PointXYZRGB* point) const {
  (void)x;
  (void)y;
  (void)point;
  // TODO(FR-M7-05, P1): 先用 viewer_->getPointCloudRenderingProperties 拿到变换矩阵，
  //   再做屏幕坐标 -> 世界坐标的反投影或用 pcl::visualization 的 picking 事件回调
  return false;
}

}  // namespace recon
