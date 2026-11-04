#pragma once
// 顶层流水线：把 M1-M8 串起来，main.cpp 只调它。
// 对应需求：FR-M1-03（分层）、FR-M4-06（端到端一条命令）
//
// 阶段划分与需求文档一致：
//   Feature -> Matching -> Sparse(SfM+BA) -> Dense(MVS) -> Evaluation
// 任一阶段失败时，前面的中间结果保留，便于 --resume。

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "core/config.hpp"
#include "core/types.hpp"
#include "eval/metrics.hpp"
#include "feature/feature_extractor.hpp"
#include "io/dataset.hpp"
#include "mvs/dense_reconstructor.hpp"
#include "mvs/point_cloud.hpp"
#include "sfm/reconstruction.hpp"
#include "sfm/view_graph.hpp"

namespace recon {

class FeatureCache;

class SfmPipeline {
 public:
  explicit SfmPipeline(AppConfig config);
  ~SfmPipeline();

  // 加载数据集、准备缓存目录
  bool initialize(std::string* error);

  // 全流程（不含可视化），成功后 reconstruction() / metrics() 可读
  bool run(const std::string& dataset_dir, std::string* error);

  bool runFeatureStage(std::string* error);
  bool runMatchingStage(std::string* error);
  bool runSparseStage(std::string* error);
  bool runDenseStage(std::string* error);
  bool runEvaluation(std::string* error);

  // 用 PCL/VTK 显示已有结果
  bool showResult() const;

  const AppConfig& config() const;
  const Reconstruction& reconstruction() const;
  const Metrics& metrics() const;

 private:
  void recordTiming(const std::string& stage, double seconds);

  AppConfig config_;
  Dataset dataset_;
  std::vector<FeatureSet> features_;
  ViewGraph graph_;
  std::map<ViewPair, std::vector<MatchRecord>> matches_;
  Reconstruction reconstruction_;
  Metrics metrics_;
  ColorCloud dense_cloud_;
  std::unique_ptr<FeatureCache> cache_;
};

}  // namespace recon
