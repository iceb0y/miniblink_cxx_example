#include "window.h"

#include <array>
#include <utility>
#include "filesystem.h"

namespace miniblink_cxx_example {

Window::Window(wkeWindowType type, HWND parent_hwnd, int width, int height)
    : window_(wkeCreateWebWindow(type, parent_hwnd, 0, 0, width, height)) {
    install_filesystem(window_);
    wkeOnWindowDestroy(window_, &on_window_destroy_stub, this);
}

Window::~Window() {
    if (window_) {
        wkeDestroyWebWindow(window_);
    }
}

void Window::on_document_ready(std::function<void()> callback) {
    document_ready_callback_ = std::move(callback);
    wkeOnDocumentReady(window_, &on_document_ready_stub, this);
}

void Window::on_window_closing(std::function<bool()> callback) {
    window_closing_callback_ = std::move(callback);
    wkeOnWindowClosing(window_, &on_window_closing_stub, this);
}

void Window::call(const char *name) {
    if (!window_) {
        return;
    }
    jsExecState es = wkeGlobalExec(window_);
    jsValue app = jsGetGlobal(es, "app");
    jsValue func = jsGet(es, app, name);
    jsCall(es, func, app, nullptr, 0);
}

void WKE_CALL_TYPE Window::on_document_ready_stub(wkeWebView, void *param) {
    reinterpret_cast<Window *>(param)->document_ready_callback_();
}

bool WKE_CALL_TYPE Window::on_window_closing_stub(wkeWebView, void *param) {
    return reinterpret_cast<Window *>(param)->window_closing_callback_();
}

void WKE_CALL_TYPE Window::on_window_destroy_stub(wkeWebView, void *param) {
    Window &window = *reinterpret_cast<Window *>(param);
    if (window.quit_on_close_) {
        PostQuitMessage(0);
    }
    window.window_ = nullptr;
}

jsValue to_js_value(jsExecState, bool value) {
    return jsBoolean(value);
}

jsValue to_js_value(jsExecState, int value) {
    return jsInt(value);
}

jsValue to_js_value(jsExecState, double value) {
    return jsDouble(value);
}

jsValue to_js_value(jsExecState es, const char *value) {
    return jsString(es, value);
}

jsValue to_js_value(jsExecState es, const wchar_t *value) {
    return jsStringW(es, value);
}

jsValue to_js_value(jsExecState es, const std::string &value) {
    return jsString(es, value.c_str());
}

jsValue to_js_value(jsExecState es, const std::wstring &value) {
    return jsStringW(es, value.c_str());
}

}  // namespace miniblink_cxx_example
