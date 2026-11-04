#include "mvs/point_cloud.hpp"

#include <pcl/io/ply_io.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/voxel_grid.h>

#include "core/logger.hpp"

namespace recon {

bool PointCloudUtils::fromDepth(const cv::Mat& depth,
                                const cv::Mat& color,
                                const cv::Mat& Q,
                                ColorCloud* cloud) {
  (void)depth;
  (void)color;
  (void)Q;
  (void)cloud;
  // TODO(FR-M6-04):
  //   cv::Mat points3d; cv::reprojectImageTo3D(depth, points3d, Q, true);
  //   遍历像素：depth 有效 && points3d 有限 -> cloud->push_back(pcl::PointXYZRGB)
  //   RGB 从 color 取，注意 OpenCV 是 BGR，要交换 R/B
  //   同时算法向的话用 pcl::NormalEstimation（P1）
  return false;
}

bool PointCloudUtils::voxelDownsample(const ColorCloud& input,
                                      float leaf_size,
                                      ColorCloud* output) {
  (void)input;
  (void)leaf_size;
  (void)output;
  // TODO(FR-M6-04): pcl::VoxelGrid<pcl::PointXYZRGB> vg;
  //   vg.setInputCloud(input.makeShared()); vg.setLeafSize(leaf, leaf, leaf);
  //   vg.filter(*output); 并 logInfo 打印 下采样前/后 点数
  return false;
}

bool PointCloudUtils::statisticalOutlierRemoval(const ColorCloud& input,
                                                int mean_k,
                                                double std_ratio,
                                                ColorCloud* output) {
  (void)input;
  (void)mean_k;
  (void)std_ratio;
  (void)output;
  // TODO(FR-M6-04): pcl::StatisticalOutlierRemoval<pcl::PointXYZRGB>
  //   setMeanK(mean_k) / setStddevMulThresh(std_ratio)
  return false;
}

bool PointCloudUtils::merge(const ColorCloud& lhs, const ColorCloud& rhs, ColorCloud* output) {
  (void)lhs;
  (void)rhs;
  (void)output;
  // TODO(FR-M6-04): *output = lhs; *output += rhs; 注意 points.size() 与 width/height 同步
  return false;
}

CloudStats PointCloudUtils::stats(const ColorCloud& cloud) {
  (void)cloud;
  // TODO(FR-M8-01): pcl::getMinMax3D 或手写循环，填 num_points 与包围盒
  //   报告里要给出场景尺度，方便判断体素尺寸是否合理
  return CloudStats{};
}

bool PointCloudUtils::savePly(const ColorCloud& cloud, const std::string& path) {
  (void)cloud;
  (void)path;
  // TODO(FR-M7-04): pcl::io::savePLYFileBinary(path, cloud)
  //   二进制格式比 ASCII 小很多，百万点必用
  return false;
}

bool PointCloudUtils::loadPly(const std::string& path, ColorCloud* cloud) {
  (void)path;
  (void)cloud;
  // TODO(FR-M7-01): pcl::io::loadPLYFile(path, *cloud)，失败时 logError 并返回 false
  return false;
}

}  // namespace recon
