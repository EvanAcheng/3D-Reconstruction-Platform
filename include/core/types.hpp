#pragma once
// 基础类型与数据结构 —— 全工程最底层，只依赖 OpenCV core，不依赖任何其他模块。
// 对应需求：FR-M1-03（工程分层）、NFR-MAINT-01（无环依赖）

#include <cstdint>
#include <utility>
#include <vector>

#include <opencv2/core.hpp>

namespace recon {

using CameraId = std::uint32_t;   // 相机（图像）编号
using Point3DId = std::uint64_t;  // 三维点编号
using ViewPair = std::pair<CameraId, CameraId>;

// 相机内参。本期假设全数据集共用一个相机模型，后续支持多相机时再扩展。
struct Intrinsics {
  double fx = 0.0;
  double fy = 0.0;
  double cx = 0.0;
  double cy = 0.0;
  double k1 = 0.0;
  double k2 = 0.0;
  double p1 = 0.0;
  double p2 = 0.0;
  int width = 0;
  int height = 0;
};

// 刚体位姿，约定 X_cam = R * X_world + t
struct Pose {
  cv::Matx33d R = cv::Matx33d::eye();
  cv::Vec3d t{0.0, 0.0, 0.0};
};

// 二维观测：某个三维点在某个相机中的成像位置
struct Observation {
  Point3DId point_id = 0;
  CameraId camera_id = 0;
  cv::Point2f xy;
};

// 一对特征匹配记录，索引指向各自 FeatureSet::keypoints
struct MatchRecord {
  int query_index = -1;
  int train_index = -1;
  float distance = 0.0f;
};

// 流水线各阶段状态，供日志与界面显示
enum class StageStatus { NotRun = 0, Running, Success, Failed, Skipped };

}  // namespace recon
