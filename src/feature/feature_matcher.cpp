#include "feature/feature_matcher.hpp"

#include <opencv2/calib3d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

namespace recon {

FeatureMatcher::FeatureMatcher(const MatchConfig& config) : config_(config) {}

std::vector<MatchRecord> FeatureMatcher::applyRatioTest(
    const std::vector<std::vector<cv::DMatch>>& knn_matches) const {
  (void)knn_matches;
  // TODO(FR-M3-01): 对每个 query 取最近的两个候选，若 best.distance <
  //   ratio_threshold * second.distance 则保留，写入 MatchRecord{queryIdx, trainIdx, distance}
  return {};
}

std::vector<MatchRecord> FeatureMatcher::matchPair(const FeatureSet& query,
                                                   const FeatureSet& train) const {
  (void)query;
  (void)train;
  // TODO(FR-M3-01):
  //   cv::BFMatcher matcher(cv::NORM_L2, false);   // SIFT 用 L2；描述子若是 CV_8U 且为二进制则用 NORM_HAMMING
  //   std::vector<std::vector<cv::DMatch>> knn;
  //   matcher.knnMatch(query.descriptors, train.descriptors, knn, config_.knn);
  //   return applyRatioTest(knn);
  return {};
}

std::vector<MatchRecord> FeatureMatcher::matchPairCuda(const FeatureSet& query,
                                                       const FeatureSet& train) const {
  (void)query;
  (void)train;
  // TODO(FR-M3-01 / NFR-PERF-02):
  //   cv::cuda::GpuMat dq(query.descriptors), dt(train.descriptors);
  //   cv::Ptr<cv::cuda::DescriptorMatcher> matcher = cv::cuda::DescriptorMatcher::createBFMatcher(cv::NORM_L2);
  //   matcher->knnMatch(dq, dt, knn, config_.knn);
  //   注意: knnMatch 的索引方向是 (train, query)，写回 MatchRecord 时别把行列弄反
  return {};
}

bool FeatureMatcher::drawMatches(const cv::Mat& image_a,
                                 const cv::Mat& image_b,
                                 const FeatureSet& features_a,
                                 const FeatureSet& features_b,
                                 const std::vector<MatchRecord>& matches,
                                 const std::vector<unsigned char>& inlier_mask,
                                 const std::string& output_path) const {
  (void)image_a;
  (void)image_b;
  (void)features_a;
  (void)features_b;
  (void)matches;
  (void)inlier_mask;
  (void)output_path;
  // TODO(FR-M3-05): cv::drawMatches(matches) 不支持分组着色，做法是画两次：
  //   先只画内点（绿色），再用同图叠加外点（红色）
  //   cv::imwrite(output_path, canvas)，供报告插图使用
  return false;
}

}  // namespace recon
