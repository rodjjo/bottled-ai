/*
 * Copyright (C) 2023 by Rodrigo Antonio de Araujo
 */
#ifndef SRC_CONFIG_CONFIG_H_
#define SRC_CONFIG_CONFIG_H_

#include <string>
#include <map>

namespace bottled_ai
{

  typedef struct {
    int max_new_tokens = 512;
    float temperature = 1;
    float top_p = 1;
    float top_k = 1;
    float repetition_penalty = 1;
    std::string context = "You are a helpful AI assistant.";
  } model_config_t;

  class Config
  {
  public:
    Config();
    ~Config();

    const std::wstring &executableDir();
    const std::wstring &librariesDir();
    const std::wstring &pyExePath();
    const std::wstring &pythonStuffDir();
    const std::wstring &pythonMainPy();
    const std::wstring &modelsRootDir();
    const std::wstring &sdModelsDir();
    const std::wstring &getConfigDir();

    int screenWidth();
    int screenHeight();

    int windowXPos();
    int windowYPos();
    int windowWidth();
    int windowHeight();

    void setAdditionalModelDir(const std::string& value);
    void setAdditionalLoraDir(const std::string& value);
    std::string getAdditionalModelDir();
    std::string getAdditionalLoraDir();
    model_config_t getModelConfig(const std::string& model_id);
    void setModelConfig(const std::string& model_id, const model_config_t& config);

    void setMaxMemory(int gpu, int cpu);
    void getMaxMemory(int &gpu, int &cpu);

    bool save();
    bool load();
    
  private:
    // string buffers
    std::map<std::string, model_config_t> model_configs_;
    int max_memory_cpu_ = -1;
    int max_memory_gpu_ = -1;
    std::wstring configDir_;
    std::wstring librariesDir_;
    std::wstring executableDir_;
    std::wstring pythonStuffDir_;
    std::wstring pyExePath_;
    std::wstring pythonMainPy_;
    std::wstring modelsRootDir_;
    std::string additionalModelDir_;
    std::string additionalLoraDir_;

  private:
  };

  Config &getConfig();

} // namespace bottled_ai

#endif