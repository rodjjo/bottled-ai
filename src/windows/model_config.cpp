#include "src/data/xpm.h"
#include "src/dialogs/common_dialogs.h"
#include "src/python/helpers.h"
#include "src/python/wrapper.h"
#include "src/config/config.h"
#include "src/windows/model_config.h"

namespace bottled_ai {


ModelParamsWindow::ModelParamsWindow(const std::string& id) {
    model_id_ = id;
    window_ = new ModalWindow(0, 0, 250, 465, "");
    window_->copy_label((std::string("Config - ") + id).c_str());
    window_->begin();
    tabs_ = new Fl_Tabs(0, 0, 1, 1);

    tabs_->begin();
    
    page_params_ = new Fl_Group(0, 0, 1, 1, "Parameters");
    page_params_->begin();

    max_new_tokens_ = new Fl_Input(1, 1, 1, 1, "Max new tokens");
    temperature_ = new Fl_Float_Input(1, 1, 1, 1, "Temperature");
    top_p_ = new Fl_Float_Input(1, 1, 1, 1, "Top_P");
    top_k_ = new Fl_Float_Input(1, 1, 1, 1, "Top_K");
    repetition_penalty_ = new Fl_Float_Input(1, 1, 1, 1, "Repetition penalty");
    context_ = new Fl_Multiline_Input(1, 1, 1, 1, "System context:");
    restore_.reset(new Button("Default", [this] {
        default_parameters();
    }));

    page_params_->end();

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

    max_new_tokens_->align(FL_ALIGN_TOP_LEFT);
    temperature_->align(FL_ALIGN_TOP_LEFT);
    top_p_->align(FL_ALIGN_TOP_LEFT);
    top_k_->align(FL_ALIGN_TOP_LEFT);
    repetition_penalty_->align(FL_ALIGN_TOP_LEFT);
    context_->align(FL_ALIGN_TOP_LEFT);

    max_new_tokens_->tooltip("The max number of tokens in the response");
    context_->tooltip("The context initialization");

    align_components();
    load_configuration();
}

ModelParamsWindow::~ModelParamsWindow() {
    Fl::delete_widget(window_);
    Fl::do_widget_deletion();
}

void ModelParamsWindow::align_components() {
    tabs_->resize(0, 0, window_->w(), window_->h() - 50);
    page_params_->resize(tabs_->x(), tabs_->y() + 30, tabs_->w(), tabs_->h() - 22);
    int left = tabs_->x() + 10;
    int top = tabs_->y() + 55;
    int height = 30;
    
    btnOk_->position(window_->w() - 215, window_->h() - 40);
    btnOk_->size(100, 30);
    btnCancel_->position(btnOk_->x() + btnOk_->w() + 2, btnOk_->y());
    btnCancel_->size(100, 30);

    // TAB: parameters
    int spacing_h = height + 15;

    max_new_tokens_->resize(left, top, page_params_->w() - 20, height);
    top += spacing_h;
    temperature_->resize(left, top, page_params_->w() - 20, height);
    top += spacing_h;
    top_p_->resize(left, top, page_params_->w() - 20, height);
    top += spacing_h;
    top_k_->resize(left, top, page_params_->w() - 20, height);
    top += spacing_h;
    repetition_penalty_->resize(left, top, page_params_->w() - 20, height);
    top += spacing_h;
    context_->resize(left, top, page_params_->w() - 20, height * 3);
    top += spacing_h + height * 2;
    restore_->position(page_params_->w() - 120, top);
    restore_->size(100, 20);
}

void ModelParamsWindow::load_configuration() {
    default_parameters();
    auto &c = getConfig();
    model_config_t model_conf = c.getModelConfig(model_id_);
    char buffer[128] = {0,};
    sprintf(buffer, "%d", model_conf.max_new_tokens);
    max_new_tokens_->value(buffer);
    sprintf(buffer, "%0.3f", model_conf.temperature);
    temperature_->value(buffer);
    sprintf(buffer, "%0.3f", model_conf.top_p);
    top_p_->value(buffer);
    sprintf(buffer, "%0.3f", model_conf.top_k);
    top_k_->value(buffer);
    sprintf(buffer, "%0.3f", model_conf.repetition_penalty);
    repetition_penalty_->value(buffer);
    context_->value(model_conf.context.c_str());
}

void ModelParamsWindow::save_configuration() {
    window_->hide();
    auto &c = getConfig();
    model_config_t model_conf = c.getModelConfig(model_id_);

    int value = model_conf.max_new_tokens;
    sscanf(max_new_tokens_->value(), "%d", &value);
    model_conf.max_new_tokens = value;

    float fvalue =  model_conf.temperature;
    sscanf(temperature_->value(), "%f", &fvalue);
    model_conf.temperature = fvalue;
    
    fvalue  = model_conf.top_k;
    sscanf(top_k_->value(), "%f", &fvalue);
    model_conf.top_k = fvalue;

    fvalue  = model_conf.top_p;
    sscanf(top_p_->value(), "%f", &fvalue);
    model_conf.top_p = fvalue;

    fvalue  = model_conf.repetition_penalty;
    sscanf(repetition_penalty_->value(), "%f", &fvalue);
    model_conf.repetition_penalty = fvalue;

    model_conf.context = context_->value();

    c.setModelConfig(model_id_, model_conf);
    c.save();
}

void ModelParamsWindow::show() {
    window_->show();
    while (true) {
        if (!window_->visible_r()) {
            break;
        }
        Fl::wait();
    }
}

void ModelParamsWindow::default_parameters() {
    max_new_tokens_->value("512");
    temperature_->value("1.0");
    top_p_->value("1.0");
    top_k_->value("0.0");
    repetition_penalty_->value("1.0");
    context_->value("You are a helpful AI assistant.");
}

void configure_model_params(const std::string& id) {
    ModelParamsWindow cfg(id);
    cfg.show();
}

}  // namespace bottled_ai
