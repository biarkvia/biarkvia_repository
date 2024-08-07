#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <ctime>
#include <functional>
#include "directory.h"

class Directory;

using namespace std;

/**
 * Struct representing a menu command, which includes a title and a function to execute.
 */
struct MenuCommand {
    /**
     * Title of the command.
     */
    string title;
    /**
     * Function to execute when this command is chosen.
     */
    function<void()> my_function;
};

/**
 * Struct representing a menu, which includes a title and a list of commands.
 */
struct Menu {
    /**
     * Title of the menu.
     */
    string title;
    /**
     * List of commands available in the menu.
     */
    vector<MenuCommand> commands;

    /**
     * Lists all commands in the menu.
     */
    void commandList() const;
    /**
     * Allows the user to choose a command to execute.
     */
    void chooseCommand();
    /**
     * Sets the title of the menu.
     */
    void setTitle(const string & new_title);
};

/**
 * Converts a given input string to HTML format.
 * @param[in] input The string to convert to HTML.
 * @returns The input string in HTML format.
 */
string convertStringToHTML(const string & input);
/**
 * Formats the provided time to a standard time format.
 * @param[in] original_time The time to format.
 * @returns The formatted time.
 */
string formatTime(time_t original_time);
/**
 * Checks if the provided data is valid for the given type.
 * @param[in] type The type of data.
 * @param[in,out] data The data to check.
 * @returns True if the data is valid, false otherwise.
 */
bool checkData(int type, string & data);

#endif // HELPERS_H