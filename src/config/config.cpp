#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <FL/Fl.H>
#include <nlohmann/json.hpp>

#include "src/config/config.h"

using json = nlohmann::json;


namespace bottled_ai {
namespace {
const wchar_t *kCONFIG_FILE = L"/bottled_ai.json";
}

Config::Config() {
    load();
}

Config::~Config() {
}

const std::wstring& Config::executableDir() {
    if (executableDir_.empty()) {
#ifdef _WIN32
    wchar_t path[1024] = { 0, };
    if (GetModuleFileNameW(NULL, path, (sizeof(path) / sizeof(wchar_t)) -1) != 0) {
        executableDir_ = path;
    }
#else
    //TODO: create linux implementation
#endif
        size_t latest = executableDir_.find_last_of(L"/\\");
        if (latest != std::wstring::npos) {
            executableDir_ = executableDir_.substr(0, latest);
        } else {
            executableDir_ = std::wstring();
        }
    }

    return executableDir_;
}

const std::wstring& Config::librariesDir() {
    if (librariesDir_.empty()) {
        librariesDir_ = executableDir() + L"/Lib";
        _wmkdir(librariesDir_.c_str());
        librariesDir_ += L"/site-packages";
        _wmkdir(librariesDir_.c_str());
    }
    return librariesDir_;
}

const std::wstring& Config::pythonStuffDir() {
    if (pythonStuffDir_.empty()) {
        pythonStuffDir_ = executableDir() + L"/../python_stuff";
    }
    return pythonStuffDir_;
}

const std::wstring& Config::pythonMainPy() {
    if (pythonStuffDir_.empty()) {
        pythonMainPy_ = pythonStuffDir() + L"/entrypoint.py";
    }
    return pythonMainPy_;
}

const std::wstring& Config::pyExePath() {
    if (pyExePath_.empty()) {
        pyExePath_ = executableDir() + L"/python.exe";
    }
    return pyExePath_;
}

const std::wstring& Config::modelsRootDir() {
    if (modelsRootDir_.empty()) {
        modelsRootDir_ = executableDir() + L"/../models";
        _wmkdir(modelsRootDir_.c_str());
    }
    return modelsRootDir_;
}

const std::wstring& Config::getConfigDir() {
    if (configDir_.empty()) {
        configDir_ = executableDir() + L"/../config";
        _wmkdir(configDir_.c_str());
    }
    return configDir_;
}

int Config::windowXPos() {
    return screenWidth() / 2 - windowWidth() / 2;
}

int Config::windowYPos() {
    return screenHeight() / 2 - windowHeight() / 2;
}

int Config::windowWidth() {
    return 860;
}

int Config::windowHeight() {
    return 640;
}

int Config::screenWidth() {
    return Fl::w();
}

int Config::screenHeight() {
    return Fl::h();
}

Config &getConfig() {
    static Config cfg;
    return cfg;
}

void Config::setAdditionalModelDir(const std::string& value) {
    additionalModelDir_ = value;
}

void Config::setAdditionalLoraDir(const std::string& value) {
    additionalLoraDir_ = value;
}

std::string Config::getAdditionalModelDir() {
    return additionalModelDir_;
}

std::string Config::getAdditionalLoraDir() {
    return additionalLoraDir_;
}

bool Config::save() {
    try {
        json data;
        json sd;
        sd["add_model_dir"] = additionalModelDir_;
        sd["add_lora_dir"] = additionalLoraDir_;
        sd["max_mem_gpu"] = max_memory_gpu_;
        sd["max_mem_cpu"] = max_memory_cpu_;
        data["autogpt"] = sd;
        json models;
        for (auto it = model_configs_.begin(); it != model_configs_.end(); it++) {
            json data;
            data["max_new_tokens"] = it->second.max_new_tokens;
            data["temperature"] = it->second.temperature;
            data["top_p"] = it->second.top_p;
            data["top_k"] = it->second.top_k;
            data["repetition_penalty"] = it->second.repetition_penalty;
            data["context"] = it->second.context;
            models[it->first] = data;
        }
        data["models"] = models;
        const std::wstring path = getConfigDir() + kCONFIG_FILE;
        std::ofstream f(path.c_str());
        f << std::setw(2) << data << std::endl;
        return true;
    } catch (json::exception& e) {
        fprintf(stderr, "Error savingbottled_ai configuration file: %s\n", e.what());
    }

    return false;
}

bool Config::load() {
    const std::wstring path = getConfigDir() + kCONFIG_FILE;
    std::ifstream f(path.c_str());
    try {
        if (!f.good()) {
            fprintf(stderr, "Bottled-AI's configuration file does not exist\n");
            return true;
        }
        json data = json::parse(f);
        if (data.contains("autogpt")) {
            auto sd = data["autogpt"];
            if (sd.contains("add_model_dir")) {
                additionalModelDir_ = sd["add_model_dir"].get<std::string>();
            }
            if (sd.contains("add_lora_dir")) {
                additionalLoraDir_ = sd["add_lora_dir"].get<std::string>();
            }
            if (sd.contains("max_mem_gpu")) {
                max_memory_gpu_ = sd["max_mem_gpu"].get<int>();
            }
            if (sd.contains("max_mem_cpu")) {
                max_memory_cpu_ = sd["max_mem_cpu"].get<int>();
            }
        }
        if (data.contains("models")) {
            auto models = data["models"];
            for (auto& el : models.items()) {
                auto k = el.key();
                auto v = el.value();
                model_config_t c;
                if (v.contains("max_new_tokens")) {
                    c.max_new_tokens = v["max_new_tokens"].get<int>();
                }
                if (v.contains("temperature")) {
                    c.temperature = v["temperature"].get<float>();
                }
                if (v.contains("top_p")) {
                    c.top_p = v["top_p"].get<float>();
                }
                if (v.contains("top_k")) {
                    c.top_k = v["top_k"].get<float>();
                }
                if (v.contains("repetition_penalty")) {
                    c.repetition_penalty = v["repetition_penalty"].get<float>();
                }
                if (v.contains("context")) {
                    c.context = v["context"].get<std::string>();
                }
                model_configs_[k] = c;
            }
        }
        return true;
    } catch(json::exception& e) {
        fprintf(stderr, "Error loadingbottled_ai configuration file: %s\n", e.what());
    }

    return false;
}

model_config_t Config::getModelConfig(const std::string& model_id) {
    auto it = model_configs_.find(model_id);
    if (it != model_configs_.end()) {
        return it->second;
    }
    model_config_t r;
    return r;
}

void Config::setModelConfig(const std::string& model_id, const model_config_t& config) {
    model_configs_[model_id] = config;
}

void Config::setMaxMemory(int gpu, int cpu) {
    max_memory_cpu_ = cpu;
    max_memory_gpu_ = gpu;
}

void Config::getMaxMemory(int &gpu, int &cpu) {
    cpu = max_memory_cpu_;
    gpu = max_memory_gpu_;
}


}  // namespace bottled_ai
