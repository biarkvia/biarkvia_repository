#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>
#include "note.h"
#include <unordered_map>

class Text;

using namespace std;

/**
 * Directory class represents a directory which contains subdirectories and notes.
 * Provides methods for creating, searching, adding, and removing subdirectories and notes.
 */
class Directory : public enable_shared_from_this<Directory> {
public:
    /**
     * Static factory method that creates a directory path and returns a shared pointer to the root.
     * @param[in] path The path to be created.
     * @param[in] root The root directory.
     * @returns A shared pointer to the root directory.
     */
    static shared_ptr<Directory> createPath(const string & path, const shared_ptr<Directory> & root);
    /**
     * Constructor for the Directory class.
     * @param[in] name The name of the directory.
     */
    Directory(const string &name);
    /**
     * Adds a subdirectory to the current directory.
     * @param[in] subdir A shared pointer to the subdirectory to be added.
     */
    void addSubdir(const shared_ptr<Directory> & subdir);
    /**
     * Creates a subdirectory in the current directory.
     * @param[in] name The name of the subdirectory to be created.
     * @returns A shared pointer to the created subdirectory.
     */
    shared_ptr<Directory> createSubdir(const string & name);
    /**
     * Adds a note to the current directory.
     * @param[in] note A shared pointer to the note to be added.
     */
    void addNote(const shared_ptr<Note> & note);
    /**
     * Removes a note with the given ID.
     * @param[in] id The ID of the note to be removed.
     */
    void removeNoteById(int id);
    /**
     * Finds and returns notes with the given name in the current directory.
     * @param[in] search_note_name The name of the notes to be found.
     * @returns A vector of shared pointers to the found notes.
     */
    vector<shared_ptr<Note>> findNotes(const string & search_note_name) const;
    /**
     * Gets the name of the directory.
     * @returns The name of the directory.
     */
    string getName() const;
    /**
     * Finds and returns a subdirectory with the given name in the current directory.
     * @param[in] subdir_name The name of the subdirectory to be found.
     * @returns A shared pointer to the found subdirectory.
     */
    shared_ptr<Directory> findSubdir(const string & subdir_name) const;
    /**
     * Gets all subdirectories of the current directory.
     * @returns A vector of shared pointers to the subdirectories.
     */
    const vector<shared_ptr<Directory>> & getSubdirectories() const;
    /**
     * Gets all notes in the current directory.
     * @returns A vector of shared pointers to the notes.
     */
    vector<shared_ptr<Note>> getNotes() const;
    /**
     * @brief Retrieves all notes located at the given directory path.
     * If the path is empty, it gets all notes in the current directory and its subdirectories.
     * If the path is not empty, it finds the directory corresponding to the path and gets all notes in that directory and its subdirectories.
     * @param path The path to the directory to get the notes from.
     * @return A vector of shared pointers to the notes located at the given path.
     */
    vector<shared_ptr<Note>> getNotesInPath(const string & path) const;
    /**
     * Finds and returns notes with the specified tag in all directories.
     * @param[in] tag The tag used to find notes.
     * @returns A vector of shared pointers to the found notes.
     */
    vector<shared_ptr<Note>> findNotesByTag(const string & tag) const;
    /**
     * Finds and returns notes with the specified creation date in all directories.
     * @param[in] creation_date The creation date used to find notes.
     * @returns A vector of shared pointers to the found notes.
     */
    vector<shared_ptr<Note>> findNotesByCreationDate(const string & creation_date) const;
    /**
     * Finds and returns a note with the given ID in all directories.
     * @param[in] id The ID of the note to be found.
     * @returns A shared pointer to the found note.
     */
    shared_ptr<Note> findNoteById(int id) const;
     /**
     * Finds and returns notes that contain the given string in their content.
     * @param[in] search_str The search string to be matched.
     * @returns A vector of shared pointers to the found notes.
     */
    vector<shared_ptr<Note>> findNotesByContent(const string & search_str) const;
    /**
     * Creates a note from an HTML file and adds it to the current directory.
     * @param[in] name The name of the note.
     * @param[in] tags The set of tags for the note.
     * @param[in] filename The name of the HTML file.
     * @param[in] type The type of the note (Text, ToDoList, ShoppingList).
     * @returns A shared pointer to the created note.
     */
    shared_ptr<Note> createNoteFromHTML(const string & name, const set<string> & tags, const string & filename, const int & type);

private:
    /**
     * Name of the directory 
     */
    string m_name;
    /**
     * Vector storing shared pointers to the subdirectories of the current directory
     */
    vector<shared_ptr<Directory>> m_subdirs;
    /**
     * Unordered map storing shared pointers to notes, with integer keys
     */
    unordered_map<int, shared_ptr<Note>> m_notes;
};

#endif // DIRECTORY_H