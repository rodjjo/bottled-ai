#include <sstream>
#ifdef WIN32
#include <Windows.h>
#endif
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

void copy_to_cb(const std::string& text) {
#ifdef WIN32
    const size_t len = strlen(text.c_str()) + 1;
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), text.c_str(), len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
    show_error("the text was copied to the clipboard!");
#endif
}

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
    chat_.reset(new ChatStore());
    auto wnd = this;

    wnd->size_range(860, 480);

    wnd->begin();

    initMenubar();

    models_ = new Fl_Choice(0, 0, 1, 1, "Model:");
    btnModelConf_.reset(new Button(xpm::image(xpm::edit_16x16), [this] {
        configure_model();
    }));

    btnDownload_.reset(new Button(xpm::image(xpm::arrow_down_16x16), [this] {
        download_model();
    }));
    input_ = new Fl_Multiline_Input(0, 0, 1, 1, "Type your prompt (Cltrl+G to send):");
    response_ = new Fl_Help_View(0, 0, 1, 1);
    response_->user_data (this);
    response_->link(link_clicked_cb);

    btnSend_.reset(new Button(xpm::image(xpm::button_ok_16x16), [this] {
        generate_text();
    }));

    wnd->end();
    wnd->resizable(wnd);

    this->show();

    models_->align(FL_ALIGN_LEFT);
    input_->align(FL_ALIGN_TOP_LEFT);
    input_->wrap(1); 
    response_->align(FL_ALIGN_TOP_LEFT);
    btnSend_->tooltip("Send [Ctrl + G]");
    btnModelConf_->tooltip("Configure selected model");
    btnDownload_->tooltip("Download models");

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

    menu_->addItem([this] { clear_all(); }, "", "File/New", "", 0, xpm::file_new_16x16);
    // menu_->addItem([this] {  }, "", "File/Save", "^s", 0, xpm::save_16x16);
    menu_->addItem([this] { Fl::delete_widget(this); }, "", "File/Exit", "", 0, xpm::exit_16x16);
    menu_->addItem([this] { generate_text(); }, "", "Edit/Generate", "^g", 0, xpm::button_play);
    menu_->addItem([this] { editConfig(); }, "", "Edit/Settings", "", 0, xpm::edit_16x16);
    menu_->addItem([this] { showConsoles("Console windows", true); }, "", "Tools/Terminal");
}

void MainWindow::alignComponents() {
    menuPanel_->position(0, 0);
    int w = this->w();
    int h = this->h();
    int stabusbar_h = 30;
    int input_size = 100;
    int label_size = 20;
    menuPanel_->size(w, menu_->h());
    menu_->position(0, 0);
    menu_->size(w, menuPanel_->h());

    models_->position(w / 2 - 160, menuPanel_->h() + 5);
    models_->size(320, 20);

    btnModelConf_->position(models_->x() + models_->w() + 5, models_->y());
    btnModelConf_->size(20, 20);
    btnDownload_->position(btnModelConf_->x() + btnModelConf_->w() + 5, btnModelConf_->y());
    btnDownload_->size(20, 20);

    response_->resize(3, btnDownload_->y() + btnDownload_->h() + 5 + label_size, w - 10, h - label_size * 2 - stabusbar_h - menuPanel_->h() - 15 - input_size);
    input_->resize(3, label_size + response_->h() + response_->y() + 5, w - 35, input_size);
    btnSend_->position(8 + input_->w(), input_->y());
    btnSend_->size(20, 20);
}

void MainWindow::editConfig() {
    show_configuration(); 
}

void MainWindow::resize(int x, int y, int w, int h) {
    Fl_Menu_Window::resize(x, y, w, h);
    alignComponents();
}

void MainWindow::generate_text_cb(void *cbdata) {
    ((MainWindow *) cbdata)->generate_text();
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

void MainWindow::generate_text() {
    std::string value = input_->value();
    if (value.empty()) {
        return;
    }
    int index = models_->value();

    if (index < 0) {
        show_error("No model selected. Download a model first!");
    }
    chat_->executePrompt(
        models[index].id.c_str(),
        value.c_str(),
        true
    );
    response_->value(chat_->toHtml().c_str());
    response_->topline(2147483646);
    response_->topline(response_->topline() - response_->h());
    input_->value("");
}

void MainWindow::download_model() {
    std::string error;
    py::get_py()->execute_callback(
        py::download_model("TheBloke/WizardLM-Uncensored-Falcon-7B-GPTQ", [&error] (bool success, const char *message) {
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
            for (const auto &m : result) {
                if (m.locally) {
                    (*mdls).push_back(m);
                }
            }
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

void MainWindow::configure_model() {
}

const char *MainWindow::link_clicked_cb(Fl_Widget *w, const char *uri) {
    return ((MainWindow *)w->user_data())->link_clicked_cb(uri);
}

const char *MainWindow::link_clicked_cb(const char *uri) {
    int index = -1;
    if (sscanf(uri, "/?a=copy-response-%d", &index) == 1) {
        copy_to_cb(chat_->at(index).raw);
    } else if (sscanf(uri, "/?a=copy-%d", &index) == 1) {
        copy_to_cb(chat_->at(index).prompt + "\n\n" + chat_->at(index).raw);
    } else if (sscanf(uri, "/?a=context-%d", &index) == 1) {
        show_error("Context editor not implemented yet");
    } else if (sscanf(uri, "/?a=regenerate-%d", &index) == 1) {
        int mdl_index = models_->value();
        if (mdl_index < 0) {
            show_error("No model selected. Download a model first!");
            return NULL;
        }
        if (ask("Do you want to generate this response again ?")) {
            int pos = response_->topline();
            chat_->at(index).model_id = models[mdl_index].id;
            chat_->regenerate(index);
            response_->value(chat_->toHtml().c_str());
            response_->topline(pos);
        }
    } else if (sscanf(uri, "/?a=delete-%d", &index) == 1) {
        if (ask("Do you want to remove this entry ?")) {
            chat_->remove(index);
            response_->value(chat_->toHtml().c_str());
        }
    }
    return NULL;
}

void MainWindow::clear_all() {
    if (chat_->count() < 1) {
        return;
    }
    if (ask("Do you want to clear the entire conversation?")) {
        chat_->clear();
        response_->value("");
    }
}

}  // namespace bottled_ai
