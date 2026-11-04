#pragma once
// 对极几何：F 矩阵（自实现归一化 8 点法 + RANSAC）与 E 矩阵分解。
// 这是任务书最核心的算法考点，必须与 OpenCV 实现可对照 —— 见 FR-M3-02/03、FR-M4-02/03。
//
// 对照实验的用法：
//   FundamentalEstimator::estimate()      自实现
//   FundamentalEstimator::estimateOpenCv() 基线
//   relativeError() 输出两者差异，写进报告

#include <array>
#include <vector>

#include <opencv2/core.hpp>

#include "core/config.hpp"
#include "core/types.hpp"

namespace recon {

// 一次对极几何估计的结果
struct EpipolarResult {
  cv::Matx33d F = cv::Matx33d::zeros();
  std::vector<unsigned char> inlier_mask;
  int num_inliers = 0;
  double inlier_ratio = 0.0;
  bool success = false;
};

class FundamentalEstimator {
 public:
  explicit FundamentalEstimator(const MatchConfig& config);

  // 归一化变换 T：把点平移到质心、缩放到平均距离 sqrt(2)
  static bool normalizePoints(const std::vector<cv::Point2f>& points,
                              cv::Matx33d* transform,
                              std::vector<cv::Point2f>* normalized);

  // 归一化 8 点法（DLT + SVD），要求至少 8 对点
  static bool eightPoint(const std::vector<cv::Point2f>& points_a,
                         const std::vector<cv::Point2f>& points_b,
                         cv::Matx33d* F);

  // Sampson 距离，RANSAC 的内点判据
  static double sampsonDistance(const cv::Matx33d& F,
                                const cv::Point2f& point_a,
                                const cv::Point2f& point_b);

  // 强制 rank(F) = 2
  static cv::Matx33d enforceRankTwo(const cv::Matx33d& F);

  // 自实现 RANSAC 主循环
  EpipolarResult estimate(const std::vector<cv::Point2f>& points_a,
                          const std::vector<cv::Point2f>& points_b) const;

  // OpenCV 基线：cv::findFundamentalMat(..., FM_RANSAC)
  EpipolarResult estimateOpenCv(const std::vector<cv::Point2f>& points_a,
                                const std::vector<cv::Point2f>& points_b) const;

  // 用给定 F 统计内点率
  double inlierRatio(const cv::Matx33d& F,
                     const std::vector<cv::Point2f>& points_a,
                     const std::vector<cv::Point2f>& points_b) const;

  // 两个 F 的相对差异（用于"与 OpenCV 误差 < 5%"的验收）
  static double relativeError(const cv::Matx33d& lhs, const cv::Matx33d& rhs);

 private:
  std::vector<int> sampleEight(int count) const;

  MatchConfig config_;
};

// E 矩阵的四组分解候选
struct EssentialDecomposition {
  std::array<cv::Matx33d, 4> rotations;
  std::array<cv::Vec3d, 4> translations;
  int best_index = -1;
  int positive_depth_count = 0;
};

class EssentialEstimator {
 public:
  // E = K^T * F * K
  static cv::Matx33d fromFundamental(const cv::Matx33d& F, const cv::Mat& K);

  // SVD 约束奇异值后分解，得到 4 组 (R, t)
  static bool decompose(const cv::Matx33d& E, EssentialDecomposition* result);

  // 手性检验：选让最多点在两相机前方的解
  static bool selectByCheirality(const EssentialDecomposition& candidates,
                                 const std::vector<cv::Point2f>& points_a,
                                 const std::vector<cv::Point2f>& points_b,
                                 const cv::Mat& K,
                                 EssentialDecomposition* best);
};

}  // namespace recon
