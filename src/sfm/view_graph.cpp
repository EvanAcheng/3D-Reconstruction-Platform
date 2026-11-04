#include "sfm/view_graph.hpp"

#include <algorithm>

#include <opencv2/core.hpp>

namespace recon {

void ViewGraph::addEdge(const ViewEdge& edge) {
  (void)edge;
  // TODO(FR-M3-04): 同一对图像只保留内点数更高的一条；
  //   写入 edges_ 并更新 index_[ViewPair]（注意统一 lhs < rhs 的顺序）
}

const std::vector<ViewEdge>& ViewGraph::edges() const {
  return edges_;
}

std::size_t ViewGraph::numEdges() const {
  return edges_.size();
}

bool ViewGraph::hasEdge(CameraId a, CameraId b) const {
  (void)a;
  (void)b;
  // TODO(FR-M3-04): index_.count(ViewPair{min, max}) > 0
  return false;
}

const ViewEdge* ViewGraph::findEdge(CameraId a, CameraId b) const {
  (void)a;
  (void)b;
  // TODO(FR-M3-04): 查 index_，找不到返回 nullptr
  return nullptr;
}

std::vector<ViewEdge> ViewGraph::neighbors(CameraId camera_id, int k) const {
  (void)camera_id;
  (void)k;
  // TODO(FR-M3-04): 收集所有包含 camera_id 的边，按 num_inliers 降序取前 k 个
  return {};
}

std::vector<CameraId> ViewGraph::cameras() const {
  // TODO(FR-M3-04): 从 edges_ 收集去重后的相机 id 并排序
  return {};
}

std::vector<CameraId> ViewGraph::isolatedCameras() const {
  // TODO(FR-M3-04): 度数 < 1 的相机 —— 这些图像无法注册，要单独提示用户
  return {};
}

void ViewGraph::pruneBelow(int min_inliers) {
  (void)min_inliers;
  // TODO(FR-M3-04): 删掉 num_inliers < min_inliers 的边，并重建 index_
  //   剪枝后要重新检查连通分量，断开的子图需要提示"数据集可能不连续"
}

bool ViewGraph::save(const std::string& path, std::string* error) const {
  (void)path;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M3-04): JSON 输出 edges（含 F 矩阵），Python 端读它画视图图
  return false;
}

bool ViewGraph::load(const std::string& path, std::string* error) {
  (void)path;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M3-04): 读回后重建 index_
  return false;
}

}  // namespace recon
