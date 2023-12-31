#pragma once

#include <memory>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include "src/console/emulated_terminal.h"

namespace bottled_ai {

class Console: public Fl_Widget { 
 public:
    Console(int X,int Y,int W,int H, std::shared_ptr<EmulatedTerminal> terminal);
    virtual ~Console();

    void textfont(Fl_Font fontface);
    void textsize(int fontsize);
    int getFontWidth();
    int getFontFace();
    int getFontSize();
    int getFontHeight();

 protected:
    void draw() override;

 private:
    static void validate(void *ud);

 private:
    int version_;
    std::shared_ptr<EmulatedTerminal> terminal_;
    int font_width = 10;
    int font_height = 16;
    int font_size;		//current font size, should equal to height
    int font_face;		//current font face
    
};

}  // namespace bottled_ai 

