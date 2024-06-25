//#include <iostream>
//#include <windows.h>
//
//static HINSTANCE g_instance_handle = NULL;
//static HWND g_window_handle = NULL;
//
//static HDC g_buffer_dc_handle = NULL;
//static UINT32 *g_pixel_buffer = NULL;
//static SIZE_T g_pixel_buffer_size = 0;
//
////static lv_disp_t* g_display = NULL;
//
//static bool volatile g_mouse_pressed = false;
//static LPARAM volatile g_mouse_value = 0;
//
//static bool volatile g_mousewheel_pressed = false;
//static int16_t volatile g_mousewheel_value = 0;
//
//static bool volatile g_keyboard_pressed = false;
//static WPARAM volatile g_keyboard_value = 0;
//
//static int volatile g_dpi_value = USER_DEFAULT_SCREEN_DPI;
//
//bool lv_win32_init(HINSTANCE instance_handle, int show_window_mode, lv_coord_t hor_res, lv_coord_t ver_res,
//                   HICON icon_handle)
//{
//    WNDCLASSEXW WindowClass;
//
//    WindowClass.cbSize = sizeof(WNDCLASSEX);
//
//    WindowClass.style = 0;
//    WindowClass.lpfnWndProc = lv_win32_window_message_callback;
//    WindowClass.cbClsExtra = 0;
//    WindowClass.cbWndExtra = 0;
//    WindowClass.hInstance = instance_handle;
//    WindowClass.hIcon = icon_handle;
//    WindowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
//    WindowClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
//    WindowClass.lpszMenuName = NULL;
//    WindowClass.lpszClassName = L"lv_sim_visual_studio";
//    WindowClass.hIconSm = icon_handle;
//
//    if (!RegisterClassExW(&WindowClass)) {
//        return false;
//    }
//
//    g_instance_handle = instance_handle;
//
//    g_window_handle = CreateWindowExW(
//            WINDOW_EX_STYLE,
//            WindowClass.lpszClassName,
//            L"LVGL Simulator for Windows Desktop",
//            WINDOW_STYLE,
//            CW_USEDEFAULT,
//            0,
//            CW_USEDEFAULT,
//            0,
//            NULL,
//            NULL,
//            instance_handle,
//            NULL);
//
//    if (!g_window_handle) {
//        return false;
//    }
//
//    g_dpi_value = lv_win32_get_dpi_for_window(g_window_handle);
//
//    RECT WindowSize;
//
//    WindowSize.left = 0;
//    WindowSize.right = MulDiv(
//            hor_res * WIN32DRV_MONITOR_ZOOM,
//            g_dpi_value,
//            USER_DEFAULT_SCREEN_DPI);
//    WindowSize.top = 0;
//    WindowSize.bottom = MulDiv(
//            ver_res * WIN32DRV_MONITOR_ZOOM,
//            g_dpi_value,
//            USER_DEFAULT_SCREEN_DPI);
//
//    AdjustWindowRectEx(
//            &WindowSize,
//            WINDOW_STYLE,
//            FALSE,
//            WINDOW_EX_STYLE);
//    OffsetRect(
//            &WindowSize,
//            -WindowSize.left,
//            -WindowSize.top);
//
//    SetWindowPos(
//            g_window_handle,
//            NULL,
//            0,
//            0,
//            WindowSize.right,
//            WindowSize.bottom,
//            SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
//
////    lv_win32_register_touch_window(g_window_handle, 0);
////
////    lv_timer_create(lv_win32_message_handler, 0, NULL);
////
////    lv_win32_enable_child_window_dpi_message(g_window_handle);
//
//    HDC hNewBufferDC = lv_win32_create_frame_buffer(
//            g_window_handle,
//            hor_res,
//            ver_res,
//            &g_pixel_buffer,
//            &g_pixel_buffer_size);
//
//    DeleteDC(g_buffer_dc_handle);
//    g_buffer_dc_handle = hNewBufferDC;
//
////    static lv_disp_draw_buf_t display_buffer;
////    lv_disp_draw_buf_init(
////            &display_buffer,
////            (lv_color_t *) malloc(hor_res * ver_res * sizeof(lv_color_t)),
////            NULL,
////            hor_res * ver_res);
//
////    static lv_disp_drv_t display_driver;
////    lv_disp_drv_init(&display_driver);
////    display_driver.hor_res = hor_res;
////    display_driver.ver_res = ver_res;
////    display_driver.flush_cb = lv_win32_display_driver_flush_callback;
////    display_driver.draw_buf = &display_buffer;
////    display_driver.rounder_cb = lv_win32_display_driver_rounder_callback;
////    g_display = lv_disp_drv_register(&display_driver);
////
////    static lv_indev_drv_t pointer_driver;
////    lv_indev_drv_init(&pointer_driver);
////    pointer_driver.type = LV_INDEV_TYPE_POINTER;
////    pointer_driver.read_cb = lv_win32_pointer_driver_read_callback;
////    lv_win32_pointer_device_object = lv_indev_drv_register(&pointer_driver);
////
////    static lv_indev_drv_t keypad_driver;
////    lv_indev_drv_init(&keypad_driver);
////    keypad_driver.type = LV_INDEV_TYPE_KEYPAD;
////    keypad_driver.read_cb = lv_win32_keypad_driver_read_callback;
////    lv_win32_keypad_device_object = lv_indev_drv_register(&keypad_driver);
////
////    static lv_indev_drv_t encoder_driver;
////    lv_indev_drv_init(&encoder_driver);
////    encoder_driver.type = LV_INDEV_TYPE_ENCODER;
////    encoder_driver.read_cb = lv_win32_encoder_driver_read_callback;
////    lv_win32_encoder_device_object = lv_indev_drv_register(&encoder_driver);
//
//    ShowWindow(g_window_handle, show_window_mode);
//    UpdateWindow(g_window_handle);
//
//    return true;
//}
//
//
//int main()
//{
//    std::cout << "This is test1/main.cpp" << std::endl;
//    std::cout << "Hello, world!" << std::endl;
//    return 0;
//}
