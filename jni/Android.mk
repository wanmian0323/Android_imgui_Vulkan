LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := Android_imgui_Vulkan.rc

LOCAL_CFLAGS := -std=c17
LOCAL_CFLAGS += -fvisibility=hidden
LOCAL_CPPFLAGS := -std=c++17
LOCAL_CPPFLAGS += -fvisibility=hidden

LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR
LOCAL_CPPFLAGS += -DIMGUI_IMPL_VULKAN_NO_PROTOTYPES
LOCAL_CPPFLAGS += -DIMGUI_DISABLE_DEBUG_TOOLS #禁用imgui调试工具

#引入头文件到全局#
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Android_draw
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/ImGui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Vulkan



LOCAL_SRC_FILES := src/main.cpp
LOCAL_SRC_FILES += src/Android_draw/draw_Gui.cpp
LOCAL_SRC_FILES += src/ImGui/TouchHelperA.cpp
LOCAL_SRC_FILES += src/ImGui/AndroidImgui.cpp
LOCAL_SRC_FILES += src/ImGui/my_imgui_impl_android.cpp
LOCAL_SRC_FILES += src/ImGui/stb_image.cpp
LOCAL_SRC_FILES += src/Vulkan/GraphicsManager.cpp
LOCAL_SRC_FILES += src/Vulkan/VulkanGraphics.cpp
LOCAL_SRC_FILES += src/Vulkan/vulkan_wrapper.cpp
LOCAL_SRC_FILES += src/ImGui/imgui.cpp
LOCAL_SRC_FILES += src/ImGui/imgui_draw.cpp
LOCAL_SRC_FILES += src/ImGui/imgui_tables.cpp
LOCAL_SRC_FILES += src/ImGui/imgui_widgets.cpp
LOCAL_SRC_FILES += src/ImGui/imgui_impl_vulkan.cpp
    


LOCAL_LDLIBS := -llog -landroid

include $(BUILD_EXECUTABLE) #可执行文件
