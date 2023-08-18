#ifndef SRC_CHAT_STORE_H
#define SRC_CHAT_STORE_H

#include <string>
#include <vector>

namespace bottled_ai
{

typedef struct {
    std::string context;
    std::string prompt;
    std::string model_id;
    std::string html;
    std::string raw;
} chat_element_t;

typedef std::vector<chat_element_t> chat_list_t;

class ChatStore {
 public:
    ChatStore();
    ~ChatStore();
    void executePrompt(const std::string& model_id, const std::string &prompt, bool use_context);
    size_t count();
    std::string toText();
    std::string toHtml();
    void regenerate(size_t index);
    chat_element_t& at(size_t index);
    void remove(size_t index);
    void clear();
    std::string context();
    void setParams(int max_new_tokens, float temperature, float top_p, float top_k, float repetition_penalty);
    
 private:
    chat_element_t executePrompt(const std::string& model_id, const std::string &prompt, const std::string& context);
 private:
    chat_list_t history_;
    int max_new_tokens = 1024;
    float temperature = 1;
    float top_p = 1;
    float top_k = 0;
    float repetition_penalty = 1;
};
    
} // namespace bottled_ai

#endif  // SRC_CHAT_STORE_H
