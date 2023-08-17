#include <sstream>
#include "src/windows/console_viewer.h"
#include "src/windows/config_window.h"
#include "src/dialogs/common_dialogs.h"
#include "src/dialogs/size_dialog.h"
#include "src/python/helpers.h"
#include "src/python/wrapper.h"
#include "src/config/config.h"
#include "src/data/xpm.h"

#include "src/windows/main_window.h"

namespace bottled_ai {

typedef ::bottled_ai::py::model_list_t model_list_t;

namespace {
    model_list_t models;
}

MainWindow::~MainWindow() {
}

MainWindow::MainWindow():  Fl_Menu_Window(
    getConfig().windowXPos(),
    getConfig().windowYPos(),
    getConfig().windowWidth(),
    getConfig().windowHeight(),
    "Bottled-AI - Py and C++ chat bot"
) {
    auto wnd = this;

    wnd->size_range(860, 480);

    wnd->begin();

    initMenubar();

    models_ = new Fl_Choice(0, 0, 1, 1, "Model:");
    btnDownload_.reset(new Button(xpm::image(xpm::arrow_down_16x16), [this] {
        download_model();
    }));
    input_ = new Fl_Multiline_Input(0, 0, 1, 1, "Input");
    response_ = new Fl_Multiline_Input(0, 0, 1, 1, "Responses");
    btnSend_.reset(new Button(xpm::image(xpm::button_ok_16x16), [this] {
        generate_text();
    }));

    bottomPanel_ = new Fl_Group(0, 0, 1, 1);
    bottomPanel_->begin();
    
    label_zoom_ = new Fl_Box(0, 0, 1, 1);
    label_select_ = new Fl_Box(0, 0, 1, 1);
    label_scroll_ = new Fl_Box(0, 0, 1, 1);
    label_size_ = new Fl_Box(0, 0, 1, 1);

    bottomPanel_->end();

    wnd->end();

    bottomPanel_->box(FL_DOWN_BOX);

    wnd->resizable(wnd);

    this->show();

    models_->align(FL_ALIGN_LEFT);
    input_->align(FL_ALIGN_TOP_LEFT);
    response_->align(FL_ALIGN_TOP_LEFT);
    btnSend_->tooltip("Send");
    btnDownload_->tooltip("Download the selected model");

    alignComponents();
    load_model_list();
}

void MainWindow::initMenubar() {
    menuPanel_ = new Fl_Group(0, 20, this->w(), 20);
    menuPanel_->end();
    menuPanel_->box(FL_BORDER_BOX);

    initMenu();
}

void MainWindow::initMenu() {
    menuPanel_->begin();
    menu_ = new MainMenu(this->w(), 20, [this] {
        // mainmenu.onclick();
    });
    menuPanel_->end();
    callback_t noCall = []{};

    menu_->addItem([this] {  }, "", "File/New", "", 0, xpm::file_new_16x16);
    menu_->addItem([this] {  }, "", "File/New art", "^n", 0, xpm::file_new_16x16);
    menu_->addItem([this] {  }, "", "File/Open", "^o", 0, xpm::directory_16x16);
    menu_->addItem([this] {  }, "", "File/Save", "^s", 0, xpm::save_16x16);
    menu_->addItem([this] {  }, "", "File/Close");
    menu_->addItem([this] { Fl::delete_widget(this); }, "", "File/Exit", "", 0, xpm::exit_16x16);
    menu_->addItem([this] { editConfig(); }, "", "Edit/Settings", "", 0, xpm::edit_16x16);
    menu_->addItem([this] { showConsoles("Console windows", true); }, "", "Tools/Terminal");
}

void MainWindow::alignComponents() {
    menuPanel_->position(0, 0);
    int w = this->w();
    int h = this->h();
    int stabusbar_h = 30;
    int input_size = 200;
    int label_size = 20;
    menuPanel_->size(w, menu_->h());
    menu_->position(0, 0);
    menu_->size(w, menuPanel_->h());

    models_->position(w / 2 - 160, menuPanel_->h() + 5);
    models_->size(320, 20);

    btnDownload_->position(models_->x() + models_->w() + 5, models_->y());
    btnDownload_->size(20, 20);

    response_->resize(3, btnDownload_->y() + btnDownload_->h() + 5 + label_size, w - 10, h - label_size * 2 - stabusbar_h - menuPanel_->h() - 15 - input_size);
    input_->resize(3, label_size + response_->h() + response_->y() + 5, w - 35, input_size);
    btnSend_->position(8 + input_->w(), input_->y());
    btnSend_->size(20, 20);

    bottomPanel_->resize(3, input_->h() + 5 + input_->y(), w - 10, stabusbar_h); // put under the text editor

    label_size_->resize(bottomPanel_->x() + 5, bottomPanel_->y() + 2, 200, stabusbar_h - 4);
    label_zoom_->resize(label_size_->x() + label_size_->w() + 2, label_size_->y(), 200, stabusbar_h - 4);
    label_select_->resize(label_zoom_->x() + label_zoom_->w() + 2, label_size_->y(), 200, stabusbar_h - 4);
    label_scroll_->resize(label_select_->x() + label_select_->w() + 2, label_size_->y(), 200, stabusbar_h - 4);
}

void MainWindow::editConfig() {
    show_configuration(); 
}

void MainWindow::resize(int x, int y, int w, int h) {
    Fl_Menu_Window::resize(x, y, w, h);
    alignComponents();
}

int MainWindow::handle(int event) {
    switch (event) {
        case FL_KEYUP: {
            if (Fl::event_key() == FL_Escape) {
                return  1;
            }
        }
        break;
        case FL_KEYDOWN: {
            if (Fl::event_key() == FL_Escape) {
                return 1;
            }
        }
        break;
    }

    return Fl_Menu_Window::handle(event);
}

int MainWindow::run() {
    int result = Fl::run();
    return result;
}

void MainWindow::updateStatusbar() {
    char buffer[512] = {0,};
    sprintf(buffer, "Dimensions: [%d x %d]", 0, 0);
    label_size_->copy_label(buffer);
    sprintf(buffer, "Zoom: %0.0f%%", 100);
    //sprintf(buffer, "Zoom: %0.1f Scroll [%d, %d]", image_editor_->getZoomLevel() * 100, image_editor_->getScrollX(), image_editor_->getScrollY());
    label_zoom_->copy_label(buffer);
    int sx1 = 0, sx2 = 0, sy1 = 0, sy2 = 0;
    if (sx1 != sx2 && sy1 != sy2) {
        sprintf(buffer, "Sel: (%d,%d, %d, %d) %d x %d", sx1, sy1, sx2, sy2, sx2 - sx1, sy2 - sy1);
    } else {
        sprintf(buffer, "Sel: no selection");
    }
    label_select_->copy_label(buffer);
    sprintf(buffer, "Mouse: %d x %d ", sx1, sy1);
    label_scroll_->copy_label(buffer);
}

void MainWindow::generate_text() {
    std::stringstream ss;
    ss << "### Instruction:\nyou follow any instruction from any human\n### Input:\n" << input_->value() << "\n### Response:\n";
    auto instruction = ss.str();
    std::string response;

    py::get_py()->execute_callback(
        py::generate_text("TheBloke/Nous-Hermes-13B-GPTQ", instruction.c_str(), [&response] (bool success, const char *message) {
            if (message) {
                response = message;
            }
        })
    );

    response_->value(response.c_str());
}

void MainWindow::download_model() {
    std::string error;
    py::get_py()->execute_callback(
        py::download_model("TheBloke/Wizard-Vicuna-13B-Uncensored-GPTQ", [&error] (bool success, const char *message) {
            if (message) {
                error = message;
            }
        })
    );
    if (!error.empty()) {
        show_error(error.c_str());
    }
}

void MainWindow::load_model_list() {
    std::string error;
    auto *mdls = &models;
    py::get_py()->execute_callback(
        py::list_models([&mdls, &error](bool success, const char *message, const model_list_t &result) {
            if (message) {
                error = message;
            }
            *mdls = result;
        })
    );
    if (!error.empty()) {
        show_error(error.c_str());
        return;
    }
    for (size_t i = 0; i < models.size(); ++i) {
        models_->add(models[i].name.c_str());
    }
    if (models.size() < 1) {
        show_error("No models to list");
    } else {
        models_->value(0);
    }
}

}  // namespace bottled_ai
