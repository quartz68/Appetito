#ifndef CONSOLE_MENU_HPP_
#define CONSOLE_MENU_HPP_

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

/**
 * @brief A console menu.
 */
class ConsoleMenu {
public:
    ConsoleMenu(const string& title,
        const string& invalid_choice_message,
        const string& prompt,
        const map<string, function<void()>>& commands,
        istream &in = cin,
        ostream &out = cout)
        : invalid_choice_message_{invalid_choice_message},
        commands_{commands},
        in{in}, 
        out{out} 
        {
            std::stringstream ss;
            ss << title;
            for (const auto &command : commands) {
            ss << '[' << command.first << ']' << '\n';
            }
            ss << prompt;
            prompt_ = ss.str();
        }
    void operator()() const;
private:
    string invalid_choice_message_;
    // the prompt_ string contains the message, all choices and prompt
    string prompt_;
    map<std::string, std::function<void()>> commands_;
    istream &in;
    ostream &out;
};

#endif