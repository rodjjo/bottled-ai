#pragma once

/*
 * Copyright (C) 2023 by Rodrigo Antonio de Araujo
 */
#include <string>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tabs.H>
#include <Fl/Fl_Check_Button.H>
#include <Fl/Fl_Input.H>
#include <Fl/Fl_Choice.H>
#include <Fl/Fl_Float_Input.H>
#include <Fl/Fl_Multiline_Input.H>

#include "src/controls/button.h"
#include "src/windows/modal_window.h"


namespace bottled_ai
{

class ModelParamsWindow {
 public:
    ModelParamsWindow(const std::string& id);
    virtual ~ModelParamsWindow();
    void show();

 private:
    void align_components();
    void load_configuration();
    void save_configuration();
    void default_parameters();

 private:
    std::string model_id_;
    Fl_Group *page_params_;
    Fl_Tabs *tabs_;
    Fl_Input *max_new_tokens_;
    Fl_Float_Input *temperature_;
    Fl_Float_Input *top_p_;
    Fl_Float_Input *top_k_;
    Fl_Float_Input *repetition_penalty_;
    Fl_Multiline_Input *context_;
    std::unique_ptr<Button> restore_;

    ModalWindow *window_ = NULL;
    std::unique_ptr<Button> btnOk_;
    std::unique_ptr<Button> btnCancel_;
};


void configure_model_params(const std::string& id);

}   // namespace bottled_ai

