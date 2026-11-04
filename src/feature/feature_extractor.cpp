#include "feature/feature_extractor.hpp"

#include <opencv2/core/cuda.hpp>
#include <opencv2/cudafeatures2d.hpp>
#include <opencv2/features2d.hpp>

#include "core/logger.hpp"

namespace recon {

bool FeatureSet::empty() const {
  return keypoints.empty() || descriptors.empty();
}

FeatureExtractor::FeatureExtractor(const FeatureConfig& config) : config_(config) {}

bool FeatureExtractor::isCudaAvailable() const {
  // TODO(FR-M1-05): cv::cuda::getCudaEnabledDeviceCount() > 0 && config_.use_cuda
  return false;
}

bool FeatureExtractor::extract(const cv::Mat& gray, FeatureSet* features) const {
  (void)gray;
  (void)features;
  // TODO(FR-M2-03): 按 config_.use_cuda 分派；CUDA 路径抛异常或返回 false 时
  //   自动回退 extractCpu()，并 logWarn("CUDA 提取失败，已回退 CPU")
  return false;
}

bool FeatureExtractor::extractCpu(const cv::Mat& gray, FeatureSet* features) const {
  (void)gray;
  (void)features;
  // TODO(FR-M2-03):
  //   cv::Ptr<cv::SIFT> sift = cv::SIFT::create(config_.max_features,
  //                                            3,                    // nOctaveLayers
  //                                            config_.contrast_threshold,
  //                                            config_.edge_threshold);
  //   sift->detectAndCompute(gray, cv::noArray(), features->keypoints, features->descriptors);
  //   填 features->image_width / image_height；keypoints 少于 100 时 logWarn
  return false;
}

bool FeatureExtractor::extractCuda(const cv::Mat& gray, FeatureSet* features) const {
  (void)gray;
  (void)features;
  // TODO(FR-M2-03 / NFR-PERF-02):
  //   cv::cuda::GpuMat gpu_gray, gpu_desc;
  //   gpu_gray.upload(gray);
  //   cv::Ptr<cv::cuda::SIFT_CUDA> sift_cuda = cv::cuda::SIFT_CUDA::create();
  //   sift_cuda->detectAndCompute(gpu_gray, cv::cuda::GpuMat(), keypoints, gpu_desc);
  //   gpu_desc.download(features->descriptors);
  //   注意: CUDA 版描述子类型与 CPU 版可能不同(CV_32F)，匹配前要统一
  //   计时并 logInfo，用于统计 CUDA 加速比（报告需要实测数据）
  return false;
}

}  // namespace recon
