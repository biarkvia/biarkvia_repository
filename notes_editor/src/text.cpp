#include "text.h"
#include "display.h"
#include <iostream>

using namespace std;

/* This class represents a Text note, inheriting from the Note class.
 * It contains a list of text lines and changes, and provides methods to manage these lines (add, edit, remove), 
 * convert the text to HTML, import from an HTML file, and display its content, info, and changes
 */
Text::Text(const string & name, const set<string> & tags) : Note(name, tags) {}

void Text::saveState(const string & command) {
    changes.push_back(command);
}

void Text::addEmptyLine(const string & line) {
    lines.push_back("");
}

void Text::addLine(const string & line, size_t index) {
    if(index > lines.size()) {
        throw out_of_range("Index is out of range.");
    }
    lines.insert(lines.begin() + index, line);
}

void Text::editLine(size_t index, const string & new_line) {
    if(index >= lines.size()) {
        throw out_of_range("Index is out of range.");
    }
    lines[index] = new_line;
}

void Text::removeLine(size_t index) {
    if(index >= lines.size()) {
        throw out_of_range("Index is out of range.");
    }
    lines.erase(lines.begin() + index);
}

vector<string> Text::getLines() const {
    return lines;
}

vector<string> Text::findContent(const string & search_str) const {
    vector<string> found_lines;
    for(const auto & line : lines) {
        if(line.find(search_str) != string::npos) {
            found_lines.push_back(line);
        }
    }
    return found_lines;
}

void Text::editNoteFunc() {
    cout << "= EXAMPLE INPUT: \"al index your_text\"\n= LIST OF COMMANDS: =\nal: Add line at\ndl: Delete line at\nel: Edit line at\nael: Add empty line" << endl;
    Display::displayInfo(*this);
    cout << "CONTENT: " << endl;
    this->displayContent();
    string command;
    while(getline(cin, command)) {
        try {
            stringstream ss(command);
            string sub_command;
            ss >> sub_command;
            if(sub_command == "exit") {
                break;
            } else if(sub_command == "ael") {
                this->addEmptyLine("");
                this->saveState(command);
                this->displayContent();
            } else if(sub_command == "al" || sub_command == "dl" || sub_command == "el") {
                size_t index;
                ss >> index;
                if(ss.fail()) {
                    cout << "Invalid index" << endl;
                    continue;
                }
                if(sub_command == "al") {
                    string line;
                    getline(ss, line);
                    this->addLine(line, index - 1);
                    this->saveState(command);
                    this->displayContent();
                } else if(sub_command == "dl") {
                    this->removeLine(index - 1);
                    this->saveState(command);
                    this->displayContent();
                } else if(sub_command == "el") {
                    string line;
                    getline(ss, line);
                    this->editLine(index - 1, line);
                    this->saveState(command);
                    this->displayContent();
                }
            } else {
                cout << "Unknown command" << endl;
            }
        } catch(const out_of_range & error) {
            cout << "Error: " << error.what() << endl;
        }
    }
}

string Text::toHTML() const {
    stringstream ss;
    ss << "<!DOCTYPE html>\n    <html>\n        <body>\n            <pre>\n";
    for(const auto & iter : lines) { 
        ss << convertStringToHTML(iter) << "<br/>\n";
    }
    ss << "\n            </pre>\n        </body>\n    </html>";
    return ss.str();
}   

void Text::importHTMLToNote(const string & filename) {
    ifstream file(filename);
    if(!file) {
        throw runtime_error("File not found");
    }
    string line;
    bool content_started = false;
    bool has_html_tag = false;
    bool has_body_tag = false;
    bool has_pre_tag = false;
    while(getline(file, line)) {
        line.erase(line.begin(), find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !isspace(ch);
        }));
        line.erase(find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !isspace(ch);
        }).base(), line.end());
        if (line == "<html>") {
            has_html_tag = true;
        }
        if(line == "<body>") {
            has_body_tag = true;
        }
        if(line == "<pre>") {
            has_pre_tag = true;
            content_started = true;
            continue;
        }
        if(line == "</pre>") {
            break;
        }
        if(content_started) {
            const string html_els[] = {"<li>", "</li>", "<br>", "<br/>", "<table style=\"width:100%\">", "<th>", "</th>", "<td>", "</td>", "<tr>", "</tr>", "<table>", "</table>", "<p>", "</p>"};
            for(const string & iter : html_els) {
                size_t pos = 0;
                while((pos = line.find(iter, pos)) != string::npos) {
                    line.replace(pos, iter.size(), "");
                }
            }
            this->lines.push_back(line);
        }
    }
    file.close();
    if(!has_html_tag || !has_body_tag || !has_pre_tag) {
        throw runtime_error("Invalid HTML structure");
    }
}

void Text::displayContent() const {
    cout << "---------------------------------------------------------------" << endl;
    for (const auto &line : this->lines) {
        cout << line << endl;
    }
    cout << "---------------------------------------------------------------" << endl;
}