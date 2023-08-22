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
            std::string additional_dir = getConfig().getAdditionalModelDir();
            return [status_cb, additional_dir]
            {
                try {
                    model_list_t models;
                    auto r = bottled_ai::py::getModule().attr("list_models")(additional_dir.c_str());
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
                        model.can_remove = false;
                        if (it.contains("can_remove")) {
                            model.can_remove = it["can_remove"].cast<bool>();
                        }
                        models.push_back(model);
                    }
                    status_cb(true, NULL, models);  
                } catch(std::runtime_error e) {
                    status_cb(false, getError(e), model_list_t());  
                }

            };
        }

        callback_t download_model(const char *repo_id, status_callback_t status_cb) {
            enable_progress_window(false, false);
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

        callback_t remove_model(const char *repo_id, status_callback_t status_cb) {
            return [status_cb, repo_id]()
            {
                try {
                    bottled_ai::py::getModule().attr("remove_model")(repo_id);
                    status_cb(true, NULL); 
                }
                catch(std::runtime_error e) {
                    status_cb(false, getError(e)); 
                }
            };
        }

        callback_t convert2html(
            const char *text, 
            status_callback_t status_cb
        ) {
            return [
                status_cb, 
                text
            ]()
            {
                try {
                    auto r = bottled_ai::py::getModule().attr("convert2html")(text);
                    auto html = r.cast<std::string>();
                    status_cb(true, html.c_str());
                }
                catch(std::runtime_error e) {
                    status_cb(false, getError(e)); 
                }
            };
        }
        
        callback_t generate_text(
            const char *repo_id, 
            const char *input, 
            text_callback_t status_cb
        ) {
            auto & c = getConfig();
            model_config_t cfg = c.getModelConfig(repo_id);
            int mem_gpu = -1, mem_cpu = -1;
            c.getMaxMemory(mem_gpu, mem_cpu);

            std::string additional_model_dir = c.getAdditionalModelDir();
            std::string instruction = cfg.context;
            int max_new_tokens = cfg.max_new_tokens;
            float temperature = cfg.temperature;
            float top_p = cfg.top_p;
            float top_k = cfg.top_k;
            float repetition_penalty = cfg.repetition_penalty;


            enable_progress_window(true);
            return [
                status_cb, 
                repo_id, 
                instruction, 
                additional_model_dir,
                input,
                max_new_tokens,
                temperature,
                top_p,
                top_k,
                repetition_penalty,
                mem_gpu,
                mem_cpu
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
                    params["mem_gpu"] = mem_gpu;
                    params["mem_cpu"] = mem_cpu;
                    params["additional_model_dir"] = additional_model_dir;
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
