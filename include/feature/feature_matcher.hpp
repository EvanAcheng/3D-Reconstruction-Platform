#pragma once
// 特征匹配：KNN + Lowe ratio test，可选 GPU 描述子匹配。
// 对应需求：FR-M3-01（KNN 匹配）、FR-M3-05（匹配可视化）

#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include "core/config.hpp"
#include "core/types.hpp"
#include "feature/feature_extractor.hpp"

namespace recon {

class FeatureMatcher {
 public:
  explicit FeatureMatcher(const MatchConfig& config);

  // 返回经过 ratio test 筛选后的匹配对
  std::vector<MatchRecord> matchPair(const FeatureSet& query, const FeatureSet& train) const;
  std::vector<MatchRecord> matchPairCuda(const FeatureSet& query, const FeatureSet& train) const;

  // k=2 的 KNN 结果 -> ratio test
  std::vector<MatchRecord> applyRatioTest(
      const std::vector<std::vector<cv::DMatch>>& knn_matches) const;

  // 并排连线图：内点绿色、外点红色，用于报告插图
  bool drawMatches(const cv::Mat& image_a, const cv::Mat& image_b,
                   const FeatureSet& features_a, const FeatureSet& features_b,
                   const std::vector<MatchRecord>& matches,
                   const std::vector<unsigned char>& inlier_mask,
                   const std::string& output_path) const;

 private:
  MatchConfig config_;
};

}  // namespace recon
