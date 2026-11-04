#include "geometry/epipolar.hpp"

#include <algorithm>
#include <cmath>
#include <random>

#include <opencv2/calib3d.hpp>

#include "core/logger.hpp"
#include "geometry/triangulation.hpp"

namespace recon {

FundamentalEstimator::FundamentalEstimator(const MatchConfig& config) : config_(config) {}

bool FundamentalEstimator::normalizePoints(const std::vector<cv::Point2f>& points,
                                           cv::Matx33d* transform,
                                           std::vector<cv::Point2f>* normalized) {
  (void)points;
  (void)transform;
  (void)normalized;
  // TODO(FR-M3-02): 1) 求质心 mean
  //   2) 求到质心的平均距离 d，缩放因子 s = sqrt(2) / d
  //   3) T = [s 0 -s*mean.x; 0 s -s*mean.y; 0 0 1]
  //   Hartley 归一化是 8 点法数值稳定的关键，不做会导致 F 完全错乱
  return false;
}

bool FundamentalEstimator::eightPoint(const std::vector<cv::Point2f>& points_a,
                                      const std::vector<cv::Point2f>& points_b,
                                      cv::Matx33d* F) {
  (void)points_a;
  (void)points_b;
  (void)F;
  // TODO(FR-M3-02):
  //   1) normalizePoints 得到 Ta、Tb 和归一化点
  //   2) 每个匹配拼一行 A: [x2*x1, x2*y1, x2, y2*x1, y2*y1, y2, x1, y1, 1]
  //   3) cv::SVD::compute(A, w, u, vt)，取 vt 最后一行 reshape(3,3) 作为 F~
  //   4) 反归一化 F = Tb^T * F~ * Ta
  //   5) enforceRankTwo(F) 并归一化（除以 F(2,2) 或 F 的范数）
  //   点数不足 8 时返回 false
  return false;
}

double FundamentalEstimator::sampsonDistance(const cv::Matx33d& F,
                                             const cv::Point2f& point_a,
                                             const cv::Point2f& point_b) {
  (void)F;
  (void)point_a;
  (void)point_b;
  // TODO(FR-M3-03): 构造 x1=(u1,v1,1)、x2=(u2,v2,1)
  //   Fx1 = F * x1; Ftx2 = F^T * x2
  //   e = x2^T * F * x1
  //   d = e^2 / (Fx1(0)^2 + Fx1(1)^2 + Ftx2(0)^2 + Ftx2(1)^2)
  //   返回 sqrt(d)
  return 0.0;
}

cv::Matx33d FundamentalEstimator::enforceRankTwo(const cv::Matx33d& F) {
  (void)F;
  // TODO(FR-M3-02): cv::SVD::compute(F, w, u, vt)
  //   把最小奇异值置 0：F = u * diag(w0, w1, 0) * vt
  return cv::Matx33d::zeros();
}

std::vector<int> FundamentalEstimator::sampleEight(int count) const {
  (void)count;
  // TODO(FR-M3-03): 用 std::mt19937（种子来自 config.seed，保证可复现）做无重复采样
  //   每次迭代随机取 8 个不同下标
  return {};
}

EpipolarResult FundamentalEstimator::estimate(const std::vector<cv::Point2f>& points_a,
                                              const std::vector<cv::Point2f>& points_b) const {
  (void)points_a;
  (void)points_b;
  // TODO(FR-M3-03) RANSAC 主循环:
  //   for (iteration < config_.ransac_max_iterations):
  //     1) sampleEight 取 8 对 -> eightPoint 得到候选 F
  //     2) 对全部点算 sampsonDistance < ransac_threshold_px 计为内点
  //     3) 内点数创新高时保存 F 与 mask，并按
  //        N = log(1-p) / log(1 - w^8) 更新自适应迭代上限（w = 内点率）
  //     4) 达到 confidence 提前退出
  //   最后用全部内点再做一次 eightPoint 精化（refit）
  //   返回值里带 inlier_mask / num_inliers / inlier_ratio / success
  return EpipolarResult{};
}

EpipolarResult FundamentalEstimator::estimateOpenCv(const std::vector<cv::Point2f>& points_a,
                                                    const std::vector<cv::Point2f>& points_b) const {
  (void)points_a;
  (void)points_b;
  // TODO(FR-M3-02 对照实验): cv::findFundamentalMat(points_a, points_b,
  //   cv::FM_RANSAC, config_.ransac_threshold_px, config_.ransac_confidence, mask)
  //   结果与自实现做 relativeError 对比，写进报告的"优化与验证"章节
  return EpipolarResult{};
}

double FundamentalEstimator::inlierRatio(const cv::Matx33d& F,
                                         const std::vector<cv::Point2f>& points_a,
                                         const std::vector<cv::Point2f>& points_b) const {
  (void)F;
  (void)points_a;
  (void)points_b;
  // TODO(FR-M3-03): 统计 sampsonDistance < ransac_threshold_px 的比例
  return 0.0;
}

double FundamentalEstimator::relativeError(const cv::Matx33d& lhs, const cv::Matx33d& rhs) {
  (void)lhs;
  (void)rhs;
  // TODO(FR-M3-02): 先各自按 Frobenius 范数归一化（F 有尺度自由度，直接减没有意义），
  //   再算 ||a - b|| / ||a||；注意符号翻转：取 min(||a-b||, ||a+b||)
  return 0.0;
}

cv::Matx33d EssentialEstimator::fromFundamental(const cv::Matx33d& F, const cv::Mat& K) {
  (void)F;
  (void)K;
  // TODO(FR-M4-02): E = K^T * F * K（K 为 3x3 CV_64F），再按奇异值 (s,s,0) 归一化
  return cv::Matx33d::zeros();
}

bool EssentialEstimator::decompose(const cv::Matx33d& E, EssentialDecomposition* result) {
  (void)E;
  (void)result;
  // TODO(FR-M4-02):
  //   1) cv::SVD::compute(E, w, u, vt)；若 det(u) < 0 则 u = -u，同理 vt
  //   2) 构造 W = [0 -1 0; 1 0 0; 0 0 1]
  //   3) R1 = u*W*vt, R2 = u*W^T*vt, t = u 的第 3 列（±）
  //   4) 4 组解写入 rotations[0..3] / translations[0..3]
  return false;
}

bool EssentialEstimator::selectByCheirality(const EssentialDecomposition& candidates,
                                            const std::vector<cv::Point2f>& points_a,
                                            const std::vector<cv::Point2f>& points_b,
                                            const cv::Mat& K,
                                            EssentialDecomposition* best) {
  (void)candidates;
  (void)points_a;
  (void)points_b;
  (void)K;
  (void)best;
  // TODO(FR-M4-03):
  //   P1 = K * [I | 0]，P2 = K * [R | t]
  //   对每组解用 Triangulator::triangulate 批量三角化，统计
  //   "两个相机前方"的点数（Triangulator::isInFront）
  //   取点数最多的一组；若最多也只有 0 个，说明初始对退化，返回 false
  return false;
}

}  // namespace recon
