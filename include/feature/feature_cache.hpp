#pragma once
// 特征缓存：按图像内容哈希落盘，避免重复提取。
// 对应需求：FR-M2-04；收益见 NFR-PERF-01

#include <string>

#include "core/types.hpp"
#include "feature/feature_extractor.hpp"

namespace recon {

class FeatureCache {
 public:
  explicit FeatureCache(std::string cache_dir);

  bool load(const std::string& image_path, FeatureSet* features) const;
  bool save(const std::string& image_path, const FeatureSet& features) const;
  bool contains(const std::string& image_path) const;
  void clear() const;

  // 文件路径 + 修改时间 + 尺寸 的哈希，图像内容变了自动失效
  static std::string hashFile(const std::string& path);

 private:
  std::string pathFor(const std::string& image_path) const;

  std::string cache_dir_;
};

}  // namespace recon
