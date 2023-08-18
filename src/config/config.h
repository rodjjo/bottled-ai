/*
 * Copyright (C) 2023 by Rodrigo Antonio de Araujo
 */
#ifndef SRC_CONFIG_CONFIG_H_
#define SRC_CONFIG_CONFIG_H_

#include <string>

namespace bottled_ai
{

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

    bool save();
    bool load();
    
  private:
    // string buffers
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