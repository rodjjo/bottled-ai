/*
 * Copyright (C) 2023 by Rodrigo Antonio de Araujo
 */
#ifndef SRC_WINDOWS_CONFIG_WINDOW_H
#define SRC_WINDOWS_CONFIG_WINDOW_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Tabs.H>
#include <Fl/Fl_Check_Button.H>
#include <Fl/Fl_Input.H>
#include <Fl/Fl_Choice.H>
#include <Fl/Fl_Float_Input.H>

#include "src/controls/button.h"
#include "src/windows/modal_window.h"

namespace bottled_ai {

typedef enum {
    controlnet_0,
    controlnet_1,
    controlnet_2,
    controlnet_3,
    controlnet_4,
    // keep controlnet_max at the end
    controlnet_max
} controlnet_counter_t;

typedef enum {
   gfpgan_arch_clean,
   gfpgan_arch_restore_former,
   // keep gfpgan_arch_count at the end
   gfpgan_arch_count 
} gfpgan_arch_t;

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
    Fl_Group *page_dirs_;
    Fl_Tabs *tabs_;
    Fl_Input *add_model_dir_;
    Fl_Input *add_lora_dir_;

    ModalWindow *window_ = NULL;
    std::unique_ptr<Button> btnOk_;
    std::unique_ptr<Button> btnCancel_;
};


void show_configuration();

}   // namespace bottled_ai

#endif  // SRC_WINDOWS_CONFIG_WINDOW_H
