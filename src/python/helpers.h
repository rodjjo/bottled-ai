#pragma once

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include <Python.h>
#include <pybind11/embed.h>

namespace py11 = pybind11;

namespace bottled_ai {
namespace py {

typedef struct  {
    bool locally;
    std::string id;
    std::string name;
    std::string loader;
    std::string description;
    std::vector<std::string> templates;
} model_t;

typedef std::vector<model_t> model_list_t;

typedef std::function<void()> callback_t;
typedef std::function<void(bool success, const char *message)> status_callback_t;
typedef std::function<void(bool success, const char *html, const char *raw)> text_callback_t;
typedef std::function<void(bool success, const char *message, const model_list_t &models)> model_callback_t;

callback_t check_have_deps(status_callback_t status_cb);
callback_t install_deps(status_callback_t status_cb);
callback_t list_models(model_callback_t status_cb);
callback_t download_model(const char *repo_id, status_callback_t status_cb);
callback_t remove_model(const char *repo_id, status_callback_t status_cb);
callback_t generate_text(
    const char *repo_id, 
    const char *input, 
    text_callback_t status_cb);

}  // namespace py

std::string escape_html(const std::string& data);

}  // namespace

