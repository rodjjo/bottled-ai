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
        data["autogpt"] = sd;
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
            fprintf(stderr, "Diffusion Expert's configuration file does not exist\n");
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
        }
        return true;
    } catch(json::exception& e) {
        fprintf(stderr, "Error loadingbottled_ai configuration file: %s\n", e.what());
    }

    return false;
}


}  // namespace bottled_ai
