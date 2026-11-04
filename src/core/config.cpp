#include "core/config.hpp"

namespace recon {

// 全部取结构体默认值；改默认参数请改 config.hpp 里的初始值，不要在这里另写一套
AppConfig ConfigLoader::defaults() {
  return AppConfig{};
}

bool ConfigLoader::load(const std::string& yaml_path, AppConfig* config, std::string* error) {
  (void)yaml_path;
  (void)config;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M1-04): 用 cv::FileStorage fs(yaml_path, cv::FileStorage::READ) 打开
  //   - 打开失败: *error = "无法打开配置文件: " + yaml_path; return false
  //   - 逐字段读取, 例如 fs["feature"]["max_features"] >> cfg.feature.max_features
  //   - 缺字段时保留默认值（不要报错，方便只覆盖想改的项）
  //   - 读完后做合法性检查: max_features > 0, ransac_threshold_px > 0, ...
  return false;
}

bool ConfigLoader::save(const std::string& yaml_path, const AppConfig& config) {
  (void)yaml_path;
  (void)config;
  // TODO(FR-M1-04): cv::FileStorage fs(yaml_path, cv::FileStorage::WRITE)
  //   把每个字段写回去，便于实验复现（run_meta.json 里也记录一份）
  return false;
}

}  // namespace recon
