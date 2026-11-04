#pragma once
// SIFT 特征提取（CPU / CUDA 双路径）。
// 对应需求：FR-M2-03、FR-M2-05；加速指标 NFR-PERF-02

#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include "core/config.hpp"

namespace recon {

// 单张图像的特征集合
struct FeatureSet {
  std::vector<cv::KeyPoint> keypoints;
  cv::Mat descriptors;  // CV_32F, N x 128 (SIFT)
  int image_width = 0;
  int image_height = 0;

  bool empty() const;
};

class FeatureExtractor {
 public:
  explicit FeatureExtractor(const FeatureConfig& config);

  // 主入口：根据配置自动选择 CUDA 或 CPU，失败时回退 CPU
  bool extract(const cv::Mat& gray, FeatureSet* features) const;

  bool extractCuda(const cv::Mat& gray, FeatureSet* features) const;
  bool extractCpu(const cv::Mat& gray, FeatureSet* features) const;

  bool isCudaAvailable() const;

 private:
  FeatureConfig config_;
};

}  // namespace recon
