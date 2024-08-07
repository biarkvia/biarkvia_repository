#ifndef NOTEPADAPP_H
#define NOTEPADAPP_H

#include "text.h"
#include "todolist.h"
#include "shoppinglist.h"
#include "display.h"

using namespace std;

/**
 * NotepadApp class represents a notepad application.
 * Provides methods for running the application and performing various operations on notes and directories.
 */
class NotepadApp {
    struct SearchParameter {
        int parameter;
        string value;
    };
    Menu mainMenu;
    Menu noteCreateMenu;
    Menu noteSetTags;
    Menu noteSetPath;
    Menu noteSetName;
    Menu noteSearchMenu;
    Menu noteOpenMenu;
    Menu noteEditMenu;
    Menu noteEditContent;
    Menu noteEditInfo;
    Menu noteDeleteMenu;
    Menu sureMenu;
    Menu noteConvertToHTMLMenu;
    Menu noteImportNote;
    Menu * currentMenu;
    int new_note_type = 0;
    int search_criterion = 0;
    string new_note_path;
    string new_note_tags;
    string new_note_name;
    set<string> tags;
    shared_ptr<Directory> root_dir;
    vector<SearchParameter> search_parameters;

private:
    /**
     * Here we enter the path where the note will be stored.
     */
    void noteSetPathFunc();
    /**
     * Here we enter the tags for the note.
     */
    void noteSetTagsFunc();
    /**
     * Here we enter a name for the note.
     */
    void noteSetNameFunc();
    /** 
     * Function for opening a note. Shows basic information, content, and changes to the note.
     */
    void noteOpenFunc(Directory * root_dir, Menu * &currentMenu, Menu * mainMenu);
    /**
     * The function deletes the note by its ID.
     */
    void noteDeleteFunc(Directory * root_dir, Menu *& currentMenu, Menu * mainMenu);
    /**
     * The function asks for the note ID and runs the HTML conversion functions in the Export class.
     */
    void noteConvertToHTMLFunc(Directory * root_dir, Menu * & currentMenu, Menu * mainMenu);
    /**
     * The function asks for the name of the note that it will carry after importing, as well as the tags and what type of note to import into.
     */
    void noteImportFunc(Directory * root_dir, Menu * & currentMenu, Menu * mainMenu, set<string> & tags);
    /**
     * Updates note metadata based on user input.
     * @param root_dir Root directory for notes.
     * @param currentMenu Current displayed menu.
     */ 
    void noteEditInfoFunc(shared_ptr<Directory> root_dir, Menu *& currentMenu);
    /**
     * Allows user to modify note content.
     * @param root_dir Root directory for notes.
     * @param currentMenu Current displayed menu.
     * @param noteEditMenu Note edit menu.
     */
    void noteEditContentFunc(Directory * root_dir, Menu *& currentMenu, Menu * noteEditMenu);
    /**
     * Here the program searches for the note according to the search criteria set in the menu.
     */
    void noteSearchFunc();
public:
    NotepadApp();
    /**
     * Method to start running the Notepad Application.
     */
    void run();
};

#endif