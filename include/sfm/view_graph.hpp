#pragma once
// 视图图（view graph）：记录哪些图像之间有可信的几何关系。
// 对应需求：FR-M3-04（视图图构建与剪枝）、FR-M4-01（初始像对选择的输入）

#include <map>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "core/types.hpp"

namespace recon {

struct ViewEdge {
  CameraId lhs = 0;
  CameraId rhs = 0;
  int num_matches = 0;
  int num_inliers = 0;
  double inlier_ratio = 0.0;
  bool degenerate = false;              // 单应主导，不宜作为初始化对
  cv::Matx33d F = cv::Matx33d::zeros();  // 该对的基础矩阵
};

class ViewGraph {
 public:
  void addEdge(const ViewEdge& edge);

  const std::vector<ViewEdge>& edges() const;
  std::size_t numEdges() const;

  bool hasEdge(CameraId a, CameraId b) const;
  const ViewEdge* findEdge(CameraId a, CameraId b) const;

  // 按内点数排序，取前 k 个邻居
  std::vector<ViewEdge> neighbors(CameraId camera_id, int k) const;

  std::vector<CameraId> cameras() const;
  std::vector<CameraId> isolatedCameras() const;

  // 内点不足的边直接剪掉，避免污染初始化
  void pruneBelow(int min_inliers);

  bool save(const std::string& path, std::string* error) const;
  bool load(const std::string& path, std::string* error);

 private:
  std::vector<ViewEdge> edges_;
  std::map<ViewPair, std::size_t> index_;
};

}  // namespace recon
