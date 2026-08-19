#include "draw.h"    //绘制套
#include "AndroidImgui.h"     //创建绘制套
#include "GraphicsManager.h" //获取 当前渲染模式



int main(int argc, char *argv[]) {
    ::graphics = GraphicsManager::getGraphicsInterface();

    //获取屏幕信息    
    ::screen_config(); 

    ::native_window_screen_x = ::displayInfo.width;
    ::native_window_screen_y = ::displayInfo.height;
    ::abs_ScreenX = ::displayInfo.width;
    ::abs_ScreenY = ::displayInfo.height;

    ::window = android::ANativeWindowCreator::Create("test", native_window_screen_x, native_window_screen_y, permeate_record);
    ::graphics->Init_Render(::window, native_window_screen_x, native_window_screen_y);
    
    // 被动监听物理触摸，不使用 EVIOCGRAB 独占设备，底层应用可直接收到原始触摸事件。
    Touch::Init({(float)::abs_ScreenX, (float)::abs_ScreenY}, true);
    Touch::setOrientation(displayInfo.orientation);

    
    ::init_My_drawdata(); //初始化绘制数据
    static bool flag = true;
    while (flag) {
        drawBegin();
        Touch::UpdateImGuiInput();
        graphics->NewFrame(true);
        
        Layout_tick_UI(&flag);

        graphics->EndFrame();        
    }
    
    Touch::Close();
    release_My_drawdata();
    graphics->Shutdown();
    android::ANativeWindowCreator::Destroy(::window);
    return 0;
}
