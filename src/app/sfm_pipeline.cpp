#include "app/sfm_pipeline.hpp"

#include <chrono>
#include <filesystem>

#include "core/logger.hpp"
#include "feature/feature_cache.hpp"
#include "feature/feature_matcher.hpp"
#include "geometry/epipolar.hpp"
#include "mvs/dense_reconstructor.hpp"
#include "sfm/initializer.hpp"
#include "sfm/incremental_mapper.hpp"

namespace recon {

SfmPipeline::SfmPipeline(AppConfig config) : config_(std::move(config)) {}

SfmPipeline::~SfmPipeline() = default;

bool SfmPipeline::initialize(std::string* error) {
  if (error != nullptr) {
    error->clear();
  }
  (void)error;
  // TODO(FR-M1-04):
  //   1) 创建 config_.output_dir / config_.cache_dir（std::filesystem::create_directories）
  //   2) cache_ = std::make_unique<FeatureCache>(config_.cache_dir)
  //   3) DatasetReader::load + validate
  //   4) logInfo 打印数据集摘要：图像数、分辨率、是否 CUDA
  return false;
}

bool SfmPipeline::run(const std::string& dataset_dir, std::string* error) {
  (void)dataset_dir;
  if (error != nullptr) {
    error->clear();
  }
  // TODO(FR-M4-06) 顺序执行，任一阶段失败即中断并保留中间结果:
  //   if (!initialize(error)) return false;
  //   if (!runFeatureStage(error)) return false;
  //   if (!runMatchingStage(error)) return false;
  //   if (!runSparseStage(error)) return false;
  //   if (config_.enable_dense && !runDenseStage(error)) return false;
  //   return runEvaluation(error);
  //   每个阶段前后用 recordTiming 记耗时（NFR-PERF-01）
  return false;
}

bool SfmPipeline::runFeatureStage(std::string* error) {
  (void)error;
  // TODO(FR-M2-03 / FR-M2-04):
  //   FeatureExtractor extractor(config_.feature);
  //   features_.resize(dataset_.images.size());
  //   for i: 先 cache_->load，未命中则读图 + extract + cache_->save
  //   打印命中率，能直接体现缓存的收益（FR-M2-04 验收）
  return false;
}

bool SfmPipeline::runMatchingStage(std::string* error) {
  (void)error;
  // TODO(FR-M3-01 ~ FR-M3-04):
  //   1) 对每对候选（全连接或 k 近邻）做 FeatureMatcher::matchPair
  //   2) FundamentalEstimator::estimate 剔除误匹配，填 ViewEdge
  //   3) graph_.addEdge + pruneBelow(config_.match.min_inliers)
  //   4) 单机多线程：每对独立，用 std::thread 或 OpenMP 并行（NFR-PERF-01）
  //   5) 100 张图时全连接太慢，先用描述子做粗筛（如取 k 近邻）再精匹配
  return false;
}

bool SfmPipeline::runSparseStage(std::string* error) {
  (void)error;
  // TODO(FR-M4-01 ~ FR-M4-07):
  //   Initializer initializer(config_, dataset_.camera);
  //   InitialPair pair = initializer.selectPair(graph_, features_, matches_);
  //   initializer.initialize(pair, features_, matches_, &reconstruction_, error)
  //   IncrementalMapper mapper(config_, dataset_.camera, cache_.get());
  //   mapper.run(dataset_, graph_, matches_, features_, &reconstruction_, error)
  //   最后写 reconstruction.json（FR-M4-07），供 --resume 与 Python 调试
  return false;
}

bool SfmPipeline::runDenseStage(std::string* error) {
  (void)error;
  // TODO(FR-M6-04):
  //   DenseReconstructor dense(config_);
  //   dense.reconstruct(dataset_, reconstruction_, &dense_cloud_, error)
  //   导出 PLY 到 output_dir/sparse_vs_dense 对照（答辩素材）
  return false;
}

bool SfmPipeline::runEvaluation(std::string* error) {
  (void)error;
  // TODO(FR-M8-01 / FR-M8-02):
  //   metrics_ = Evaluator::compute(reconstruction_, dataset_.images.size());
  //   metrics_.num_dense_points = dense_cloud_.size();
  //   Evaluator::print(metrics_);
  //   Evaluator::writeJson / writeCsv 到 output_dir
  //   同时导出位姿（Exporter::exportSparseJson / exportTrajectoryTum）
  return false;
}

bool SfmPipeline::showResult() const {
  // TODO(FR-M7-01): 构造 PointCloudViewer，加载 dense_cloud_（没有则显示稀疏点云）
  //   + showCameras(reconstruction_) + showTrajectory(reconstruction_) + run()
  return false;
}

void SfmPipeline::recordTiming(const std::string& stage, double seconds) {
  (void)stage;
  (void)seconds;
  // TODO(FR-M8-01): metrics_.timings.push_back(StageTiming{stage, seconds})
}

const AppConfig& SfmPipeline::config() const {
  return config_;
}

const Reconstruction& SfmPipeline::reconstruction() const {
  return reconstruction_;
}

const Metrics& SfmPipeline::metrics() const {
  return metrics_;
}

}  // namespace recon
