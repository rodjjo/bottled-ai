#include <sstream>
#include "src/data/chat_store.h"

#include "src/python/helpers.h"
#include "src/python/wrapper.h"
#include "src/python/utils.h"


namespace bottled_ai
{

ChatStore::ChatStore() {
}

ChatStore::~ChatStore() {

}

void ChatStore::executePrompt(const std::string& model_id, const std::string &prompt, bool use_context) {
    chat_element_t el = executePrompt(
        model_id,
        prompt,
        use_context ? context() : std::string()
    );
    history_.push_back(el);
}

size_t ChatStore::count() {
    return history_.size();
}

void ChatStore::setParams(int max_new_tokens, float temperature, float top_p, float top_k, float repetition_penalty) {
    this->max_new_tokens = max_new_tokens;
    this->temperature = temperature;
    this->top_p = top_p;
    this->top_k = top_k;
    this->repetition_penalty = repetition_penalty;
}

chat_element_t ChatStore::executePrompt(const std::string& model_id, const std::string &prompt, const std::string& context) {
    chat_element_t el;
    el.model_id = model_id;
    el.context = context;
    el.prompt = prompt;

    std::string respHtml;
    std::string respRaw;

    py::get_py()->execute_callback(
        py::generate_text(
            model_id.c_str(), 
            prompt.c_str(), 
            [&respHtml, &respRaw] (bool success, const char *html, const char *raw) {
                if (html) {
                    respHtml = html;
                }
                if (raw) {
                    respRaw = raw;
                }
        })
    );
   
    respHtml = std::string("<p><font face=\"arial\" size=\"6\"><b>Assistant:</b></font></p>") + respHtml; 
    respHtml = std::string("<p><font face=\"arial\" size=\"6\"><b>User:</b></font></p>") + bottled_ai::convert2html(prompt) + respHtml; 
    el.html = respHtml;
    el.raw = respRaw;
    return el;
}

void ChatStore::regenerate(size_t index) {
    chat_element_t &el = at(index);
    auto elptr = &el;
    *elptr = executePrompt(el.model_id, el.prompt, el.context);
}

std::string ChatStore::toText() {
    std::string result;
    for (auto &d: history_) {
        result += d.raw + "\n";
    }
    return result;
}

std::string ChatStore::toHtml() {
    std::stringstream ss;
    for (size_t i = 0; i < count(); ++i) {
        if (i + 1 == count())
            ss << "<a name=\"the_end_mark\"></a>";
        ss << at(i).html;
        ss << "<CENTER><p>";
        ss << "<a href='/?a=copy-response-" << i << "'>[Copy Response]</a> ";
        ss << "<a href='/?a=copy-prompt-" << i << "'>[Copy Prompt]</a> ";
        ss << "<a href='/?a=copy-" << i << "'>[Copy Both]</a> ";
        ss << "<a href='/?a=context-" << i << "'>[Context]</a> ";
        ss << "<a href='/?a=regenerate-" << i << "'>[Regenerate]</a> ";
        ss << "<a href='/?a=delete-" << i << "'>[Remove]</a>";
        ss << "</p></CENTER>";
        if (i + 1 != count())
            ss << "<HR>";
    }
    return ss.str();
}

chat_element_t& ChatStore::at(size_t index) {
    return history_[index];
}

void ChatStore::remove(size_t index) {
    history_.erase(history_.begin() + index);
}

void ChatStore::clear() {
    history_.clear();
}

std::string ChatStore::context() {
    return std::string();
}
    
} // namespace bottled_ai

    