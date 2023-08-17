/*
 * Copyright (C) 2023 by Rodrigo Antonio de Araujo
 */
#ifndef SRC_WINDOWS_MAIN_WINDOW_H_
#define SRC_WINDOWS_MAIN_WINDOW_H_

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

#include "src/windows/main_menu.h"
#include "src/python/wrapper.h"
#include "src/controls/button.h"

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
  void initMenubar();
  void initMenu();
  void alignComponents();
  void editConfig();
  void updateStatusbar();
  void generate_text();
  void download_model();
  void load_model_list();
  
 private:
    Fl_Group *menuPanel_ = NULL;
    Fl_Group *bottomPanel_ = NULL;
    Fl_Box * label_zoom_ = NULL;
    Fl_Box * label_select_ = NULL;
    Fl_Box * label_scroll_ = NULL;
    Fl_Box * label_size_ = NULL;
    Fl_Multiline_Input *input_ = NULL;
    Fl_Multiline_Input *response_ = NULL;
    Fl_Choice *models_;
    std::unique_ptr<Button> btnDownload_;
    std::unique_ptr<Button> btnSend_;
    MainMenu *menu_ = NULL;
};


} // namespace bottled_ai

#endif  // SRC_WINDOWS_MAIN_WINDOW_H_