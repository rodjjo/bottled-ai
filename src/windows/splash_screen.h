#ifndef SRC_WINDOWS_SPLASH_SCREEN_H
#define SRC_WINDOWS_SPLASH_SCREEN_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

namespace bottled_ai
{
    
class SplashScreen : public Fl_Window {
    public:
        SplashScreen();
        virtual ~SplashScreen();
        void open();
        void close();
    private:
        Fl_Box *loading_label_;
};

void wait_python();

} // namespace bottled_ai

#endif  // SRC_WINDOWS_SPLASH_SCREEN_H
