#include "src/python/utils.h"
#include "src/python/helpers.h"
#include "src/python/wrapper.h"

namespace bottled_ai
{

std::string convert2html(const std::string& text) {
    std::string result;
    py::get_py()->execute_callback(
            py::convert2html(
                text.c_str(), 
                [&result] (bool success, const char *html) {
                    if (html) {
                        result = html;
                    }
            })
        );
    return result;
}

} // namespace bottled_ai
