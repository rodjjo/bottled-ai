/*
 * Copyright (C) 2023 by Rodrigo Antonio de Araujo
 */
#pragma once

#include <memory>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Help_View.H>

#include "src/windows/main_menu.h"
#include "src/python/wrapper.h"
#include "src/controls/button.h"
#include "src/data/chat_store.h"

namespace bottled_ai {

class MainWindow : Fl_Menu_Window {
 public:
    MainWindow();
    ~MainWindow();
    int run();

 protected:
   void resize(int x, int y, int w, int h) override;
   int handle(int event) override;

 private:
  static void generate_text_cb(void *cbdata);
  static const char *link_clicked_cb(Fl_Widget *w, const char *uri);
  const char *link_clicked_cb(const char *uri);
  void initMenubar();
  void initMenu();
  void alignComponents();
  void editConfig();
  void generate_text();
  void download_model();
  void load_model_list();
  void configure_model();
  void clear_all();
  
 private:
    Fl_Group *menuPanel_ = NULL;
    Fl_Multiline_Input *input_ = NULL;
    Fl_Help_View *response_ = NULL;
    Fl_Choice *models_;
    std::unique_ptr<Button> btnDownload_;
    std::unique_ptr<Button> btnModelConf_;
    std::unique_ptr<Button> btnSend_;
    std::unique_ptr<ChatStore> chat_;
    MainMenu *menu_ = NULL;
};


} // namespace bottled_ai
