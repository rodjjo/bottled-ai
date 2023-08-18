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
                        status_cb(r.cast<py11::bool_>(), NULL);
                    } else {
                        status_cb(false, "Dependencies module unloaded!");
                    }
                }
                catch(std::runtime_error e) {
                    status_cb(false, getError(e));
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
                    status_cb(true, NULL, models);  
                } catch(std::runtime_error e) {
                    status_cb(false, getError(e), model_list_t());  
                }

            };
        }

        callback_t download_model(const char *repo_id, status_callback_t status_cb) {
            enable_progress_window();
            return [status_cb, repo_id]()
            {
                try {
                    bottled_ai::py::getModule().attr("download_model")(repo_id);
                    status_cb(true, NULL); 
                }
                catch(std::runtime_error e) {
                    status_cb(false, getError(e)); 
                }
            };
        }

        callback_t generate_text(
            const char *repo_id, 
            const char *instruction, 
            const char *input, 
            int max_new_tokens,
            float temperature,
            float top_p,
            float top_k,
            float repetition_penalty,
            text_callback_t status_cb
        ) {
            enable_progress_window(true);
            return [
                status_cb, 
                repo_id, 
                instruction, 
                input,
                max_new_tokens,
                temperature,
                top_p,
                top_k,
                repetition_penalty
            ]()
            {
                try {
                    py11::dict params;
                    params["prompt"] = input;
                    params["context"] = instruction;
                    params["max_new_tokens"] = max_new_tokens;
                    params["temperature"] = temperature;
                    params["top_p"] = top_p;
                    params["top_k"] = top_k;
                    params["repetition_penalty"] = repetition_penalty;
                    auto r = bottled_ai::py::getModule().attr("generate_text")(repo_id, params);
                    auto html = r["html"].cast<std::string>();
                    auto raw = r["raw"].cast<std::string>();
                    status_cb(true, html.c_str(), raw.c_str());
                }
                catch(std::runtime_error e) {
                    status_cb(false, getError(e), ""); 
                }
            };
        }

    } // namespace py
} // namespace
