#pragma once

#include <FL/Fl_Group.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Help_View.H>

#include "src/controls/button.h"
#include "src/windows/modal_window.h"

namespace bottled_ai
{

class DownloaderWindow {
 public:
    DownloaderWindow();
    virtual ~DownloaderWindow();
    void show();
 private:
   static const char *link_clicked_cb(Fl_Widget *w, const char *uri);
   void link_clicked_cb(const char *uri, bool for_downloading);

    void alignComponents();
    void loadModels();
    void download_model(const std::string& model_id);
    void remove_model(const std::string& model_id);
    
 private:
    Fl_Group *page_have_;
    Fl_Group *page_availiable_;
    Fl_Tabs *tabs_;
    Fl_Help_View *have_ = NULL;
    Fl_Help_View *availiable_ = NULL;
    ModalWindow *window_ = NULL;
    std::unique_ptr<Button> btnOk_;
};

void show_downloader_window();

} // namespace bottled_ai
