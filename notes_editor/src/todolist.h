#ifndef TODOLIST_H
#define TODOLIST_H

#include <list>
#include <string>
#include <sstream>
#include "note.h"

using namespace std;

/** 
 * ToDoList class derived from Note, represents a to-do list note.
 * Contains a list of tasks. Each task is defined by its time and task text.
 */
class ToDoList: public Note {
public:
    /** 
     * @brief Struct representing a task, which consists of a time and task text.
     */
    struct Task {
        /**
         * Constructor for Task
         * @param time Time associated with the task
         * @param task_text Text of the task
         */
        Task(const string & time, const string & task_text);
        
        /**
         * Time associated with the task
         */
        string time;
        /**
         * Text of the task
         */
        string task_text;
    };

    /** 
     * @brief Constructor for ToDoList
     * @param name Name of the ToDoList
     * @param tags Set of tags associated with the ToDoList
     */
    ToDoList(const string & name, const set<string> & tags);

    /**
     * @brief Saves the state of the ToDoList with the command provided.
     */
    void saveState(const string & command);
    /**
     * @brief Adds a task to the beginning of the task list.
     */
    void addTaskAtBeginning(const Task & task);
    /**
     * @brief Adds a task to the end of the task list.
     */
    void addTaskAtEnd(const Task & task);
    /**
     * @brief Inserts a task at the specified index in the task list.
     */
    void insertTaskAt(const Task & task, size_t index);
    /**
     * @brief Edits a task at the specified index in the task list.
     */
    void editTask(size_t index, const string & new_time, const string & new_task_text);
    /**
     * @brief Removes a task at the specified index in the task list.
     */
    void removeTask(size_t index);
    /**
     * @brief Returns the tasks in the task list.
     */
    list<Task> getTasks() const;
    /**
     * @brief Searches for content in the task list that matches the search string.
     */
    virtual vector<string> findContent(const string & search_str) const override;
    /** 
     * @brief Modifies a ToDoList note.
     * This function should be overridden with specific implementation in derived classes. 
     */
    void editNoteFunc() override;
    /** 
     * @brief Converts the current ToDoList note into a string of HTML format.
     * @returns HTML string representing the current ToDoList note.
     */
    string toHTML() const override;
    /** 
     * @brief Imports a ToDoList note from an HTML file.
     * @param filename The name of the HTML file to import.
     */
    void importHTMLToNote(const string & filename) override;
    /**
     * @brief Displays the content of the ToDoList note.
     * This method prints the tasks in the ToDoList along with their associated times.
     */
    void displayContent() const override;
    friend class Import;
private:
    /**
     * list of Task objects of the ToDo note 
     */
    list<Task> tasks;
};

#endif