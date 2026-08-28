#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <thread>

#include "draw.h"
#include "embedded_assets.h"
#include "fontawesome-brands.h"
#include "fontawesome-regular.h"
#include "fontawesome-solid.h"
#include "gui_icon.h"

bool permeate_record = false;
bool permeate_record_ini = false;
struct Last_ImRect LastCoordinate = {0, 0, 0, 0};


std::unique_ptr<AndroidImgui> graphics;
ANativeWindow *window = NULL; 
android::ANativeWindowCreator::DisplayInfo displayInfo;// 屏幕信息
ImGuiWindow *g_window = NULL;// 窗口信息
int abs_ScreenX = 0, abs_ScreenY = 0;// 绝对屏幕X _ Y
int native_window_screen_x = 0, native_window_screen_y = 0;

TextureInfo picture_image{};
TextureInfo_gif congyv_gif{};
int gui_frame_rate = 60;

ImFont* heiti_font = NULL;
ImFont* icon_font_0 = NULL;
ImFont* icon_font_1 = NULL;
ImFont* icon_font_2 = NULL;



bool M_Android_LoadFont(float SizePixels) {
    ImGuiIO &io = ImGui::GetIO();

    ImFontConfig font_config;
    font_config.OversampleH = 2;
    font_config.OversampleV = 1;
    font_config.FontDataOwnedByAtlas = false;
    ::heiti_font = io.Fonts->AddFontFromMemoryTTF(
            const_cast<unsigned char *>(g_heiti_ttf_start),
            static_cast<int>(EmbeddedAssets::HeitiSize()),
            SizePixels, &font_config, io.Fonts->GetGlyphRangesChineseFull());
    if (::heiti_font == nullptr) {
        return false;
    }
    io.FontDefault = ::heiti_font;

	static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 2;
    icons_config.OversampleV = 1;
    icons_config.SizePixels = SizePixels;

	::icon_font_0 = io.Fonts->AddFontFromMemoryCompressedTTF((const void *)&font_awesome_brands_compressed_data, sizeof(font_awesome_brands_compressed_data), 0.0f, &icons_config, icons_ranges);
	::icon_font_1 = io.Fonts->AddFontFromMemoryCompressedTTF((const void *)&font_awesome_regular_compressed_data, sizeof(font_awesome_regular_compressed_data), 0.0f, &icons_config, icons_ranges);
	::icon_font_2 = io.Fonts->AddFontFromMemoryCompressedTTF((const void *)&font_awesome_solid_compressed_data, sizeof(font_awesome_solid_compressed_data), 0.0f, &icons_config, icons_ranges);
    return true;
}
void init_My_drawdata() {
    ImGui::StyleColorsLight(); //白色
    M_Android_LoadFont(25.0f); //加载项目内置黑体与图标
    ImGui::GetStyle().ScaleAllSizes(3.25f);
    ::picture_image = graphics->LoadTextureFromMemory(
            const_cast<unsigned char *>(g_picture_jpg_start),
            static_cast<int>(EmbeddedAssets::PictureSize()));
    ::congyv_gif = graphics->LoadTextureFromMemory_gif(
            const_cast<unsigned char *>(g_congyv_gif_start),
            static_cast<int>(EmbeddedAssets::CongyvGifSize()));
}

void release_My_drawdata() {
    std::free(::congyv_gif.DS);
    std::free(::congyv_gif.delays);
    ::congyv_gif = {};
    ::picture_image = {};
}


void screen_config() {
     static std::chrono::steady_clock::time_point lastTime{};
     if (std::chrono::steady_clock::now() - lastTime < std::chrono::milliseconds(250)) //0.25秒
         return;
         
    const auto next_display_info = android::ANativeWindowCreator::GetDisplayInfo();
    if (next_display_info.width > 0 && next_display_info.height > 0) {
        ::displayInfo = next_display_info;
    }
    
    lastTime = std::chrono::steady_clock::now();
}

static void limit_gui_frame_rate() {
    static std::chrono::steady_clock::time_point next_frame{};
    const auto now = std::chrono::steady_clock::now();
    const int target_fps = std::clamp(gui_frame_rate, 60, 185);
    const auto frame_period = std::chrono::duration_cast<std::chrono::steady_clock::duration>(
            std::chrono::duration<double>(1.0 / target_fps));

    if (next_frame == std::chrono::steady_clock::time_point{}) {
        next_frame = now;
    }
    if (now < next_frame) {
        std::this_thread::sleep_until(next_frame);
    }

    const auto frame_start = std::chrono::steady_clock::now();
    next_frame += frame_period;
    if (next_frame <= frame_start) {
        next_frame = frame_start + frame_period;
    }
}

void drawBegin() {
    limit_gui_frame_rate();
    screen_config();

    const bool valid_display_size = displayInfo.width > 0 && displayInfo.height > 0;
    const bool display_size_changed = valid_display_size
            && (native_window_screen_x != displayInfo.width
                || native_window_screen_y != displayInfo.height);

    if (::permeate_record_ini || display_size_changed) {
        if (::g_window != nullptr) {
            LastCoordinate.Pos_x = ::g_window->Pos.x;
            LastCoordinate.Pos_y = ::g_window->Pos.y;
            LastCoordinate.Size_x = ::g_window->Size.x;
            LastCoordinate.Size_y = ::g_window->Size.y;
        }

        if (display_size_changed) {
            native_window_screen_x = displayInfo.width;
            native_window_screen_y = displayInfo.height;
            abs_ScreenX = displayInfo.width;
            abs_ScreenY = displayInfo.height;
        }

        release_My_drawdata();
        graphics->Shutdown();
        android::ANativeWindowCreator::Destroy(::window);
        ::window = android::ANativeWindowCreator::Create("test_sysGui", native_window_screen_x, native_window_screen_y, permeate_record);
        graphics->Init_Render(::window, native_window_screen_x, native_window_screen_y);
        ::init_My_drawdata(); //初始化绘制数据
        ::g_window = nullptr;
        ::permeate_record_ini = true;

        Touch::UpdateDisplaySize({(float) native_window_screen_x,
                                  (float) native_window_screen_y});
    }

    static int32_t orientation = -1;
    if (orientation != displayInfo.orientation) {
        orientation = displayInfo.orientation;
        Touch::setOrientation(displayInfo.orientation);
    }
}


void Layout_tick_UI(bool *main_thread_flag) {
    static bool show_picture_window = false;
    static bool show_gif_window = false;
    { 
        ImGui::Begin("晚宀-imgui", main_thread_flag);  // Create a window called "Hello, world!" and append into it.
        if (::permeate_record_ini) {
            const float restored_width = std::min(LastCoordinate.Size_x,
                                                  (float) displayInfo.width);
            const float restored_height = std::min(LastCoordinate.Size_y,
                                                   (float) displayInfo.height);
            const float max_x = std::max(0.0f, (float) displayInfo.width - restored_width);
            const float max_y = std::max(0.0f, (float) displayInfo.height - restored_height);
            ImGui::SetWindowSize({restored_width, restored_height});
            ImGui::SetWindowPos({std::clamp(LastCoordinate.Pos_x, 0.0f, max_x),
                                 std::clamp(LastCoordinate.Pos_y, 0.0f, max_y)});
            permeate_record_ini = false;   
        }
        ImGui::Text("渲染接口 : %s, gui版本 : %s", graphics->RenderName, ImGui::GetVersion());               // Display some text (you can use a format strings too)
        if (ImGui::Checkbox("过录制", &::permeate_record)) {
            ::permeate_record_ini = true;
        }
            
        ImGui::Checkbox("绘制图片", &show_picture_window);
        ImGui::Checkbox("绘制 GIF", &show_gif_window);
        ImGui::SliderInt("GUI 帧率", &gui_frame_rate, 60, 185, "%d FPS");
        g_window = ImGui::GetCurrentWindow();
        ImGui::End();
    }
    
        
    if (show_picture_window) {
        ImGui::Begin("图片窗口", &show_picture_window);
        ImGui::Text("内置 JPG（内存加载）");
        if (picture_image.DS != 0) {
            ImGui::Image(picture_image.DS, ImVec2(240, 240));
        } else {
            ImGui::TextDisabled("JPG 加载失败");
        }
        ImGui::End();
    }

    if (show_gif_window) {
        ImGui::Begin("GIF 窗口", &show_gif_window);
        ImGui::Text("内置 GIF（内存加载）");
        if (congyv_gif.DS != nullptr && congyv_gif.frames > 0) {
            static int gif_frame = 0;
            static std::chrono::steady_clock::time_point next_gif_frame{};
            const auto now = std::chrono::steady_clock::now();
            if (gif_frame >= congyv_gif.frames) {
                gif_frame = 0;
                next_gif_frame = {};
            }
            if (next_gif_frame == std::chrono::steady_clock::time_point{}) {
                const int delay = congyv_gif.delays != nullptr && congyv_gif.delays[gif_frame] > 0
                                  ? congyv_gif.delays[gif_frame] : 100;
                next_gif_frame = now + std::chrono::milliseconds(delay);
            } else if (now >= next_gif_frame) {
                gif_frame = (gif_frame + 1) % congyv_gif.frames;
                const int delay = congyv_gif.delays != nullptr && congyv_gif.delays[gif_frame] > 0
                                  ? congyv_gif.delays[gif_frame] : 100;
                next_gif_frame = now + std::chrono::milliseconds(delay);
            }
            if (congyv_gif.DS[gif_frame] != 0) {
                ImGui::Image(congyv_gif.DS[gif_frame], ImVec2(240, 240));
            }
        } else {
            ImGui::TextDisabled("GIF 加载失败");
        }
        ImGui::End();
    }
}
