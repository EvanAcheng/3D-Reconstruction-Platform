#include "feature/feature_cache.hpp"

#include <filesystem>

#include <opencv2/core.hpp>

namespace recon {

FeatureCache::FeatureCache(std::string cache_dir) : cache_dir_(std::move(cache_dir)) {}

std::string FeatureCache::hashFile(const std::string& path) {
  (void)path;
  // TODO(FR-M2-04): 用 std::filesystem::last_write_time + file_size 拼成字符串再哈希
  //   （不读全文件内容，避免大图 IO 拖慢；想更严谨可以加前 1KB 内容的哈希）
  return {};
}

std::string FeatureCache::pathFor(const std::string& image_path) const {
  (void)image_path;
  // TODO(FR-M2-04): cache_dir_ + "/" + hashFile(image_path) + ".feature.yml"
  return {};
}

bool FeatureCache::contains(const std::string& image_path) const {
  (void)image_path;
  // TODO(FR-M2-04): std::filesystem::exists(pathFor(image_path))
  return false;
}

bool FeatureCache::load(const std::string& image_path, FeatureSet* features) const {
  (void)image_path;
  (void)features;
  // TODO(FR-M2-04): cv::FileStorage 读 keypoints（写成 Nx2 的 CV_32F + size + angle 等）
  //   或直接序列化 cv::write(FileStorage&, "features", keypoints)
  //   文件损坏时返回 false 并删除坏缓存，让上层重新提取
  return false;
}

bool FeatureCache::save(const std::string& image_path, const FeatureSet& features) const {
  (void)image_path;
  (void)features;
  // TODO(FR-M2-04): 创建父目录 -> cv::FileStorage 写入 -> 先写临时文件再 rename，
  //   避免进程中断留下半个文件
  return false;
}

void FeatureCache::clear() const {
  // TODO(FR-M2-04): std::filesystem::remove_all(cache_dir_)，并 logInfo 打印清掉了多少文件
}

}  // namespace recon
