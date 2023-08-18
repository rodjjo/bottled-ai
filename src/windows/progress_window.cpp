#include <memory>
#include <mutex>
#include <string>
#include <atomic>
#include "src/data/xpm.h"
#include "src/config/config.h"
#include "src/windows/progress_window.h"


namespace bottled_ai
{
namespace {
    bool progress_enabled = false;
    bool progress_canceled = false;
    bool textStream_enabled = false;
    std::unique_ptr<ProgressWindow> prog_win;
    std::string progress_label;
    std::string progress_text;

    std::mutex mtx_progress;
    float current_progress = 0;
    float max_progress = 100;
}

ProgressWindow::ProgressWindow() {
    window_ = new ModalWindow(0, 0, 640 ,  textStream_enabled ? 310 : 90, "Please wait");
    window_->begin();
    title_ = new Fl_Box(5, window_->h() - 85 + 3, window_->w() - 10, 20, "Wait");
    progress_ = new Fl_Progress(5, title_->y() + title_->h() + 3, window_->w() - 10, 15);
    preview_ = new Fl_Multiline_Output(5, 5, window_->w() - 10, 200);

    btnCancel_.reset(new Button(xpm::image(xpm::button_cancel_16x16), [this] {
        progress_canceled = true;
        btnCancel_->enabled(false);
    }));

    window_->end();
    window_->position(Fl::w() / 2 - window_->w() / 2, Fl::h() / 2 - window_->h() / 2);

    progress_->maximum(100);
    progress_->value(0);

    //preview_->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    preview_->wrap(1); 
    if (textStream_enabled) {
        progress_->hide();
    } else {
        preview_->hide();
    }

    btnCancel_->tooltip("Cancel the operation");
    btnCancel_->position(window_->w() / 2 - 50, window_->h() - 40);
    btnCancel_->size(100, 30);

    Fl::add_timeout(0.01, &ProgressWindow::update, this);
}

ProgressWindow::~ProgressWindow() {
    Fl::delete_widget(window_);
    Fl::do_widget_deletion();
}
 
void ProgressWindow::show() {
    window_->show();
}

void ProgressWindow::hide() {
    window_->hide();
}

void ProgressWindow::update(void *cbdata) {
    if (prog_win.get() == cbdata) {
        prog_win->update();
        Fl::repeat_timeout(0.33, ProgressWindow::update, cbdata); // retrigger timeout
    }
}

void ProgressWindow::update() {
    std::unique_lock<std::mutex> lk(mtx_progress);
    progress_->maximum(max_progress);
    progress_->value(current_progress);
    if (!progress_label.empty()) {
        title_->copy_label(progress_label.c_str());
        progress_label.clear();
    }

    if (textStream_enabled && !progress_text.empty()) {
        std::string current = preview_->value();
        current += progress_text;
        preview_->value(current.c_str());
        preview_->position(preview_->size());
        progress_text.clear();
    }
}

void set_progress_title(const char *title) {
    std::unique_lock<std::mutex> lk(mtx_progress);
    if (title)
        progress_label = title;
}

void set_progress_text(const char *text) {
    std::unique_lock<std::mutex> lk(mtx_progress);
    if (text) 
        progress_text += text;
}

void set_progress(size_t progress, size_t max) {
    std::unique_lock<std::mutex> lk(mtx_progress);
    if (max < 1) {
        max = 100;
    }
    max_progress = 100.0;
    current_progress = (100.0 / max) * progress;
}

bool should_cancel_progress() {
    return progress_canceled;
}

void enable_progress_window(bool textStream) {
    progress_enabled = true;
    progress_canceled = false;
    textStream_enabled = textStream;
    progress_text.clear();
 }

void show_progress_window() {
    if (prog_win) 
        return;
    if (!progress_enabled)
        return;
    progress_text.clear();
    current_progress = 0;
    max_progress = 100;
    prog_win.reset(new ProgressWindow());
    prog_win->show();
    progress_enabled = false;
}

void hide_progress_window() {
    if (prog_win) {
        prog_win->hide();
        prog_win.reset();
    }
}


} // namespace bottled_ai
