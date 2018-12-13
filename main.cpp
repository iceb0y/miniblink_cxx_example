#include <windows.h>
#include "bind.h"
#include "window.h"
#include "wke.h"

int WINAPI wWinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    using namespace miniblink_cxx_example;

    if (!wkeInitialize()) {
        return 1;
    }

    Window window(WKE_WINDOW_TYPE_POPUP, NULL, 800, 600);
    bind("add", [&window](int a, int b) {
        window.call("setValue", a + b);
    });
    window.load(L"app:///index.html");
    window.set_quit_on_close();
    window.show();

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
