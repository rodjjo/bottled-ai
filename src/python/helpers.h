/*
 * Copyright (C) 2023 by Rodrigo Antonio de Araujo
 */
#ifndef SRC_PYTHON_HELPERS_H_
#define SRC_PYTHON_HELPERS_H_

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
typedef std::function<void(bool success, const char *message, const model_list_t &models)> model_callback_t;

callback_t check_have_deps(status_callback_t status_cb);
callback_t install_deps(status_callback_t status_cb);
callback_t list_models(model_callback_t status_cb);
callback_t download_model(const char *repo_id, status_callback_t status_cb);
callback_t generate_text(const char *repo_id, const char *instruction, const char *input, status_callback_t status_cb);


}  // namespace py
}  // namespace

#endif  // SRC_PYTHON_HELPERS_H_
