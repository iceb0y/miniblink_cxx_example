#include "filesystem.h"

namespace miniblink_cxx_example {
namespace {

class Filesystem {
public:
    Filesystem() = default;
    Filesystem(const Filesystem &) = delete;
    Filesystem &operator=(const Filesystem &) = delete;
    ~Filesystem() = delete;

    bool handle_request(wkeNetJob job, const char *url) const;
};

const char index_html[] = R"(<html>
<body>
<input id="a" value="1"> + <input id="b" value="2">
<input id="add" type="button" value=" = ">
<input id="c">
<script>
document.getElementById('add').onclick = function() {
  add(
      document.getElementById('a').value,
      document.getElementById('b').value);
};

function setValue(value) {
  document.getElementById('c').value = value;
}
</script>
</body>
</html>
)";

bool Filesystem::handle_request(wkeNetJob job, const char *url) const {
    if (!strcmp(url, "app:///index.html")) {
        wkeNetSetData(
            job,
            const_cast<char *>(index_html),
            static_cast<int>(sizeof(index_html) - 1));
        return true;
    }
    return false;
}

bool WKE_CALL_TYPE on_load_url_begin(
    wkeWebView webView, void *param, const char *url, wkeNetJob job) {
    return reinterpret_cast<Filesystem *>(param)->handle_request(job, url);
}

}  // namespace

void install_filesystem(wkeWebView webView) {
    static Filesystem *fs = new Filesystem;
    wkeOnLoadUrlBegin(webView, &on_load_url_begin, fs);
}

}  // miniblink_cxx_example
