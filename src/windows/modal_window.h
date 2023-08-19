#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

namespace bottled_ai {
    
class ModalWindow : public Fl_Window {
 public:
  ModalWindow(int xx, int yy, int ww, int hh, const char* title );
  virtual ~ModalWindow();
  void enableExitShortcuts();

 protected:
  int handle(int event) override;

 private:
  bool exit_shortcuts_ = false;
};

}  // namespace bottled_ai
