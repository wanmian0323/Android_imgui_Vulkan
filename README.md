# Android ImGui Vulkan

一个面向 Android 原生环境的 Dear ImGui + Vulkan 绘制示例。项目直接通过 Android NDK 构建 `arm64-v8a` 原生可执行文件，创建独立的 `ANativeWindow`，使用 Vulkan 渲染 ImGui，并把触摸事件转换为 ImGui 输入。

> [!IMPORTANT]
> 这是 NDK 原生可执行项目，不是 Gradle/Android Studio APK 工程。当前构建脚本会生成 `libs/arm64-v8a/Android_imgui_Vulkan.rc`，运行时通常需要 root 权限才能访问 SurfaceFlinger 相关接口、`/dev/input` 和 `/dev/uinput`。

![Android](https://img.shields.io/badge/Android-9%2B-3DDC84?logo=android&logoColor=white)
![Vulkan](https://img.shields.io/badge/Renderer-Vulkan-AC162C?logo=vulkan&logoColor=white)
![C%2B%2B](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)
![ABI](https://img.shields.io/badge/ABI-arm64--v8a-444444)

## 功能概览

- 使用 `VulkanGraphics` 初始化 Vulkan instance、physical device、logical device、swapchain、render pass 和 framebuffer。
- 通过 `imgui_impl_vulkan` 将 Dear ImGui 绘制命令提交到 Vulkan command buffer。
- 通过 `ANativeWindowCreator` 动态解析 `libgui.so`/`libutils.so` 的符号，并创建独立 Surface。
- 按 Android 系统版本选择 SurfaceComposer 兼容符号，覆盖 Android 9+，并针对 Android 9、10、11、12、14、15、16、17 提供符号适配表。
- 监听 `/dev/input/event*`，解析多点触摸事件，并通过 `/dev/uinput` 创建虚拟触摸设备。
- 处理屏幕旋转、窗口尺寸变化和 swapchain 重建。
- 内置中文黑体、Font Awesome 图标字体和 PNG 图片资源。
- 支持从文件、内存和 GIF 内存数据加载纹理。
- 示例 UI 包含主题切换、穿透开关、绘制对角线、第二窗口、图片、按钮、颜色编辑器和 FPS 显示。

## 工作原理

~~~mermaid
flowchart LR
    entry[main.cpp] --> display[DisplayInfo]
    entry --> surface[ANativeWindowCreator]
    surface --> composer[SurfaceComposer / libgui]
    entry --> manager[GraphicsManager]
    manager --> vk[VulkanGraphics]
    vk --> vulkan[Android Vulkan driver]
    vk --> imgui[Dear ImGui Vulkan backend]
    touch[TouchHelperA] --> input[/dev/input + /dev/uinput]
    touch --> io[ImGuiIO]
    ui[draw_Gui.cpp] --> imgui
    io --> imgui
    imgui --> surface
~~~

启动后的主循环位于 `jni/src/main.cpp`：

1. 读取屏幕尺寸和旋转方向。
2. 创建 `ANativeWindow` 并初始化 Vulkan 渲染器。
3. 创建 ImGui context、Android 输入后端和字体/纹理资源。
4. 每帧执行 `NewFrame()`、`Layout_tick_UI()`、`EndFrame()`。
5. 退出时销毁纹理、ImGui context、Vulkan 资源和 Surface。

## 环境要求

| 组件 | 要求 |
| --- | --- |
| Android NDK | 已用 `27.0.12077973`（NDK r27）验证编译 |
| 编译器 | NDK Clang，C++17 |
| ABI | `arm64-v8a` |
| Android API | `APP_PLATFORM := android-22`；SurfaceComposer 运行时适配 Android 9+ |
| GPU | 设备需要可用 Vulkan 驱动 |
| 运行权限 | 当前 Surface 和触摸实现通常需要 root/shell 特权 |
| 工具 | `ndk-build`、`adb`、PowerShell 或 Bash |

## 快速开始

### Windows PowerShell

在项目根目录执行：

~~~powershell
$env:ANDROID_NDK_HOME = "D:\Android\Sdk\ndk\27.0.12077973"
& "$env:ANDROID_NDK_HOME\ndk-build.cmd" -j4
~~~

构建成功后，产物位于：

~~~text
libs/arm64-v8a/Android_imgui_Vulkan.rc
~~~

推送到已连接的 Android 设备：

~~~powershell
adb devices
adb push .\libs\arm64-v8a\Android_imgui_Vulkan.rc /data/local/tmp/
adb shell chmod 755 /data/local/tmp/Android_imgui_Vulkan.rc
adb shell su -c '/data/local/tmp/Android_imgui_Vulkan.rc'
~~~

另开终端观察日志：

~~~powershell
adb logcat -s ImGui Vulkan AndroidRuntime
~~~

### Linux/macOS

~~~bash
export ANDROID_NDK_HOME="$ANDROID_HOME/ndk/27.0.12077973"
"$ANDROID_NDK_HOME/ndk-build" -j4
adb push libs/arm64-v8a/Android_imgui_Vulkan.rc /data/local/tmp/
adb shell chmod 755 /data/local/tmp/Android_imgui_Vulkan.rc
adb shell su -c '/data/local/tmp/Android_imgui_Vulkan.rc'
~~~

## 构建配置

构建入口是 `jni/Android.mk`，全局 NDK 配置位于 `jni/Application.mk`。

| 配置 | 当前值 | 作用 |
| --- | --- | --- |
| `LOCAL_MODULE` | `Android_imgui_Vulkan.rc` | 输出可执行文件名 |
| `APP_ABI` | `arm64-v8a` | 目标 CPU 架构 |
| `APP_PLATFORM` | `android-22` | NDK 编译 API level |
| `APP_STL` | `c++_static` | 静态链接 C++ STL |
| `APP_OPTIM` | `release` | 默认 Release 优化 |
| `LOCAL_CPPFLAGS` | `-std=c++17` | 使用 C++17 |
| `VK_USE_PLATFORM_ANDROID_KHR` | 已启用 | 启用 Android Vulkan surface 扩展 |
| `IMGUI_IMPL_VULKAN_NO_PROTOTYPES` | 已启用 | 运行时加载 Vulkan 函数 |
| `IMGUI_DISABLE_DEBUG_TOOLS` | 已启用 | 关闭 ImGui Debug Tools |

### Debug 构建

~~~powershell
& "$env:ANDROID_NDK_HOME\ndk-build.cmd" -j4 NDK_DEBUG=1 V=1
~~~

### 清理构建目录

~~~powershell
& "$env:ANDROID_NDK_HOME\ndk-build.cmd" clean
~~~

## 目录结构

~~~text
.
├── README.md
└── jni
    ├── Android.mk                    # NDK 模块、编译选项和源文件清单
    ├── Application.mk                # ABI、API level 和 STL 配置
    ├── include
    │   ├── Android_draw              # 绘制入口和 UI 声明
    │   ├── Android_Graphics          # Vulkan 图形抽象和函数加载声明
    │   ├── Android_my_imgui          # Android ImGui 生命周期封装
    │   ├── Android_touch             # 触摸、旋转和输入结构
    │   ├── ImGui                     # Dear ImGui 核心、Vulkan backend 和字体
    │   ├── My_Utils                  # stb_image 和内置图片资源
    │   └── native_surface             # SurfaceComposer/ANativeWindow 封装
    └── src
        ├── main.cpp                  # 程序入口和主循环
        ├── Android_draw              # 示例 UI、字体和纹理初始化
        ├── Android_Graphics          # Vulkan 初始化、渲染、纹理和清理
        ├── Android_my_imgui          # ImGui context 和 Android backend
        ├── Android_touch             # /dev/input 与 /dev/uinput 处理
        ├── ImGui                     # Dear ImGui 实现和 Vulkan backend
        └── My_Utils                  # stb_image 实现
~~~

## 二次开发

### 修改示例 UI

业务 UI 入口是 `jni/src/Android_draw/draw_Gui.cpp` 中的 `Layout_tick_UI()`。不要修改 Vulkan 提交逻辑即可替换示例控件：

~~~cpp
void Layout_tick_UI(bool *main_thread_flag) {
    ImGui::Begin("My Window", main_thread_flag);
    ImGui::Text("Hello from Android ImGui");
    ImGui::Checkbox("Enable feature", &feature_enabled);
    ImGui::End();
}
~~~

主循环的渲染顺序保持如下：

~~~cpp
graphics->NewFrame();
Layout_tick_UI(&flag);
graphics->EndFrame();
~~~

### 加载纹理

`AndroidImgui` 提供文件和内存两种常用入口，返回的 `TextureInfo.DS` 可直接传给 `ImGui::Image()`：

~~~cpp
TextureInfo texture = graphics->LoadTextureFromMemory(image_data, image_size);
ImGui::Image(texture.DS, ImVec2(static_cast<float>(texture.w),
                                static_cast<float>(texture.h)));
~~~

PNG/JPEG 解码由内置 `stb_image` 完成；GIF 使用 `LoadTextureFromMemory_gif()`，每个动画帧会分别创建 Vulkan texture。

### 调整触摸模式

`jni/src/main.cpp` 当前使用：

~~~cpp
Touch::Init({static_cast<float>(abs_ScreenX),
             static_cast<float>(abs_ScreenY)}, false);
~~~

第二个参数控制只读/注入模式。旋转变化由 `Touch::setOrientation(displayInfo.orientation)` 同步到触摸坐标转换。

## 运行时兼容性

- `ANativeWindowCreator` 通过 `dlopen()`/`dlsym()` 访问 `/system/lib64/libgui.so` 和 `/system/lib64/libutils.so`，依赖 Android 内部符号，厂商 ROM 改名或裁剪符号时可能创建 Surface 失败。
- 代码按 `ro.build.version.release` 选择符号表；实际可用性仍取决于设备 Android 版本、厂商实现和进程权限。
- `skipScreenshot`/穿透开关会重新创建窗口，并尝试恢复之前的 ImGui 窗口位置和尺寸。
- Vulkan swapchain 在窗口尺寸或旋转变化后重建；当前实现会等待约 500 ms 以避开屏幕切换瞬间的无效尺寸。
- 当前 `Application.mk` 只构建 `arm64-v8a`，32 位设备不会生成可运行产物。
- 工程没有 Java/Kotlin Activity、Manifest 或 APK 打包层；需要由外部脚本、adb、root shell 或宿主应用负责启动。

## 故障排查

### 构建时报找不到 `ndk-build`

确认 `ANDROID_NDK_HOME` 指向 NDK 根目录，而不是 `build/` 子目录：

~~~powershell
Test-Path "$env:ANDROID_NDK_HOME\ndk-build.cmd"
~~~

### 启动后提示 Vulkan 不可用

~~~powershell
adb shell getprop ro.hardware.vulkan
adb shell dumpsys SurfaceFlinger | Select-String Vulkan
adb logcat -s ImGui Vulkan
~~~

确认设备有 Vulkan 驱动，并且进程可以加载 `libvulkan.so`。

### Surface 创建失败或直接退出

检查设备版本和权限：

~~~powershell
adb shell getprop ro.build.version.release
adb shell su -c 'ls -l /system/lib64/libgui.so /system/lib64/libutils.so'
adb logcat -s ImGui AndroidRuntime
~~~

如果 ROM 没有当前代码需要的内部符号，需要在 `jni/include/native_surface/ANativeWindowCreator.h` 的 `patchesTable` 中为该 ROM 增加对应 mangled symbol，并重新编译。

### 触摸没有响应

~~~powershell
adb shell su -c 'ls -l /dev/input/event* /dev/uinput'
adb shell su -c 'getevent -lp'
~~~

确认进程具备读取输入设备、创建 uinput 设备以及执行 `EVIOCGRAB` 的权限。某些设备的输入节点权限由厂商策略额外限制。

### 画面空白或旋转后错位

先确认 `DisplayInfo.width/height/orientation` 日志，再检查 swapchain 重建日志。窗口尺寸发生变化时，`VulkanGraphics::PrepareFrame()` 会重建 swapchain；如果设备切屏速度较慢，可调整其中的等待时间。

## 依赖与致谢

- [Dear ImGui](https://github.com/ocornut/imgui)：核心 UI、Vulkan backend 和 Android 输入接口基础。
- [stb_image](https://github.com/nothings/stb)：PNG/JPEG/GIF 解码。
- [Font Awesome](https://fontawesome.com/)：内置图标字体。
- Android NDK / Vulkan Android WSI：原生构建、Surface 和 GPU 渲染能力。

Dear ImGui、stb_image 和 Font Awesome 的许可条款请以各自上游文件和官方仓库为准。当前项目目录没有独立的 `LICENSE` 文件；对外发布前请补充项目许可，并保留第三方组件的版权与许可声明。

## 当前验证记录

在 Windows 环境使用 NDK `27.0.12077973` 执行：

~~~powershell
& "D:\Android\Sdk\ndk\27.0.12077973\ndk-build.cmd" -j4
~~~

已验证 `arm64-v8a` 全量 C/C++ 编译、链接和安装步骤通过，产物为 `libs/arm64-v8a/Android_imgui_Vulkan.rc`。物理设备上的 Surface、Vulkan 驱动和触摸行为取决于具体 ROM/权限，需要按上面的 adb 命令进行设备侧验证。

## License

本项目当前未声明独立许可证。请在分发或二次开发前补充许可证文件，并同时遵守 Dear ImGui、stb_image、Font Awesome 及 Android SDK/NDK 的相应条款。
