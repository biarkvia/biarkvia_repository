#ifndef NOTE_H
#define NOTE_H

#include <string>
#include <set>
#include <ctime>
#include <vector>

using namespace std;

/** Parent class Note, containing common attributes and operations for all note types. */
class Note {
public:
    /**
    * Constructor for the Note class.
    * @param[in] name The name of the note.
    * @param[in] tags The set of tags associated with the note.
    */
    Note(const string & name, const set<string> & tags);
    virtual ~Note() {}
    /**
    * Get the name of the note.
    * @returns The name of the note.
    */
    string noteGetName() const;
    /**
    * Set the name of the note.
    * @param[in] name The new name for the note.
    */
    void noteSetName(const string & name);
    /**
    * Get the tags associated with the note.
    * @returns The set of tags associated with the note.
    */
    set<string> noteGetTags() const;
    /**
    * Set the tags associated with the note.
    * @param[in] tags The new set of tags for the note.
    */
    void noteSetTags(const set<string> & tags);
    /**
    * Get the creation time of the note.
    * @returns The creation time of the note.
    */
    time_t noteGetCreationTime() const;
    /**
    * Set the creation time of the note.
    * @param[in] creation_date The new creation time for the note.
    */
    void noteSetCreationTime(const time_t & creation_date);
    /**
    * Get the ID of the note.
    * @returns The ID of the note.
    */
    int getId() const;
    /**
    * Find the content in the note that matches the search string.
    * @param[in] search_str The search string to be matched.
    * @returns A vector of strings containing the matching content.
    */
    virtual vector<string> findContent(const string & search_str) const = 0;
    /**
    * Get the changes made to the note.
    * @returns A vector of strings representing the changes made to the note.
    */
    vector<string> getChanges() const;
    /** 
     * @brief Abstract method to modify a note.
     * The specific implementation of this method should be provided in the derived classes.
     */
    virtual void editNoteFunc() = 0;
    /** 
     * @brief Abstract method to convert the current note into a string of HTML format.
     * The specific implementation of this method should be provided in the derived classes.
     * @returns HTML string representing the current note.
     */
    virtual string toHTML() const = 0;
    /** 
     * @brief Abstract method to import a note from an HTML file.
     * The specific implementation of this method should be provided in the derived classes.
     * @param filename The name of the HTML file to import.
     */
    virtual void importHTMLToNote(const string & filename) = 0;
    /** 
     * @brief Abstract method to display content of a Note.
     * The specific implementation of this method should be provided in the derived classes.
     */
    virtual void displayContent() const = 0;
protected:
    /**
     * static identifier to ensure each Note object has a unique id 
     */
    static int s_nextId;
    /**
     * name of the note 
     */
    string m_name;
    /**
     * set of tags associated with the note 
     */
    set<string> m_tags;
    /**
     * time of note creation 
     */
    time_t m_creation_time;
    /**
     * unique identifier for the note 
     */
    int m_id;
    /**
     * record of changes made to the note 
     */
    vector<string> changes;
};

#endif