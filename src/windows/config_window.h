/*
 * Copyright (C) 2023 by Rodrigo Antonio de Araujo
 */
#pragma once

#include <FL/Fl_Group.H>
#include <FL/Fl_Tabs.H>
#include <Fl/Fl_Check_Button.H>
#include <Fl/Fl_Input.H>
#include <Fl/Fl_Choice.H>
#include <Fl/Fl_Float_Input.H>

#include "src/controls/button.h"
#include "src/windows/modal_window.h"

namespace bottled_ai {

class ConfigWindow {
 public:
    ConfigWindow();
    virtual ~ConfigWindow();
    void show();

 private:
    void align_components();
    void load_configuration();
    void save_configuration();

 private:
    Fl_Group *page_general_;
    Fl_Tabs *tabs_;
    Fl_Input *add_model_dir_;
    Fl_Input *add_lora_dir_;
    Fl_Input *max_memory_gpu_;
    Fl_Input *max_memory_cpu_;

    ModalWindow *window_ = NULL;
    std::unique_ptr<Button> btnOk_;
    std::unique_ptr<Button> btnCancel_;
};


void show_configuration();

}   // namespace bottled_ai
