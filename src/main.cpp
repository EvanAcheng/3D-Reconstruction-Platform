// 程序入口：解析命令行 -> 加载配置 -> 跑流水线 -> 显示/导出结果。
// 这里只做参数与流程编排，不放任何算法。
//
// 用法示例：
//   three_dimension.exe --dataset data/samples/scene01 --config configs/default.yaml
//   three_dimension.exe --dataset ... --no-dense --no-cuda --show

#include <exception>
#include <iostream>
#include <string>

#include <opencv2/core.hpp>

#include "app/sfm_pipeline.hpp"
#include "core/config.hpp"
#include "core/logger.hpp"

namespace {

struct CommandLineArgs {
  std::string dataset_dir;
  std::string config_path = "configs/default.yaml";
  std::string output_dir;
  bool enable_dense = true;
  bool enable_cuda = true;
  bool show = false;
  bool verbose = false;
  bool help = false;
};

void printUsage() {
  std::cout << "用法: three_dimension --dataset <图像目录> [选项]\n"
               "  --config <path>   配置文件，默认 configs/default.yaml\n"
               "  --output <dir>    输出目录，默认 results\n"
               "  --no-dense        跳过稠密重建\n"
               "  --no-cuda         强制 CPU 路径\n"
               "  --show            重建后打开三维查看器\n"
               "  -v, --verbose     打开 DEBUG 日志\n"
               "  -h, --help        显示帮助\n";
}

CommandLineArgs parseArgs(int argc, char** argv) {
  CommandLineArgs args;
  (void)argc;
  (void)argv;
  // TODO(FR-M1-04): 逐个解析 argv
  //   - 未知参数: 打印提示并置 help = true（不要静默忽略，答辩时最容易踩这个）
  //   - 路径统一用 std::filesystem::path 规范化
  return args;
}

}  // namespace

int main(int argc, char** argv) {
  const CommandLineArgs args = parseArgs(argc, argv);
  if (args.help) {
    printUsage();
    return 0;
  }

  recon::Logger::instance().setLevel(args.verbose ? recon::LogLevel::Debug : recon::LogLevel::Info);
  recon::logInfo("main", "多视觉融合三维重建平台启动");

  try {
    recon::AppConfig config = recon::ConfigLoader::defaults();
    std::string error;

    // TODO(FR-M1-04): ConfigLoader::load(args.config_path, &config, &error)
    //   失败时打印 error 并 return 2（配置错误与运行失败要区分退出码）
    config.enable_dense = args.enable_dense;
    config.feature.use_cuda = args.enable_cuda;

    recon::SfmPipeline pipeline(config);
    if (!pipeline.run(args.dataset_dir, &error)) {
      recon::logError("main", "重建失败: " + error);
      return 1;
    }

    if (args.show) {
      // TODO(FR-M7-01): pipeline.showResult()
    }
  } catch (const cv::Exception& e) {
    // OpenCV 异常必须单独接住，否则只会打印一串 sts 错误码
    recon::logError("main", std::string("OpenCV 异常: ") + e.what());
    return 3;
  } catch (const std::exception& e) {
    recon::logError("main", std::string("未处理异常: ") + e.what());
    return 3;
  }

  recon::logInfo("main", "完成");
  return 0;
}
