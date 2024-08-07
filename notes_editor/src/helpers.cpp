#include "helpers.h"
#include <sstream>

void Menu::commandList() const {
    cout << title << endl;
    for(size_t i = 0; i < commands.size(); i++) {
        cout << "[" << (i + 1) << "] " << commands[i].title << endl;
    }
}

void Menu::chooseCommand() {
    int choice = 0;
    while(true) {
        cout << "Enter choice: ";
        cin >> choice;
        if(cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Please enter a valid number!" << endl;
            continue;
        }
        if(choice < 1 || choice > static_cast<int>(commands.size())) {
            cout << "There is no such option!" << endl;
            continue;
        } else {
            commands[choice - 1].my_function();
            break;
        }
    }
}

void Menu::setTitle(const string & new_title) {
    title = new_title;
}

string convertStringToHTML(const string & input) {
    stringstream ss;
    for(char symbol : input) {
        switch (symbol) {
        case '<':
            ss << "&lt;";
            break;
        case '>':
            ss << "&gt;";
            break;
        case '&':
            ss << "&amp;";
            break;
        case '"':
            ss << "&quot;";
            break;
        case '\'':
            ss << "&apos;";
            break;
        default:
            ss << symbol;
            break;
        }
    }
    return ss.str();
}

string formatTime(time_t original_time) {
    tm *time_information = localtime(&original_time);
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%H:%M %d.%m.%Y", time_information);
    return string(buffer);
}

bool checkData(int type, string & data) {
    if(type == 1) {
        if(data.size() > 255 || data.empty()) {
            return false;
        }
    }
    if(type == 2) {
        if(data.size() > 15) {
            cout << "The tag is too long!" << endl;
            return false;
        }
    }
    return true;
}