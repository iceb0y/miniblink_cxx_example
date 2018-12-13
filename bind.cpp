#include "bind.h"

#include <unordered_map>

namespace miniblink_cxx_example {
namespace {

using Callback = std::function<jsValue(jsExecState)>;

template <typename ...T>
using HashMap = std::unordered_map<T...>;

class Binder {
public:
    Binder() = default;
    Binder(const Binder &) = delete;
    Binder &operator=(const Binder &) = delete;
    ~Binder() = delete;

    void bind(const char *name, int arg_count, Callback callback);

private:
    HashMap<std::string, Callback> callbacks_;
};

Binder *binder() {
    static Binder *binder = new Binder;
    return binder;
}

jsValue WKE_CALL_TYPE binder_stub(jsExecState es, void *param) {
    return reinterpret_cast<Callback *>(param)->operator()(es);
}

void Binder::bind(
    const char *name, int arg_count,
    std::function<jsValue(jsExecState)> callback) {
    Callback &moved_callback = callbacks_[name] = std::move(callback);
    wkeJsBindFunction(name, &binder_stub, &moved_callback, arg_count);
}

}  // namespace

void bind(const char *name, std::function<void()> callback) {
    binder()->bind(name, 0, [callback = std::move(callback)](jsExecState) {
        callback();
        return jsUndefined();
    });
}

void bind(const char *name, std::function<void(JsValueShim)> callback) {
    binder()->bind(name, 1, [callback = std::move(callback)](jsExecState es) {
        callback({ es, 0 });
        return jsUndefined();
    });
}

void bind(
    const char *name,
    std::function<void(JsValueShim, JsValueShim)> callback) {
    binder()->bind(name, 2, [callback = std::move(callback)](jsExecState es) {
        callback({ es, 0 }, { es, 1 });
        return jsUndefined();
    });
}

void bind(
    const char *name,
    std::function<void(JsValueShim, JsValueShim, JsValueShim)> callback) {
    binder()->bind(name, 3, [callback = std::move(callback)](jsExecState es) {
        callback({ es, 0 }, { es, 1 }, { es, 2 });
        return jsUndefined();
    });
}

void bind(
    const char *name,
    std::function<void(
        JsValueShim, JsValueShim, JsValueShim, JsValueShim)> callback) {
    binder()->bind(name, 4, [callback = std::move(callback)](jsExecState es) {
        callback({ es, 0 }, { es, 1 }, { es, 2 }, { es, 3 });
        return jsUndefined();
    });
}

}  // namespace miniblink_cxx_example
