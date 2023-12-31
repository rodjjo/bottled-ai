#pragma once

#include <memory>

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tabs.H>

#include "src/console/console.h"
#include "src/controls/button.h"


namespace bottled_ai
{

class ConsoleTabs : public Fl_Group {
 public:
  ConsoleTabs(int x, int y, int w, int h);
  ~ConsoleTabs();
  void hideShowConsoleButtons();
  void goStdout();
  void goStderr();
  
 protected:
   void resize(int x, int y, int w, int h) override;
 private:
   void align_components();

 private:
   Fl_Tabs   *tabs_ = NULL;
   Fl_Group  *tabStdout_   = NULL;
   Fl_Group  *tabStderr_   = NULL;
   std::unique_ptr<Button> btnCopyStdout_;
   std::unique_ptr<Button> btnCopyStderr_;
   std::unique_ptr<Button> btnSeeStdout_;
   std::unique_ptr<Button> btnSeeStderr_;
   Console   *stdout_ = NULL;
   Console   *stderr_ = NULL;
};
    
} // namespace bottled_ai

