#pragma once
// 点云工具：深度图反投影、体素下采样、统计离群点去除、PLY 读写。
// 对应需求：FR-M6-04（点云融合与去噪）、FR-M7-04（导出）

#include <cstddef>
#include <string>

#include <opencv2/core.hpp>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

namespace recon {

using ColorCloud = pcl::PointCloud<pcl::PointXYZRGB>;

struct CloudStats {
  std::size_t num_points = 0;
  pcl::PointXYZRGB min_point;
  pcl::PointXYZRGB max_point;
};

class PointCloudUtils {
 public:
  // 深度图 + 彩色图 + 重投影矩阵 Q -> 世界坐标点云
  static bool fromDepth(const cv::Mat& depth,
                        const cv::Mat& color,
                        const cv::Mat& Q,
                        ColorCloud* cloud);

  static bool voxelDownsample(const ColorCloud& input, float leaf_size, ColorCloud* output);
  static bool statisticalOutlierRemoval(const ColorCloud& input,
                                        int mean_k,
                                        double std_ratio,
                                        ColorCloud* output);
  static bool merge(const ColorCloud& lhs, const ColorCloud& rhs, ColorCloud* output);

  static CloudStats stats(const ColorCloud& cloud);

  static bool savePly(const ColorCloud& cloud, const std::string& path);
  static bool loadPly(const std::string& path, ColorCloud* cloud);
};

}  // namespace recon
