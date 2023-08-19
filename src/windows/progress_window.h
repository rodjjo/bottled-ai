#pragma once

#include <string>
#include <functional>
#include <memory>

#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Multiline_Output.H>

#include "src/controls/button.h"
#include "src/windows/modal_window.h"

namespace bottled_ai
{

class ProgressWindow {
 public: 
    ProgressWindow();
    virtual ~ProgressWindow();
    void show();
    void hide();

 protected:
    void update();
    static void update(void *cbdata);

 private:
    ModalWindow *window_ = NULL;
    Fl_Progress *progress_;
    Fl_Multiline_Output *preview_;
    Fl_Box* title_;
    std::unique_ptr<Button> btnCancel_;
};

void set_progress_title(const char *title);
void set_progress_text(const char *text);
void set_progress(size_t progress, size_t max);
bool should_cancel_progress();
void enable_progress_window(bool textStream=false);
void show_progress_window();
void hide_progress_window();

} // namespace bottled_ai

