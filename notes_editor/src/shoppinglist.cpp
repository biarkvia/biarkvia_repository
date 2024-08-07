#include "shoppinglist.h"
#include "helpers.h"
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include "display.h"

/* This class represents a Shopping List note, inheriting from the Note class.
 * It contains a list of items and changes, and provides methods to manage these items (add, edit, remove),
 * calculate the total price, convert the list to HTML, and display its content, info, and changes
*/
using namespace std;

ShoppingList::Item::Item(double price, int quantity, const string & name) : price(price), quantity(quantity), name(name) {}

ShoppingList::ShoppingList(const string & name, const set<string> & tags) : Note(name, tags) {}

void ShoppingList::saveState(const string & command) {
    changes.push_back(command);
}

void ShoppingList::addItem(const Item & item) {
    items.push_back(item);
}

void ShoppingList::editItem(size_t index, double new_price, int new_quantity,  const string & new_name) {
    if(index >= items.size()) {
        throw out_of_range("Index is out of range.");
    }
    items[index].name = new_name;
    items[index].price = new_price;
    items[index].quantity = new_quantity;
}

void ShoppingList::deleteItem(size_t index) {
    if(index >= items.size()) {
        throw out_of_range("Index is out of range.");
    }
    items.erase(items.begin() + index);
}

vector<ShoppingList::Item> ShoppingList::getItems() const { 
    return items; 
}

double ShoppingList::getTotalPrice() const {
    double total_price = 0.0;
    for(const auto & iter : items) {
        total_price += iter.price * iter.quantity;
    }
    return total_price;
}

vector<string> ShoppingList::findContent(const string & search_str) const {
    vector<string> found_items;
    for(const auto & item : items) {
        if(item.name.find(search_str) != string::npos) {
            found_items.push_back(item.name);
        }
    }
    return found_items;
}

void ShoppingList::editNoteFunc() {
    cout << "= EXAMPLE INPUT: \"ei index price quantity name\"\n= LIST OF COMMANDS: =\nai: Add item\nei: Edit item\ndi: Delete item" << endl;
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
            } else if(sub_command == "ai") {
                string name;
                double price;  
                int quantity;
                ss >> price >> quantity >> name;
                this->addItem(ShoppingList::Item(price, quantity, name));
                this->saveState(command);
                this->displayContent();
            } else if(sub_command == "ei") {
                size_t index;
                string new_name;
                double new_price;
                int new_quantity;
                ss >> index >> new_price >> new_quantity >> new_name;
                this->editItem(index - 1, new_price, new_quantity, new_name);
                this->saveState(command);
                this->displayContent();
            } else if(sub_command == "di") {
                size_t index;
                ss >> index;
                this->deleteItem(index - 1);
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

string ShoppingList::toHTML() const {
    stringstream ss;
    ss << "<!DOCTYPE html>\n    <html>\n        <body>\n            <pre>\n";
    ss << "            <table style=\"width:100%\">\n";
    ss << "                <tr>\n";
    ss << "                    <th>Price</th>\n";
    ss << "                    <th>Quantity</th>\n";
    ss << "                    <th>Name of item</th>\n";
    ss << "                </tr>\n";
    for(const auto & iter : items) {
        ss << "                <tr>\n";
        ss << "                    <td>" << fixed << setprecision(2) << iter.price << "</td>\n";
        ss << "                    <td>" << iter.quantity << "</td>\n";
        ss << "                    <td>" << convertStringToHTML(iter.name) << "</td>\n";
        ss << "                </tr>\n";
    }
    ss << "            </table>\n";
    ss << "            <p>Total price: " << fixed << setprecision(2) << getTotalPrice() << "</p>\n";
    ss << "            </pre>\n        </body>\n        </html>";
    return ss.str();
}

void ShoppingList::importHTMLToNote(const string & filename) {
    ifstream file(filename);
    if(!file) {
        throw runtime_error("File not found");
    }
    string line;
    bool content_started = false;
    bool inside_table_row = false;
    bool header_row_passed = false;
    vector<string> row_data;
    while(getline(file, line)) {
        line.erase(line.begin(), find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !isspace(ch);
        }));
        line.erase(find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !isspace(ch);
        }).base(), line.end());
        if(line == "<pre>") {
            content_started = true;
            continue;
        }
        if(line == "</pre>") {
            break;
        }
        if(content_started) {
            const string html_els[] = {"<td>", "</td>"};
            if(line == "<tr>") {
                inside_table_row = true;
            }
            else if(line == "</tr>") {
                inside_table_row = false;
                header_row_passed = true;

                if(row_data.size() == 3) {
                    try {
                        double price = stod(row_data[0]);
                        int quantity = stoi(row_data[1]);
                        string name = row_data[2];
                        this->items.push_back(ShoppingList::Item(price, quantity, name));
                    }
                    catch(const invalid_argument& ia) {
                        cerr << "Invalid argument: " << ia.what() << '\n';
                    }
                    row_data.clear();
                }
            }
            else if(inside_table_row && header_row_passed) {
                for(const string & iter : html_els) {
                    size_t pos = 0;
                    while((pos = line.find(iter, pos)) != string::npos) {
                        line.replace(pos, iter.size(), "");
                    }
                }
                row_data.push_back(line);
            }
        }
    }
    file.close();
}

void ShoppingList::displayContent() const {
        cout << "---------------------------------------------------------------" << endl;
        cout << "Price | Quantity | Name of item" << endl;
        int item_number = 1;
        for(const auto & item : getItems()) {
            cout << item_number << ". " << fixed << setprecision(2) << item.price << " | " << item.quantity << " |" << item.name << endl;
            ++item_number;
        }
        cout << "Total price: " << fixed << setprecision(2) << getTotalPrice() << endl;
        cout << "---------------------------------------------------------------" << endl;
    }