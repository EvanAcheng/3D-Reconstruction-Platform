#pragma once
// 数据集读取：图像列表、读图与降采样、内参绑定、输入校验。
// 对应需求：FR-M2-01（序列读取）、FR-M2-02（内参校验）、FR-M2-05（质量统计）

#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "core/config.hpp"
#include "io/camera_model.hpp"

namespace recon {

struct ImageRecord {
  std::string path;
  std::string name;
  int width = 0;
  int height = 0;
};

// 一个可重建的数据集：根目录 + 有序图像 + 相机模型
struct Dataset {
  std::string root;
  std::vector<ImageRecord> images;
  CameraModel camera;
};

class ImageLoader {
 public:
  // 自然排序：img_2 排在 img_10 之前
  static bool naturalCompare(const std::string& lhs, const std::string& rhs);

  // 列出目录下所有 jpg/png，并按自然序排好
  static std::vector<std::string> listImageFiles(const std::string& directory);

  static cv::Mat loadGray(const std::string& path, int max_edge, int* width, int* height);
  static cv::Mat loadColor(const std::string& path, int max_edge, int* width, int* height);

 private:
  static cv::Mat resizeKeepingRatio(const cv::Mat& image, int max_edge);
};

class DatasetReader {
 public:
  // 读取 config.dataset_dir 下的图像与内参，失败时写 error 并返回 false
  static bool load(const AppConfig& config, Dataset* dataset, std::string* error);

  // 内参与图像分辨率一致性校验
  static bool validate(const Dataset& dataset, std::string* error);
};

}  // namespace recon
