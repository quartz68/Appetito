#include <console_menu.hpp>
#include <sstream>

void ConsoleMenu::operator()() const {
    std::string user_choice;
    const auto bad{commands_.cend()};
    auto result{bad};
    out << '\n';
    while ((out << prompt_) && (!(std::getline(in, user_choice)) ||
            ((result = commands_.find(user_choice)) == bad))) {
        out << '\n' << invalid_choice_message_;
    }
    result->second();
}