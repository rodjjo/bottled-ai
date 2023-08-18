#include "src/data/xpm.h"
#include "src/dialogs/common_dialogs.h"
#include "src/python/helpers.h"
#include "src/python/wrapper.h"
#include "src/config/config.h"
#include "src/windows/config_window.h"

namespace bottled_ai {


ConfigWindow::ConfigWindow() {
    window_ = new ModalWindow(0, 0, 640, 480, "Configurations");
    window_->begin();
    tabs_ = new Fl_Tabs(0, 0, 1, 1);

    tabs_->begin();
    
    page_dirs_ = new Fl_Group(0, 0, 1, 1, "Directories");
    page_dirs_->begin();
    add_model_dir_ = new Fl_Input(1, 1, 1, 1, "Additional model dir");
    add_lora_dir_ = new Fl_Input(1, 1, 1, 1, "Additional lora dir");
    page_dirs_->end();

    tabs_->end();

    btnOk_.reset(new Button(xpm::image(xpm::button_ok_16x16), [this] {
        save_configuration();
    }));
    btnCancel_.reset(new Button(xpm::image(xpm::button_cancel_16x16), [this] {
        window_->hide();        
    }));
    window_->end();

    window_->enableExitShortcuts();

    btnOk_->tooltip("Save the configuration");
    btnCancel_->tooltip("Save the configuration");
    window_->position(Fl::w() / 2 - window_->w() / 2, Fl::h() / 2 - window_->h() / 2);

    add_model_dir_->align(FL_ALIGN_TOP_LEFT);
    add_lora_dir_->align(FL_ALIGN_TOP_LEFT);

    add_model_dir_->tooltip("The additional directory for stable diffusion models");
    add_lora_dir_->tooltip("The additional directory for lora model");

    align_components();
    load_configuration();
}

ConfigWindow::~ConfigWindow() {
    Fl::delete_widget(window_);
    Fl::do_widget_deletion();
}

void ConfigWindow::align_components() {
    tabs_->resize(0, 0, window_->w(), window_->h() - 50);
    page_dirs_->resize(tabs_->x(), tabs_->y() + 30, tabs_->w(), tabs_->h() - 22);
    int left = tabs_->x() + 10;
    int top = tabs_->y() + 55;
    int height = 30;
    
    btnOk_->position(window_->w() - 215, window_->h() - 40);
    btnOk_->size(100, 30);
    btnCancel_->position(btnOk_->x() + btnOk_->w() + 2, btnOk_->y());
    btnCancel_->size(100, 30);

    // TAB: directories
    add_model_dir_->resize(left, top, page_dirs_->w() - 20, height);
    add_lora_dir_->resize(left, add_model_dir_->y() + add_model_dir_->h() + 20, page_dirs_->w() - 20, height);
}

void ConfigWindow::load_configuration() {
    auto &c = getConfig();
    add_lora_dir_->value(c.getAdditionalLoraDir().c_str());
    add_model_dir_->value(c.getAdditionalModelDir().c_str());
}

void ConfigWindow::save_configuration() {
    window_->hide();
    auto &c = getConfig();

    c.setAdditionalModelDir(add_model_dir_->value());
    c.setAdditionalLoraDir(add_lora_dir_->value());
    c.save();
}

void ConfigWindow::show() {
    window_->show();
    while (true) {
        if (!window_->visible_r()) {
            break;
        }
        Fl::wait();
    }
}

void show_configuration() {
    ConfigWindow cfg;
    cfg.show();
}

}  // namespace bottled_ai
