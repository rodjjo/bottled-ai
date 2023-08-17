#include <list>
#include <Python.h>

#include "src/config/config.h"
#include "src/python/helpers.h"
#include "src/windows/progress_window.h"
#include "src/python/wrapper.h"

namespace bottled_ai
{
    namespace py
    {
        const char *getError(const std::runtime_error &e) {
            static std::string error;
            error = e.what();
            return error.c_str();
        }

        const char *errorFromPyDict(py11::dict &result, const char *def)
        {
            static std::string buffer;
            buffer = def;
            if (result.contains("error"))
            {
                buffer = result["error"].cast<std::string>();
                return buffer.c_str();
            }
            return buffer.c_str();
        }

        callback_t install_deps(status_callback_t status_cb)
        {
            return [status_cb]
            {
                try {
                    auto m =  bottled_ai::py::depsModule();
                    if (m) {
                        m->attr("install_dependencies")();
                        status_cb(true, NULL); 
                    } else {
                        status_cb(false, "Dependencies module unloaded!"); 
                    }
                }
                catch(std::runtime_error e) {
                    status_cb(false, getError(e)); 
                }
            };
        }

        callback_t check_have_deps(status_callback_t status_cb)
        {
            return [status_cb]
            {
                try {
                    auto m =  bottled_ai::py::depsModule();
                    if (m) {
                        auto r = m->attr("have_dependencies")();
                        status_cb(r.cast<py11::bool_>(), NULL); // TODO: check error!
                    } else {
                        status_cb(false, "Dependencies module unloaded!"); // TODO: check error!
                    }
                }
                catch(std::runtime_error e) {
                    status_cb(false, getError(e)); // TODO: check error!
                }
            };
        }

        callback_t list_models(model_callback_t status_cb)
        {
            return [status_cb]
            {
                try {
                    model_list_t models;
                    auto r = bottled_ai::py::getModule().attr("list_models")();
                    auto seq = r.cast<py11::sequence>();
                    for (size_t i = 0; i < seq.size(); ++i)
                    {
                        auto it = seq[i].cast<py11::dict>();
                        model_t model;
                        model.id = it["id"].cast<std::string>();
                        model.name = it["name"].cast<std::string>();
                        model.description = it["description"].cast<std::string>();
                        model.loader = it["loader"].cast<std::string>();
                        model.locally = it["locally"].cast<bool>();
                        // model.templates = it["templates"].cast<std::vector<std::string> >();
                        models.push_back(model);
                    }
                    status_cb(true, NULL, models); // TODO: check error!
                } catch(std::runtime_error e) {
                    status_cb(false, getError(e), model_list_t()); // TODO: check error!
                }

            };
        }

        callback_t download_model(const char *repo_id, status_callback_t status_cb) {
            enable_progress_window();
            return [status_cb, repo_id]()
            {
                try {
                    bottled_ai::py::getModule().attr("download_model")(repo_id);
                    status_cb(true, NULL); // TODO: check error!
                }
                catch(std::runtime_error e) {
                    status_cb(false, getError(e)); // TODO: check error!
                }
            };
        }

        callback_t generate_text(const char *repo_id, const char *input, status_callback_t status_cb) {
            enable_progress_window();
            return [status_cb, repo_id, input]()
            {
                try {
                    auto r = bottled_ai::py::getModule().attr("generate_text")(repo_id, input);
                    auto response = r["response"].cast<std::string>();
                    status_cb(true, response.c_str()); // TODO: check error!
                }
                catch(std::runtime_error e) {
                    status_cb(false, getError(e)); // TODO: check error!
                }
            };
        }

    } // namespace py
} // namespace
