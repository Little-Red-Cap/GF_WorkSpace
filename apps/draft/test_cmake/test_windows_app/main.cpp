#include <iostream>

#include <windows.h>
// 窗口处理函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wparam, LPARAM lparam){
    return DefWindowProc(hWnd, msgID, wparam, lparam);
}
// 入口函数
int CALLBACK WinMain(HINSTANCE hIns, HINSTANCE hPreIns, LPSTR lpCmdLine, int nCmdShow){
    // 注册窗口类
    WNDCLASS wndclass = {0};
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wndclass.hCursor = nullptr;
    wndclass.hIcon = nullptr;
    wndclass.hInstance = hIns;
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszClassName = "Main";
    wndclass.lpszMenuName = nullptr;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    // 将以上所有赋值全部写入操作系统
    RegisterClass(&wndclass);
    // 在内存中创建窗口
    HWND hwnd = CreateWindow("Main","window", WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, nullptr, nullptr, hIns, nullptr);
    // 显示窗口
    ShowWindow(hwnd, SW_SHOW);
//    UpdateWindow(hwnd);
    // 消息循环
    MSG msg = {nullptr};
    while (GetMessage(&msg, nullptr, 0, 0)){
        TranslateMessage(&msg);
        // 将消息交给窗口处理函数来处理
        DispatchMessage(&msg);
    }
    return 0;
}


int main() {
    std::cout << "This is test_windows_app/main.cpp" << std::endl;
    std::cout << "Hello, world!" << std::endl;
    return 0;
}


#pragma comment( linker, "/subsystem:\"windows\" /entry:\"WinMainCRTStartup\"" ) // 告诉链接器使用windows子系统，入口点为WinMainCRTStartup，有窗口的子系统
//#pragma comment( linker, "/subsystem:\"console\" /entry:\"mainCRTStartup\"" ) // 告诉链接器使用控制台子系统，入口点为mainCRTStartup，有窗口的控制台子系统
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") // 告诉链接器使用windows子系统，入口点为mainCRTStartup，无窗口的控制台子系统


//#include<Windows.h>
//LRESULT CALLBACK WinSunProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
//{
//    switch (uMsg)//通过判断消息进行消息响应
//    {
//        case WM_CLOSE:
//            DestroyWindow(hwnd);//销毁窗口并发送WM_DESTROY消息，但是程序没有退出
//            break;
//        case WM_DESTROY:
//            PostQuitMessage(0);//发出WM_QUIT消息，结束消息循环
//            break;
//        default:
//            return DefWindowProc(hwnd, uMsg, wParam, lParam);//对不感兴趣的消息进行缺省处理，必须有该代码，否则程序有问题
//    }
//    return 0;
//}
//
//int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd) {
//    WNDCLASS wndcls; //创建一个窗体类
//    wndcls.cbClsExtra = 0;//类的额外内存，默认为0即可
//    wndcls.cbWndExtra = 0;//窗口的额外内存，默认为0即可
//    wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//获取画刷句柄（将返回的HGDIOBJ进行强制类型转换）
//    wndcls.hCursor = LoadCursor(NULL, IDC_CROSS);//设置光标
//    wndcls.hIcon = LoadIcon(NULL, IDI_ERROR);//设置窗体左上角的图标
//    wndcls.hInstance = hInstance;//设置窗体所属的应用程序实例
//    wndcls.lpfnWndProc = WinSunProc;//设置窗体的回调函数，暂时没写，先设置为NULL，后面补上
//    wndcls.lpszClassName = reinterpret_cast<LPCSTR>(L"test");//设置窗体的类名
//    wndcls.lpszMenuName = NULL;//设置窗体的菜单,没有，填NULL
//    wndcls.style = CS_HREDRAW | CS_VREDRAW;//设置窗体风格为水平重画和垂直重画
//    RegisterClass(&wndcls);//向操作系统注册窗体
//
//
//    //产生一个窗体，并返回该窗体的句柄，第一个参数必须为要创建的窗体的类名，第二个参数为窗体标题名
//    HWND hwnd = CreateWindow(reinterpret_cast<LPCSTR>(L"test"), reinterpret_cast<LPCSTR>(L"我的第一个窗口"),
//                             WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
//                             CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
//                             NULL, NULL, hInstance, NULL);
//
//    ShowWindow(hwnd, SW_SHOWNORMAL);//把窗体显示出来
//    UpdateWindow(hwnd);//更新窗体
//
//    MSG msg;
//    //消息循环
//    while (GetMessage(&msg, NULL, 0, 0))//如果消息不是WM_QUIT,返回非零值；如果消息是WM_QUIT，返回零
//    {
//        TranslateMessage(&msg);//翻译消息，如把WM_KEYDOWN和WM_KEYUP翻译成一个WM_CHAR消息
//        DispatchMessage(&msg);//派发消息
//    }
//
//}

