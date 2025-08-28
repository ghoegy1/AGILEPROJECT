/***************************************************************************************
 * Utils.cpp — implementation
 ****************************************************************************************/
#include "Utils.hpp"

#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <iomanip>

namespace sb {

std::string trim(std::string s) {
    auto notSpace = [](unsigned char ch){ return !std::isspace(ch); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
    s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
    return s;
}

std::string upperCopy(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return s;
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string tok;
    while (std::getline(ss, tok, delim)) {
        out.push_back(trim(tok));
    }
    return out;
}

std::string safeGetLine() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int promptIntInRange(const std::string& prompt, int lo, int hi) {
    while (true) {
        std::cout << prompt;
        int x;
        if (std::cin >> x && x >= lo && x <= hi) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return x;
        }
        std::cout << "Invalid choice. Please enter a number between "
                  << lo << " and " << hi << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

bool parseHHMM(const std::string& s, int& minutesOut) {
    int h, m; char colon;
    std::stringstream ss(s);
    if (!(ss >> h)) return false;
    if (!(ss >> colon) || colon != ':') return false;
    if (!(ss >> m)) return false;
    if (h < 0 || h > 23 || m < 0 || m > 59) return false;
    minutesOut = h * 60 + m;
    return true;
}

std::string formatHHMM(int minutes) {
    int h = minutes / 60;
    int m = minutes % 60;
    std::ostringstream os;
    os << std::setw(2) << std::setfill('0') << h << ":"
       << std::setw(2) << std::setfill('0') << m;
    return os.str();
}

} // namespace sb