#include "io/dataset.hpp"

#include <algorithm>
#include <filesystem>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace recon {

bool ImageLoader::naturalCompare(const std::string& lhs, const std::string& rhs) {
  // TODO(FR-M2-01): 逐字符比较，遇到数字段时按数值比较（img_2 < img_10）
  //   不要直接用 std::string::operator<，否则 img_10 会排在 img_2 前面
  return lhs < rhs;
}

std::vector<std::string> ImageLoader::listImageFiles(const std::string& directory) {
  (void)directory;
  // TODO(FR-M2-01): std::filesystem::directory_iterator 过滤 .jpg/.jpeg/.png/.bmp
  //   收集后按 naturalCompare 排序，返回完整路径
  return {};
}

cv::Mat ImageLoader::loadGray(const std::string& path, int max_edge, int* width, int* height) {
  (void)path;
  (void)max_edge;
  (void)width;
  (void)height;
  // TODO(FR-M2-01): cv::imread(path, cv::IMREAD_GRAYSCALE)
  //   - 空图: logError 并返回空 Mat（调用方负责判断）
  //   - cv::resize(..., INTER_AREA) 保持长边 = max_edge
  //   - 通过 width/height 回传实际尺寸，后面的内参要用它
  return cv::Mat();
}

cv::Mat ImageLoader::loadColor(const std::string& path, int max_edge, int* width, int* height) {
  (void)path;
  (void)max_edge;
  (void)width;
  (void)height;
  // TODO(FR-M6-04): cv::imread(path, cv::IMREAD_COLOR)，同样的缩放策略
  //   注意彩色图与灰度图必须用同一个 max_edge，否则深度图无法和颜色对齐
  return cv::Mat();
}

cv::Mat ImageLoader::resizeKeepingRatio(const cv::Mat& image, int max_edge) {
  (void)image;
  (void)max_edge;
  // TODO(FR-M2-01): 长边 <= max_edge 时原样返回；否则按比例 cv::resize
  return cv::Mat();
}

bool DatasetReader::load(const AppConfig& config, Dataset* dataset, std::string* error) {
  (void)config;
  (void)dataset;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M2-01): 1) listImageFiles(config.dataset_dir)
  //   2) config.dataset_dir + "/camera.yaml" -> CameraModel::fromCalibrationFile
  //      找不到标定文件时尝试 fromExif 并 WARN
  //   3) 逐张读尺寸填入 ImageRecord::width/height（可只读前 1 张以加速）
  //   4) 图像少于 2 张 -> *error = "图像数量不足"; return false
  return false;
}

bool DatasetReader::validate(const Dataset& dataset, std::string* error) {
  (void)dataset;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M2-02): 内参分辨率与图像分辨率不一致时报错（这是"静默出错"的高发点）
  //   允许 1px 以内的误差；不一致时提示"内参对应 WxH，实际图像 WxH"
  return false;
}

}  // namespace recon
