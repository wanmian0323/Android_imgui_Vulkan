cmake_minimum_required(VERSION 3.22)

project(AndroidImguiVulkan LANGUAGES CXX ASM)

set(JNI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/jni")
set(SRC_DIR "${JNI_DIR}/src")

set(IMGUI_SOURCES
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/Android_draw/draw_Gui.cpp"
    "${SRC_DIR}/ImGui/TouchHelperA.cpp"
    "${SRC_DIR}/ImGui/AndroidImgui.cpp"
    "${SRC_DIR}/ImGui/my_imgui_impl_android.cpp"
    "${SRC_DIR}/ImGui/stb_image.cpp"
    "${SRC_DIR}/ImGui/imgui.cpp"
    "${SRC_DIR}/ImGui/imgui_draw.cpp"
    "${SRC_DIR}/ImGui/imgui_tables.cpp"
    "${SRC_DIR}/ImGui/imgui_widgets.cpp"
    "${SRC_DIR}/ImGui/imgui_impl_vulkan.cpp"
    "${SRC_DIR}/Vulkan/GraphicsManager.cpp"
    "${SRC_DIR}/Vulkan/VulkanGraphics.cpp"
    "${SRC_DIR}/Vulkan/vulkan_wrapper.cpp"
    "${SRC_DIR}/ImGui/embedded_assets.S"
)

add_executable(AndroidImguiVulkan ${IMGUI_SOURCES})

set_target_properties(AndroidImguiVulkan PROPERTIES
    OUTPUT_NAME "Android_imgui_Vulkan.rc"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}"
)

target_include_directories(AndroidImguiVulkan PRIVATE
    "${JNI_DIR}/include"
    "${JNI_DIR}/include/Android_draw"
    "${JNI_DIR}/include/ImGui"
    "${JNI_DIR}/include/Vulkan"
)

target_compile_definitions(AndroidImguiVulkan PRIVATE
    VK_USE_PLATFORM_ANDROID_KHR
    IMGUI_IMPL_VULKAN_NO_PROTOTYPES
    IMGUI_DISABLE_DEBUG_TOOLS
)

target_compile_options(AndroidImguiVulkan PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-fvisibility=hidden>
    $<$<COMPILE_LANGUAGE:ASM>:-I${JNI_DIR}/include>
)

target_link_libraries(AndroidImguiVulkan PRIVATE
    log
    android
)

# Keep the embedded .incbin assets visible to IDEs and rebuild when they change.
set_source_files_properties("${SRC_DIR}/ImGui/embedded_assets.S" PROPERTIES
    OBJECT_DEPENDS
        "${JNI_DIR}/include/素材/heiti.ttf;${JNI_DIR}/include/素材/picture.jpg;${JNI_DIR}/include/素材/congyv.gif"
)

