#pragma once
// 配置项与配置加载器。所有阈值、路径、开关都从这里来，禁止散落在代码里写死。
// 对应需求：FR-M1-04（运行期配置）、NFR-MAINT-04（配置外置）

#include <string>

#include "core/types.hpp"

namespace recon {

// 特征提取参数（M2）
struct FeatureConfig {
  int max_features = 8000;
  double contrast_threshold = 0.04;
  double edge_threshold = 10.0;
  double ratio_threshold = 0.75;   // Lowe ratio test
  int resize_max_edge = 1600;      // 长边上限，控制内存
  bool use_cuda = true;            // 无 GPU 时自动回退 CPU
};

// 匹配与几何验证参数（M3）
struct MatchConfig {
  int knn = 2;
  double ransac_threshold_px = 1.5;   // Sampson 距离阈值
  int ransac_max_iterations = 1000;
  double ransac_confidence = 0.999;
  int min_inliers = 30;               // 少于该内点数的匹配对丢弃
  double min_inlier_ratio = 0.30;
  int view_graph_k = 10;              // 每张图最多保留的邻居数
  double homography_dominance_ratio = 0.8;  // H/F 内点比超过它判为退化对
};

// 稀疏重建与 BA 参数（M4/M5）
struct SfmConfig {
  int min_observations_per_point = 2;
  double max_reprojection_error_px = 2.0;
  double min_triangulation_angle_deg = 1.0;
  int local_ba_window = 5;            // 局部 BA 的滑窗帧数
  int max_ba_iterations = 50;
  bool fix_intrinsics = true;
  std::string robust_loss = "HUBER";  // HUBER / CAUCHY / NONE
  double robust_loss_scale = 1.0;
  int max_rounds = 200;               // 增量注册主循环上限
};

// 稠密重建参数（M6）
struct MvsConfig {
  int num_disparities = 128;
  int block_size = 7;
  int p1 = 8 * 3 * 7 * 7;    // SGBM 平滑项
  int p2 = 32 * 3 * 7 * 7;
  int neighbor_views = 3;
  double lr_consistency_threshold = 1.0;
  double voxel_leaf_size = 0.01;   // 单位：场景尺度
  int sor_mean_k = 20;
  double sor_std_ratio = 1.0;
  bool use_cuda = true;
};

// 可视化参数（M7）
struct VizConfig {
  std::string window_title = "Multi-View 3D Reconstruction";
  int screen_width = 1280;
  int screen_height = 800;
  double point_size = 1.5;
  bool show_cameras = true;
  bool show_trajectory = true;
  bool show_matches = false;
};

// 全局配置，由 YAML 加载
struct AppConfig {
  std::string dataset_dir;
  std::string output_dir = "results";
  std::string cache_dir = "cache";
  unsigned int seed = 42;
  int num_threads = 8;
  bool enable_dense = true;
  FeatureConfig feature;
  MatchConfig match;
  SfmConfig sfm;
  MvsConfig mvs;
  VizConfig viz;
};

// 配置读写
class ConfigLoader {
 public:
  static AppConfig defaults();
  static bool load(const std::string& yaml_path, AppConfig* config, std::string* error);
  static bool save(const std::string& yaml_path, const AppConfig& config);
};

}  // namespace recon
