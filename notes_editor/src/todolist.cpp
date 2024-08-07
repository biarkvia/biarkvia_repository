#include "todolist.h"
#include <iostream>
#include "display.h"
#include "notepadapp.h"

/* This class represents a To-Do List note, inheriting from the Note class.
 * It contains a list of tasks and changes, and provides methods to manage these tasks (add, edit, remove),
 * convert the list to HTML, and display its content, info, and changes
 */
ToDoList::Task::Task(const string & time, const string & task_text) : time(time), task_text(task_text) {}

ToDoList::ToDoList(const string & name, const set<string> & tags) : Note(name, tags) {}

void ToDoList::saveState(const string & command) {
    changes.push_back(command);
}

void ToDoList::addTaskAtBeginning(const Task & task) {
    tasks.push_front(task);
}

void ToDoList::addTaskAtEnd(const Task & task) {
    tasks.push_back(task);
}

void ToDoList::insertTaskAt(const Task & task, size_t index) {
    if(index > tasks.size()) {
        throw out_of_range("Index is out of range.");
    }
    tasks.insert(next(tasks.begin(), index), task);
}

void ToDoList::editTask(size_t index, const string & new_time, const string & new_task_text) {
    if(index >= tasks.size()) {
        throw out_of_range("Index is out of range.");
    }
    auto iter = next(tasks.begin(), index);
    iter->task_text = new_task_text;
    iter->time = new_time;
}

void ToDoList::removeTask(size_t index) {
    if(index >= tasks.size()) {
        throw out_of_range("Index is out of range.");
    }
    tasks.erase(next(tasks.begin(), index));
}

list<ToDoList::Task> ToDoList::getTasks() const { 
    return tasks; 
}

vector<string> ToDoList::findContent(const string & search_str) const {
    vector<string> found_tasks;
    for(const auto & iter : tasks) {
        if(iter.task_text.find(search_str) != string::npos) {
            found_tasks.push_back(iter.task_text);
        }
    }
    return found_tasks;
}

void ToDoList::editNoteFunc() {
    cout << "= EXAMPLE INPUT: \"atb new_task\"\n= LIST OF COMMANDS: =\natb: Add task at beginning\nate: At end\nit: Insert task\net: Edit task\nrt: Remove task" << endl;
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
            } else if(sub_command == "atb") {
                string rest, time, task_text;
                getline(ss, rest);
                size_t last_space = rest.rfind(' ');
                if(last_space != string::npos) {
                    time = rest.substr(last_space + 1);
                    task_text = rest.substr(0, last_space);
                } else {
                    cout << "Invalid command format. Please use: atb TASK TEXT TIME" << endl;
                    continue;
                }
                this->addTaskAtBeginning(ToDoList::Task(time, task_text));
                this->saveState(command);
                this->displayContent();
            } else if(sub_command == "ate") {
                string rest, time, task_text;
                getline(ss, rest);
                size_t last_space = rest.rfind(' ');
                if(last_space != string::npos) {
                    time = rest.substr(last_space + 1);
                    task_text = rest.substr(0, last_space);
                } else {
                    cout << "Invalid command format. Please use: atb TASK TEXT TIME" << endl;
                    continue;
                }
                this->addTaskAtEnd(ToDoList::Task(time, task_text));
                this->saveState(command);
                this->displayContent();
            } else if(sub_command == "it") {
                size_t index;
                string rest, time, task_text;
                ss >> index;
                getline(ss, rest);
                size_t last_space = rest.rfind(' ');
                if(last_space != string::npos) {
                    time = rest.substr(last_space + 1);
                    task_text = rest.substr(0, last_space);
                } else {
                    cout << "Invalid command format. Please use: it INDEX TASK TEXT TIME" << endl;
                    continue;
                }
                this->insertTaskAt(ToDoList::Task(time, task_text), index - 1);
                this->saveState(command);
                this->displayContent();
            } else if(sub_command == "et") {
                size_t index;
                string rest, new_time, new_task_text;
                ss >> index;
                getline(ss, rest);
                size_t last_space = rest.rfind(' ');
                if(last_space != string::npos) {
                    new_time = rest.substr(last_space + 1);
                    new_task_text = rest.substr(0, last_space);
                } else {
                    cout << "Invalid command format. Please use: et INDEX NEW_TASK_TEXT NEW_TIME" << endl;
                    continue;
                }
                this->editTask(index - 1, new_time, new_task_text);
                this->saveState(command);
                this->displayContent();
            } else if(sub_command == "rt") {
                size_t index;
                ss >> index;
                this->removeTask(index - 1);
                this->saveState(command);
                this->displayContent();
            } else {
                cout << "Unknown command" << endl;
            }
        } catch(const out_of_range & error) {
            cout << "Error: " << error.what() << endl;
        }
    }
}

string ToDoList::toHTML() const {
    stringstream ss;
    ss << "<!DOCTYPE html>\n    <html>\n        <body>\n            <pre>\n";
    for(const auto & iter : tasks) {
        ss << "                <li>" << convertStringToHTML(iter.task_text) << " - " << iter.time << "</li>\n";
    }
    ss << "            </pre>\n        </body>\n        </html>";
    return ss.str();
}

void ToDoList::importHTMLToNote(const string & filename) {
    ifstream file(filename);
    if(!file) {
        throw runtime_error("File not found");
    }
    string line;
    bool content_started = false;
    while(getline(file, line)) {
        if(line == "<pre>") {
            content_started = true;
            continue;
        }
        if(line == "</pre>") {
            break;
        }
        if(content_started) {
            const string html_els[] = {"<li>", "</li>", "<br>", "<br/>"};
            for(const string & iter : html_els) {
                size_t pos = 0;
                while((pos = line.find(iter, pos)) != string::npos) {
                    line.replace(pos, iter.size(), "");
                }
            }
            size_t separator_pos = line.find(" - ");
            string task_text = line.substr(0, separator_pos);
            string time = line.substr(separator_pos + 3);
            this->tasks.push_back(ToDoList::Task(time, task_text));
        }
    }
    file.close();
}

void ToDoList::displayContent() const {
    cout << "---------------------------------------------------------------" << endl;
    cout << "Time | Task" << endl;
    int task_number = 1;
    for (const auto & task : getTasks()) {
        cout << task_number << ". " << task.time << " | " << task.task_text << endl;
        ++task_number;
    }
    cout << "---------------------------------------------------------------" << endl;
}