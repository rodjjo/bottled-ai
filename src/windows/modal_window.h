#ifndef SRC_WINDOWS_MODAL_WINDOW_H
#define SRC_WINDOWS_MODAL_WINDOW_H

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

#endif  // SRC_WINDOWS_MODAL_WINDOW_H
