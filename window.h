#ifndef _MINIBLINK_CXX_EXAMPLE_WINDOW_H
#define _MINIBLINK_CXX_EXAMPLE_WINDOW_H

#include <windows.h>
#include <array>
#include <functional>
#include <string>
#include "wke.h"

namespace miniblink_cxx_example {

// This class is not thread-safe.
class Window {
public:
    Window(wkeWindowType type, HWND parent_hwnd, int width, int height);
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    ~Window();

    void set_title(const wchar_t *s) { wkeSetWindowTitleW(window_, s); }
    void on_document_ready(std::function<void()> callback);
    void on_window_closing(std::function<bool()> callback);
    void load(const wchar_t *url) { wkeLoadURLW(window_, url); }
    void call(const char *name);
    template <typename T0>
    void call(const char *name, T0 arg0);
    template <typename T0, typename T1>
    void call(const char *name, T0 arg0, T1 arg1);
    template <typename T0, typename T1, typename T2>
    void call(const char *name, T0 arg0, T1 arg1, T2 arg2);
    template <typename T0, typename T1, typename T2, typename T3>
    void call(const char *name, T0 arg0, T1 arg1, T2 arg2, T3 arg3);

    void show() { wkeShowWindow(window_, true); }
    void hide() { wkeShowWindow(window_, false); }
    void minimize() { ShowWindow(hwnd(), SW_MINIMIZE); }
    void enable() { wkeEnableWindow(window_, true); }
    void disable() { wkeEnableWindow(window_, false); }
    void clear_quit_on_close() { quit_on_close_ = false; }
    void set_quit_on_close() { quit_on_close_ = true; }
    void close() { PostMessageW(hwnd(), WM_CLOSE, 0, 0); }

    HWND hwnd() const { return wkeGetWindowHandle(window_); }

private:
    static void WKE_CALL_TYPE on_document_ready_stub(wkeWebView, void *param);
    static bool WKE_CALL_TYPE on_window_closing_stub(wkeWebView, void *param);
    static void WKE_CALL_TYPE on_window_destroy_stub(wkeWebView, void *param);

    wkeWebView window_;
    bool quit_on_close_ = false;
    std::function<void()> document_ready_callback_;
    std::function<bool()> window_closing_callback_;
};

jsValue to_js_value(jsExecState, bool value);
jsValue to_js_value(jsExecState, int value);
jsValue to_js_value(jsExecState, double value);
jsValue to_js_value(jsExecState es, const char *value);
jsValue to_js_value(jsExecState es, const wchar_t *value);
jsValue to_js_value(jsExecState es, const std::string &value);
jsValue to_js_value(jsExecState es, const std::wstring &value);

template <typename T0>
void Window::call(const char *name, T0 arg0) {
    if (!window_) {
        return;
    }
    jsExecState es = wkeGlobalExec(window_);
    jsValue func = jsGetGlobal(es, name);
    std::array<jsValue, 1> js_args;
    js_args[0] = to_js_value(es, arg0);
    jsCall(
        es, func, jsUndefined(), js_args.data(),
        static_cast<int>(js_args.size()));
}

template <typename T0, typename T1>
void Window::call(const char *name, T0 arg0, T1 arg1) {
    if (!window_) {
        return;
    }
    jsExecState es = wkeGlobalExec(window_);
    jsValue func = jsGetGlobal(es, name);
    std::array<jsValue, 2> js_args;
    js_args[0] = to_js_value(es, arg0);
    js_args[1] = to_js_value(es, arg1);
    jsCall(
        es, func, jsUndefined(), js_args.data(),
        static_cast<int>(js_args.size()));
}

template <typename T0, typename T1, typename T2>
void Window::call(const char *name, T0 arg0, T1 arg1, T2 arg2) {
    if (!window_) {
        return;
    }
    jsExecState es = wkeGlobalExec(window_);
    jsValue func = jsGetGlobal(es, name);
    std::array<jsValue, 3> js_args;
    js_args[0] = to_js_value(es, arg0);
    js_args[1] = to_js_value(es, arg1);
    js_args[2] = to_js_value(es, arg2);
    jsCall(
        es, func, jsUndefined(), js_args.data(),
        static_cast<int>(js_args.size()));
}

template <typename T0, typename T1, typename T2, typename T3>
void Window::call(const char *name, T0 arg0, T1 arg1, T2 arg2, T3 arg3) {
    if (!window_) {
        return;
    }
    jsExecState es = wkeGlobalExec(window_);
    jsValue func = jsGetGlobal(es, name);
    std::array<jsValue, 4> js_args;
    js_args[0] = to_js_value(es, arg0);
    js_args[1] = to_js_value(es, arg1);
    js_args[2] = to_js_value(es, arg2);
    js_args[3] = to_js_value(es, arg3);
    jsCall(
        es, func, jsUndefined(), js_args.data(),
        static_cast<int>(js_args.size()));
}

}  // namespace miniblink_cxx_example

#endif  // _MINIBLINK_CXX_EXAMPLE_WINDOW_H
