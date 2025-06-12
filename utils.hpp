#pragma once
#include <string>
#include <iomanip>

inline std::string truncate_text(const std::string& text, size_t max_length) {
    if (text.length() <= max_length) return text;
    
    return "..." + text.substr(text.length() - max_length + 3);
}