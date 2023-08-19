#pragma once

#include <string>

namespace bottled_ai {

typedef enum {
    r_yes = 0,
    r_no,
    r_cancel
} response_t;

bool ask(const char *message);
response_t yes_nc(const char *message);
void show_error(const char *message);
const char *ask_value(const char *message);

}  // namespace bottled_ai
