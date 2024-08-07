#ifndef TEXT_H
#define TEXT_H

#include <fstream>
#include "helpers.h"
#include "note.h"
#include "directory.h"

/** Text class derived from Note, represents a text note. */
class Text: public Note {
public:
    /**
     * Constructor which initializes name and tags of the Text note.
     * @param[in] name name of the text note
     * @param[in] tags set of tags associated with the text note
     */
    Text(const string & name, const set<string> & tags);

    /** 
     * @brief Manipulates and retrieves information from the Text note
     * @param[in] command command to save the state of the note
     */
    void saveState(const string & command);
    /** 
     * @brief Adds an empty line to the Text note
     * @param[in] line line to be added to the note
     */
    void addEmptyLine(const string & line);
    /** 
     * @brief Adds a line to the Text note at a given index
     * @param[in] line line to be added
     * @param[in] index position to add the line at
     */
    void addLine(const string & line, size_t index);
    /** 
     * @brief Edits a line in the Text note
     * @param[in] index position of line to be edited
     * @param[in] new_line new content for the line
     */
    void editLine(size_t index, const string & new_line);
    /** 
     * @brief Removes a line from the Text note
     * @param[in] index position of line to be removed
     */
    void removeLine(size_t index);
    /** 
     * @brief Retrieves all lines from the Text note
     * @returns vector of strings, each string being a line from the note
     */
    vector<string> getLines() const;
    /** 
     * @brief Finds content in the Text note
     * @param[in] search_str string to be searched in the note
     * @returns vector of strings containing lines that match the search string
     */
    vector<string> findContent(const string & search_str) const;
    /** 
     * @brief Modifies a Text note.
     * This function should be overridden with specific implementation in derived classes.
     */
    void editNoteFunc() override;
    /** 
     * @brief Converts the current Text note into a string of HTML format.
     * @returns HTML string representing the current Text note.
     */
    string toHTML() const override;
    /** 
     * @brief Imports a Text note from an HTML file.
     * @param filename The name of the HTML file to import.
     */
    void importHTMLToNote(const string & filename) override;
    /**
     * @brief Displays the content of the Text note.
     * This method prints the tasks in the Text note along with their associated times.
     */
    void displayContent() const override;
private:
    /**
     * lines of the Text note
     */
    vector<string> lines;
};

#endif