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
    
    page_general_ = new Fl_Group(0, 0, 1, 1, "General");
    page_general_->begin();
    add_model_dir_ = new Fl_Input(1, 1, 1, 1, "Additional model dir");
    add_lora_dir_ = new Fl_Input(1, 1, 1, 1, "Additional lora dir");
    max_memory_gpu_ = new Fl_Input(1, 1, 1, 1, "Max gpu memory (MB)");
    max_memory_cpu_ = new Fl_Input(1, 1, 1, 1, "Max cpu memory (MB)");

    page_general_->end();

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
    max_memory_gpu_->align(FL_ALIGN_TOP_LEFT);
    max_memory_cpu_->align(FL_ALIGN_TOP_LEFT);

    add_model_dir_->tooltip("The additional directory for stable diffusion models");
    add_lora_dir_->tooltip("The additional directory for lora model");
    max_memory_gpu_->tooltip("Limit GPU memory");
    max_memory_cpu_->tooltip("Limit CPU memory");

    align_components();
    load_configuration();
}

ConfigWindow::~ConfigWindow() {
    Fl::delete_widget(window_);
    Fl::do_widget_deletion();
}

void ConfigWindow::align_components() {
    ((Fl_Group*) tabs_)->resize(0, 0, window_->w(), window_->h() - 50);
    page_general_->resize(tabs_->x(), tabs_->y() + 30, tabs_->w(), tabs_->h() - 22);
    int left = tabs_->x() + 10;
    int top = tabs_->y() + 55;
    int height = 30;
    
    btnOk_->position(window_->w() - 215, window_->h() - 40);
    btnOk_->size(100, 30);
    btnCancel_->position(btnOk_->x() + btnOk_->w() + 2, btnOk_->y());
    btnCancel_->size(100, 30);

    // TAB: directories
    add_model_dir_->resize(left, top, page_general_->w() - 20, height);
    add_lora_dir_->resize(left, add_model_dir_->y() + add_model_dir_->h() + 20, page_general_->w() - 20, height);
    max_memory_gpu_->resize(left, add_lora_dir_->y() + add_lora_dir_->h() + 20, 160, height);
    max_memory_cpu_->resize(left, max_memory_gpu_->y() + max_memory_gpu_->h() + 20, 160, height);
}

void ConfigWindow::load_configuration() {
    auto &c = getConfig();
    add_lora_dir_->value(c.getAdditionalLoraDir().c_str());
    add_model_dir_->value(c.getAdditionalModelDir().c_str());
    char buffer[128] = {0,};
    int cpu = -1, gpu = -1;
    c.getMaxMemory(gpu, cpu);
    sprintf(buffer, "%d", gpu);
    max_memory_gpu_->value(buffer);
    sprintf(buffer, "%d", cpu);
    max_memory_cpu_->value(buffer);
}

void ConfigWindow::save_configuration() {
    window_->hide();
    auto &c = getConfig();
    c.setAdditionalModelDir(add_model_dir_->value());
    c.setAdditionalLoraDir(add_lora_dir_->value());
    int cpu = -1, gpu = -1;
    sscanf(max_memory_gpu_->value(), "%d", &gpu);
    sscanf(max_memory_cpu_->value(), "%d", &cpu);
    c.setMaxMemory(gpu, cpu);
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
