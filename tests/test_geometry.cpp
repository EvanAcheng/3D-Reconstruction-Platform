# 几何核心的单元测试骨架。
# 这些函数的正确性决定整条链路的精度，必须用合成数据验证 —— NFR-TEST-01。
#
# 建议顺序：
#   1. 构造 8 个以上的三维点 + 已知相机位姿，投影得到两组二维点
#   2. 给二维点加高斯噪声，跑 8 点法，与真值 F 比相对误差
#   3. 跑 RANSAC，断言内点率 > 0
#   4. 三角化后与原始三维点比误差
#   5. PnP 后与真值 R/t 比误差

#include <cmath>
#include <iostream>

#include "geometry/epipolar.hpp"
#include "geometry/pose_estimator.hpp"
#include "geometry/triangulation.hpp"

int main() {
  // TODO(FR-M3-02): 合成数据 -> FundamentalEstimator::eightPoint -> 与真值比
  // TODO(FR-M4-03): 合成数据 -> EssentialEstimator::decompose + selectByCheirality
  // TODO(FR-M4-04): 合成数据 -> Triangulator::triangulate -> 相对误差 < 1%
  // TODO(FR-M4-05): 合成数据 -> PoseEstimator::solve -> 旋转误差 < 0.5°

  std::cout << "[test_geometry] 尚未实现，全部用例为 TODO\n";
  return 0;
}
