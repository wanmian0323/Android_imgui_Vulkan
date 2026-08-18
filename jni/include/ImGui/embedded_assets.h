#ifndef ANDROID_IMGUI_EMBEDDED_ASSETS_H
#define ANDROID_IMGUI_EMBEDDED_ASSETS_H

#include <cstddef>

extern "C" {
extern const unsigned char g_heiti_ttf_start[];
extern const unsigned char g_heiti_ttf_end[];

extern const unsigned char g_picture_jpg_start[];
extern const unsigned char g_picture_jpg_end[];

extern const unsigned char g_congyv_gif_start[];
extern const unsigned char g_congyv_gif_end[];
}

namespace EmbeddedAssets {

inline std::size_t HeitiSize() {
    return static_cast<std::size_t>(g_heiti_ttf_end - g_heiti_ttf_start);
}

inline std::size_t PictureSize() {
    return static_cast<std::size_t>(g_picture_jpg_end - g_picture_jpg_start);
}

inline std::size_t CongyvGifSize() {
    return static_cast<std::size_t>(g_congyv_gif_end - g_congyv_gif_start);
}

} // namespace EmbeddedAssets

#endif // ANDROID_IMGUI_EMBEDDED_ASSETS_H
