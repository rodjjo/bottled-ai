#include <string>
#include <sstream>

#include "src/dialogs/common_dialogs.h"

#include "src/python/helpers.h"
#include "src/python/wrapper.h"

#include "src/windows/downloader.h"

namespace bottled_ai
{

typedef ::bottled_ai::py::model_list_t model_list_t;

std::string model2html(const py::model_t& model) {
    std::stringstream ss;
    ss << "<p><font face=\"arial\" size=\"6\"><b>";
    ss << "Model Name:";
    ss << "</b></font></p>";
    ss << "<p><font face=\"arial\" size=\"4\">";
    ss << escape_html(model.name);
    ss << "</font></p>";
    ss << "<p><font face=\"arial\" size=\"6\"><b>";
    ss << "Description:";
    ss << "</b></font></p>";
    ss << "<p><font face=\"arial\" size=\"4\">";
    ss << escape_html(model.description);
    ss << "</font></p>";
    ss << "<center>";
    if (model.locally) {
        ss << "<a href=\"/?remove-";
    } else {
        ss << "<a href=\"/?download-";
    }
    ss << escape_html(model.id) << "\">";
    if (model.locally) {
        ss << "[REMOVE THIS MODEL]";
    } else {
        ss << "[DOWNLOAD THIS MODEL]";
    }
    ss << "</a></center>";
    return ss.str();
}

std::string model_list_2_htm(const model_list_t & models, bool option_locally) {
    bool first = true;
    std::stringstream ss;
    for (const auto & m : models) {
        if (m.locally != option_locally) {
            continue;
        }
        if (first) {
            first = false;
        } else {
            ss << "<hr>";
        }
        ss << model2html(m);
    }
    if (ss.str().empty()) {
        ss << "<center><p><font face=\"arial\" size=\"6\"><b>";
        ss << "No models to show in this page.";
        ss << "</b></font></p></center>";
    }
    return ss.str();
}

DownloaderWindow::DownloaderWindow() {
    window_ = new ModalWindow(0, 0, 840, 650, "Manage local model");
    window_->begin();
    tabs_ = new Fl_Tabs(0, 0, 1, 1);
    tabs_->begin();
    
    page_availiable_ = new Fl_Group(0, 0, 1, 1, "A list of models to download");
    page_availiable_->begin();
    availiable_ = new Fl_Help_View(0, 0, 1, 1);
    page_availiable_->end();

    page_have_ = new Fl_Group(0, 0, 1, 1, "A list of models already downloaded");
    page_have_->begin();
    have_= new Fl_Help_View(0, 0, 1, 1);
    page_have_->end();

    tabs_->end();
    
    btnOk_.reset(new Button("Close", [this] {
        window_->hide();
    }));

    window_->end();

    have_->user_data(this);
    availiable_->user_data(this);
    have_->link(link_clicked_cb);
    availiable_->link(link_clicked_cb);

    btnOk_->tooltip("Close the window");
    
    window_->position(Fl::w() / 2 - window_->w() / 2, Fl::h() / 2 - window_->h() / 2);

    alignComponents();
    loadModels();
}

DownloaderWindow::~DownloaderWindow() {
    Fl::delete_widget(window_);
    Fl::do_widget_deletion();
}

void DownloaderWindow::alignComponents() {
    ((Fl_Group*) tabs_)->resize(0, 0, window_->w(), window_->h() - 50);
    page_availiable_->resize(tabs_->x(), tabs_->y() + 30, tabs_->w(), tabs_->h() - 22);
    page_have_->resize(tabs_->x(), tabs_->y() + 30, tabs_->w(), tabs_->h() - 22);
    int left = tabs_->x() + 10;
    int top = tabs_->y() + 55;
    int height = 30;

    btnOk_->position(window_->w() - 107, window_->h() - 40);
    btnOk_->size(100, 30);

    height = window_->h() - top - btnOk_->h() - 30;
    availiable_->resize(left, top, page_availiable_->w() - 20, height);
    have_->resize(left, top, availiable_->w(), availiable_->h());
}

void DownloaderWindow::loadModels() {
    std::string error;
    model_list_t  models;
    py::get_py()->execute_callback(
        py::list_models([&models, &error](bool success, const char *message, const model_list_t &result) {
            if (message) {
                error = message;
            }
            for (const auto &m : result) {
                models.push_back(m);
            }
        })
    );
    if (!error.empty()) {
        show_error(error.c_str());
        return;
    }
    have_->value(model_list_2_htm(models, true).c_str());
    availiable_->value(model_list_2_htm(models, false).c_str());
}

void DownloaderWindow::show() {
    window_->show();
    while (true) {
        if (!window_->visible_r()) {
            break;
        }
        Fl::wait();
    }
}

const char *DownloaderWindow::link_clicked_cb(Fl_Widget *w, const char *uri) {
    auto dwn = (DownloaderWindow *) w->user_data();
    dwn->link_clicked_cb(uri, dwn->have_ != w);
    return NULL;
}

void DownloaderWindow::link_clicked_cb(const char *uri, bool for_downloading) {
    const char *id = strstr(uri, "-");
    if (id == NULL) {
        return;
    }
    id++;
    if (for_downloading) {
        download_model(id);
    } else {
        remove_model(id);
    }
}

void DownloaderWindow::remove_model(const std::string& model_id) {
    if (ask("Irreversible operation.\nDo you want to remove the model files?")) {
        std::string error;
        py::get_py()->execute_callback(
            py::remove_model(model_id.c_str(), [&error] (bool success, const char *message) {
                if (message) {
                    error = message;
                }
            })
        );
        if (!error.empty()) {
            show_error(error.c_str());
        } else {
            loadModels();
        }
    }
}

void DownloaderWindow::download_model(const std::string& model_id) {
    std::string error;
    py::get_py()->execute_callback(
        py::download_model(model_id.c_str(), [&error] (bool success, const char *message) {
            if (message) {
                error = message;
            }
        })
    );
    if (!error.empty()) {
        show_error(error.c_str());
    } else {
        loadModels();
    }
}

void show_downloader_window() {
    DownloaderWindow cfg;
    cfg.show();
}


} // namespace bottled_ai
