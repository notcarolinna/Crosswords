#include "../include/utils.h"
#include <algorithm>
#include <cctype>

void leftTrim(std::string &str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void rightTrim(std::string &str) {
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
}

void trim(std::string &str) {
    rightTrim(str);
    leftTrim(str);
}