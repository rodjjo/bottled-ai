#include <iostream>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "src/python/wrapper.h"
#include "src/python/helpers.h"
#include "src/dialogs/common_dialogs.h"
#include "src/windows/console_viewer.h"
#include "src/windows/main_window.h"
#include "src/windows/splash_screen.h"


bool install_deps() {
    bool success = false;
    std::string message = "Unexpected error. Callback did not run!";
    bottled_ai::runPyShowConsole("Installing Python dependencies", [&success, &message] {
        auto installer = bottled_ai::py::install_deps([&success, &message] (bool st, const char *msg) {
            success = st;
            if (msg) {
                message = msg;
            }
        });
        installer();
    }, [&success, &message] () -> bool {
        if (!success) {
            bottled_ai::show_error(message.c_str());
        }
        return success;
    });

    return success;
}


int main(int argc, char **argv)
{
    int result = 0;
    /*
        It runs the gui in a secodary thread to let python to use the main thread.
        Python should use the main thread to avoid the app to hang unloading some libraries (ex. PyTorch).
    */
    std::thread gui_thread([&result] {
        Fl::scheme("gtk+");
        /*
        for (int i = 0; i < 100; i++) {
            bottled_ai::test_generators();
        }
        */

        bool have_deeps = false;
        const char *msg = NULL;
        bottled_ai::py::get_py()->execute_callback(
            bottled_ai::py::check_have_deps([&have_deeps, &msg] (bool status, const char *error) {
                msg = error;
                have_deeps = status;
            })
        );

        if (msg) {
            bottled_ai::py::py_end();    
            bottled_ai::show_error(msg);
            bottled_ai::showConsoles("Unexpected Error", false);
            result = 1;
            return;
        }

        if (!have_deeps && !install_deps()) {
            bottled_ai::py::py_end();    
            result = 2;
            return;
        }

        bottled_ai::py::get_py()->setDepsOk();
        bottled_ai::wait_python();

        /*
        bool success = false;
        bottled_ai::py::get_py()->execute_callback(bottled_ai::py::configure_stable_diffusion([&success, &msg] (bool status, const char *error) {
            msg = error;
            success = status;
        })); 

        if (!success) {
            bottled_ai::py::py_end();    
            bottled_ai::show_error(msg);
            bottled_ai::showConsoles("Unexpected Error", false);
            result = 1;
            return;
        } 
        */

        auto w = new bottled_ai::MainWindow(); // fltk deletes the object after we run it.
        result = w->run();
        bottled_ai::py::py_end();
    });

    bottled_ai::py::py_main();
    gui_thread.join();
    return result;
}
