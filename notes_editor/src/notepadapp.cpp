#include "notepadapp.h"

int getInputId() {
    string input;
    int search_id;
    bool first_run = true;
    cout << "Enter ID: ";
    while (true) {
        getline(cin, input);
        try {
            search_id = stoi(input);
            break;
        }
        catch(invalid_argument& e) {
            if(!first_run) {
                cout << "Please enter a valid number!" << endl;
                break;
            }
            first_run = false;
        }
    }
    return search_id;
}

void NotepadApp::noteSetPathFunc() {
    cout << "Enter path: ";
    cin >> new_note_path;  
    cin.ignore();
    if(new_note_path == "1") {
        currentMenu = &noteCreateMenu;
    } else if(!checkData(1, new_note_path)) {
        cout << "A path cannot be created!" << endl;
        currentMenu = &noteSetPath; 
    } else {
        currentMenu = &noteSetTags;
    }
}

void NotepadApp::noteSetTagsFunc() {
    cout << "Enter tags separated by spaces: ";
    getline(cin, new_note_tags);
    istringstream iss(new_note_tags);
    string first_input;
    iss >> first_input;
    if(first_input == "1") {
        currentMenu = &noteSetName;
    } else if (first_input == "2") {
        currentMenu = &noteSetPath;
    } else {
        tags.clear();
        tags.insert(first_input);
        string tag;
        bool input_error = false;
        while(iss >> tag) {
            if(!checkData(2, tag)) {
                cout << "A tag '" << tag << "' cannot be created!" << endl;
                input_error = true;
                break;
            }
            tags.insert(tag);
        }
        if(!input_error) {
            currentMenu = &noteSetName;
        }
    }
}

void NotepadApp::noteSetNameFunc() {
    string new_note_name;
    cout << "Enter the note name: ";
    getline(cin, new_note_name);
    if(new_note_name == "1") {
        currentMenu = &noteSetTags;
        return;
    }
    if(checkData(1, new_note_name)) {
        auto new_note_dir = Directory::createPath(new_note_path, root_dir);
        if(new_note_type == 1) {
            auto new_note = make_shared<Text>(new_note_name, tags);
            new_note_dir->addNote(new_note);
            cout << "The note was successfully created" << endl;
            cout << "Path to note: " << new_note_path << endl;
            Display::displayInfo(*new_note);
        } else if(new_note_type == 2) {
            auto new_note = make_shared<ToDoList>(new_note_name, tags);
            new_note_dir->addNote(new_note);
            cout << "The note was successfully created!" << endl;
            cout << "Path to note: " << new_note_path << endl;
            Display::displayInfo(*new_note);
        } else if(new_note_type == 3) {
            auto new_note = make_shared<ShoppingList>(new_note_name, tags);
            new_note_dir->addNote(new_note);
            cout << "The note was successfully created!" << endl;
            cout << "Path to note: " << new_note_path << endl;
            Display::displayInfo(*new_note);
        } else {
            cout << "This type of note does not exist" << endl;
            currentMenu = &noteSetName; 
        }
        new_note_type = 0;
        currentMenu = &mainMenu;
    } else {
        cout << "A note name cannot be created" << endl;
        currentMenu = &noteSetName; 
    }
}

void NotepadApp::noteSearchFunc() {
    set<int> found_ids;
    vector<shared_ptr<Note>> found_notes;
    bool first_search = true;
    for (const auto & param : search_parameters) {
        vector<shared_ptr<Note>> notes;
        switch (param.parameter) {
            case 1: {
                notes = root_dir->findNotes(param.value);
                break;
            }
            case 2: {
                notes = root_dir->findNotesByTag(param.value);
                break;
            }
            case 3: {
                notes = root_dir->findNotesByCreationDate(param.value);
                break;
            }
            case 4: {
                notes = root_dir->getNotesInPath(param.value);
                break;
            }
            case 5: {
                notes = root_dir->findNotesByContent(param.value);
                break;
            }
            default: {
                cout << "Unknown search type" << endl;
                break;
            }
        }
        if(first_search) {
            found_notes = notes;
            first_search = false;
        } else {
            vector<shared_ptr<Note>> new_found_notes;
            for (const auto & note : notes) {
                if (found_ids.count(note->getId()) == 0) {
                    found_ids.insert(note->getId());
                    new_found_notes.push_back(note);
                }
            }
            found_notes = new_found_notes;
        }
        if (found_notes.empty()) {
            break;
        }
    }
    if (!found_notes.empty()) {
        cout << "Found notes: " << endl;
        int counter = 1;
        for (const auto & note : found_notes) {
            cout << counter << ". " << note->noteGetName() << " (id = " << note->getId() << ")" << endl;
            counter++;
        }
    } else {
        cout << "No notes found that match all criteria" << endl;
    }
    search_parameters.clear();
}

void NotepadApp::noteOpenFunc(Directory * root_dir, Menu * &currentMenu, Menu * mainMenu) {
    int search_id = getInputId();
    if(search_id == 1) {
        currentMenu = mainMenu;
    } else {
        auto found_note = root_dir->findNoteById(search_id);
        if(found_note != nullptr) {
            cout << "INFO: " << endl;
            Display::displayInfo(*found_note);
            cout << "CONTENT: " << endl;
            found_note->displayContent();
            cout << "CHANGES: " << endl;
            Display::displayChanges(*found_note);
        } else {
            cout << "No note found with ID: " << search_id << endl;
        }
    }
}

void NotepadApp::noteEditInfoFunc(shared_ptr<Directory> root_dir, Menu *& currentMenu) {
    int search_id = getInputId();
    if(search_id == 1) {
        currentMenu = &noteEditMenu;
        return;
    } else {
        auto found_note = root_dir->findNoteById(search_id);
        if(found_note != nullptr) {
            cout << "= EXAMPLE INPUT: \"et new_name\"\n= LIST OF COMMANDS: =\nen: Edit name\net: Edit tags\nec: Edit creation time" << endl;
            Display::displayInfo(*found_note);
            string command;
            while(getline(cin, command)) {
                try {
                    stringstream ss(command);
                    string sub_command;
                    ss >> sub_command;
                    if(sub_command == "exit") {
                        break;
                    } else if(sub_command == "en") {
                        string new_name;
                        ss >> new_name;
                        found_note->noteSetName(new_name);
                        Display::displayInfo(*found_note);
                    } else if(sub_command == "et") {
                        string tag;
                        vector<string> new_tags;
                        while (ss >> tag) {
                            new_tags.push_back(tag);
                        }
                        set<string> new_tags_set(new_tags.begin(), new_tags.end());
                        found_note->noteSetTags(new_tags_set);
                        Display::displayInfo(*found_note);
                    } else if(sub_command == "ec") {
                        string time_str;
                        getline(ss, time_str);
                                istringstream iss(time_str);
                                tm tm = {};
                        iss >> get_time(&tm, "%H:%M %d.%m.%Y");
                        time_t new_time = mktime(&tm);
                        found_note->noteSetCreationTime(new_time);
                        Display::displayInfo(*found_note);
                    } else {
                        cout << "Unknown command" << endl;
                    }
                } catch (const out_of_range & error) {
                    cout << "Error: " << error.what() << endl;
                }
            }
        }
    }
}

void NotepadApp::noteEditContentFunc(Directory * root_dir, Menu *& currentMenu, Menu * noteEditMenu) {
    int search_id = getInputId();
    if(search_id == 1) {
        currentMenu = noteEditMenu;
    } else {
        auto found_note = root_dir->findNoteById(search_id);
        if(found_note != nullptr) {
            found_note->editNoteFunc();
        } else {
            cout << "No note found with ID: " << search_id << endl;
        }
    }
}

void NotepadApp::noteDeleteFunc(Directory * root_dir, Menu *& currentMenu, Menu * mainMenu) {
    int search_id = getInputId();
    if(search_id == 1) {
        currentMenu = mainMenu;
    } else {
        auto found_note = root_dir->findNoteById(search_id);
        if(found_note != nullptr) {
            root_dir->removeNoteById(search_id);
            auto deleted_note = root_dir->findNoteById(search_id);
            if(deleted_note == nullptr) {
                cout << "Note was successfully deleted!" << endl;
            } else {
                cout << "Note was not deleted" << endl;
            }
            currentMenu = mainMenu;
        } else {
            cout << "No note found with ID: " << search_id << endl;
        }
    }
}

void NotepadApp::noteConvertToHTMLFunc(Directory * root_dir, Menu * & currentMenu, Menu * mainMenu) {
    int search_id = getInputId();
    if(search_id == 1) {
        currentMenu = mainMenu;
    } else {
        auto found_note = root_dir->findNoteById(search_id);
        if(found_note != nullptr) {
            string outfile_name;
            while(true) {
                cout << "Enter output file name: ";
                getline(cin, outfile_name);
                if(outfile_name.empty()) {
                    cout << "File name cannot be empty" << endl;
                    continue;
                }
                string invalid_symbols = "<>:\"/\\|?*";
                bool invalid_symbol = false;
                for(char symbol : invalid_symbols) {
                    if(outfile_name.find(symbol) != string::npos) {
                        invalid_symbol = true;
                        break;
                    }
                }
                if(invalid_symbol) {
                    cout << "File name contains invalid characters" << endl;
                    continue;
                }
                break;
            }
            ofstream outfile(outfile_name + ".html");
            if(outfile.is_open()) {                
                string html_content = found_note->toHTML();
                outfile << html_content;
                outfile.close();
                currentMenu = mainMenu;
            } else {
                cout << "Cannot create the file" << endl;
            }
        } else {
            cout << "No note found with ID: " << search_id << endl;
        }
    } 
}

void NotepadApp::noteImportFunc(Directory * root_dir, Menu *& currentMenu, Menu * mainMenu, set<string> & tags) {
    auto import_dir = root_dir->createSubdir("import");
    string filename;
    int file_type;
    cout << "Enter filename for import note without .html: ";
    cin.ignore();
    getline(cin, filename);
    if(filename == "1") {
        currentMenu = mainMenu;
        return;
    }
    filename = filename + ".html";
    ifstream file(filename);
    if(!file) {
        cout << "Invalid filename! File does not exist.\n";
        currentMenu = &noteImportNote;
        return;
    }
    file.close();
    cout << "Select the type of file to import (1 - Text, 2 - ToDoList, 3 - ShoppingList): ";
    cin >> file_type;
    if (file_type == 1) {
        currentMenu = mainMenu;
    }
    string tag_input;
    cout << "Enter tags separated by spaces: ";
    cin.ignore();
    getline(cin, tag_input);
    istringstream iss(tag_input);
    string first_input;
    iss >> first_input;
    if(tag_input == "1") {
        currentMenu = mainMenu;
    } else {
        tags.clear();
        tags.insert(first_input);
        string tag;
        bool input_error = false;
        while(iss >> tag) {
            if(!checkData(2, tag)) {
                cout << "A tag '" << tag << "' cannot be created!" << endl;
                input_error = true;
                break;
            }
            tags.insert(tag);
        }
        if(!input_error) {
            try {
                if(file_type == 1 || file_type == 2 || file_type == 3) {
                    import_dir->createNoteFromHTML(filename, tags, filename, file_type);
                    cout << "Note was successfully imported!" << endl;
                } else {
                    cout << "Invalid file type selection. Returning to import menu.\n";
                    currentMenu = &noteImportNote;
                    return;
                }
            } catch(const runtime_error & error) {
                if(string(error.what()) == "Invalid HTML structure") {
                    cout << "Invalid HTML structure. Returning to import menu.\n";
                    currentMenu = &noteImportNote;
                    return;
                } else {
                    throw;
                }
            }
            currentMenu = &noteImportNote;
        }
    }
    currentMenu = mainMenu;
}

NotepadApp::NotepadApp() :
    mainMenu{ "Welcome to notepad!", {
            { "Create note", [&]() { currentMenu = &noteCreateMenu; } },
            { "Search note", [&]() { currentMenu = &noteSearchMenu; } },
            { "Open note", [&]() { currentMenu = &noteOpenMenu; } },
            { "Edit note", [&]() { currentMenu = &noteEditMenu; } },
            { "Delete note", [&]() { currentMenu = &noteDeleteMenu; } },
            { "Export note to HTML", [&]() { currentMenu = &noteConvertToHTMLMenu; } },
            { "Import note", [&]() { currentMenu = &noteImportNote; } },
            { "Exit", [&]() { currentMenu = &sureMenu; } },
    }},
    noteCreateMenu{ "Choose a note type", {
        { "Text", [&]() { 
            new_note_type = 1; 
            currentMenu = &noteSetPath; 
        } },
        { "To do list", [&]() { 
            new_note_type = 2; 
            currentMenu = &noteSetPath; 
        } },
        { "Shopping list", [&]() { 
            new_note_type = 3; 
            currentMenu = &noteSetPath; 
        } },
        { "Go back", [&]() { 
            currentMenu = &mainMenu; 
        } },
    }},
    noteSetTags{ "Add tags (optional)", {
            { "Skip", [&]() { currentMenu = &noteSetName; } },
            { "Go back", [&]() { currentMenu = &noteCreateMenu; } },
    }},
    noteSetPath{ "Enter the path to the note", {
            { "Go back", [&]() { currentMenu = &noteCreateMenu; } },
    }},
    noteSetName{ "Enter a name for the new note", {
            { "Go back", [&]() { currentMenu = &noteSetTags; } },
    }},
    noteSearchMenu{ "Select a search criterion, = EXAMPLE INPUT: \"1 note_name\nsearch\"=", {
            { "Name", [&]() { } },
            { "Tag", [&]() { } },
            { "Creation time", [&]() { } },
            { "Directory", [&]() { } },
            { "Content", [&]() { } },
            { "Go back", [&]() { currentMenu = &mainMenu; } },
    }},
    noteOpenMenu{ "Enter a ID of the note", {
            { "Go back", [&]() { currentMenu = &mainMenu; } },
    }},
    noteEditMenu{ "Choose edit mode", {
            { "Edit info", [&]() { currentMenu = &noteEditInfo; } },
            { "Edit content", [&]() { currentMenu = &noteEditContent; } },
            { "Go back", [&]() { currentMenu = &mainMenu; } },
    }},
    noteEditContent{ "Content edit mode", {
            { "Go back", [&]() { currentMenu = &noteEditMenu; } },
    }},
    noteEditInfo{ "Info edit mode", {
        { "Go back", [&]() { currentMenu = &noteEditMenu; } },
    }},
    noteDeleteMenu{ "Enter a ID of the note", {
            { "Go back", [&]() { currentMenu = &mainMenu; } },
    }},
    sureMenu{ "Are you sure you want to quit the notepad?", {
            { "Yes", [&]() { currentMenu = nullptr; } },
            { "Go back", [&]() { currentMenu = &mainMenu; } },
    }},
    noteConvertToHTMLMenu{ "Enter a ID of the note", {
            { "Go back", [&]() { currentMenu = &mainMenu; } },
    }},
    noteImportNote{ "Enter a data of the note", {
            { "Go back", [&]() { currentMenu = &mainMenu; } },
    }},
    currentMenu(&mainMenu),
    root_dir(make_shared<Directory>("root")) {
        noteSearchMenu.setTitle("Enter info");
    }

void NotepadApp::run() {
    while(currentMenu && cin) {
        cout << endl;
        currentMenu->commandList();
        if(currentMenu == &noteSetPath) {
            noteSetPathFunc();
        } else if (currentMenu == &noteSetTags) {
            noteSetTagsFunc();
        } else if(currentMenu == &noteSetName) {
            noteSetNameFunc();
        } else if (currentMenu == &noteSearchMenu) {
            string command;
            int search_parameter = 0;
            string search_value;
            while (getline(cin, command)) {
                if (command == "6") {
                    currentMenu = &mainMenu;
                    break;
                } else if (command == "search") {
                    noteSearchFunc();
                    break;
                } else {
                    stringstream ss(command);
                    ss >> search_parameter;
                    getline(ss, search_value);
                    if (!search_value.empty() && search_value[0] == ' ') {
                        search_value.erase(0, 1);
                    }
                    if (search_parameter < 1 || search_parameter > 5) {
                        cout << "Search type must be between 1 and 5!" << endl;
                        continue;
                    }
                    if (search_value.empty()) {
                        cout << "Search value cannot be empty!" << endl;
                        continue;
                    }
                    SearchParameter param = {search_parameter, search_value};
                    search_parameters.push_back(param);
                }
            }
        } else if (currentMenu == &noteOpenMenu) {
            noteOpenFunc(root_dir.get(), currentMenu, &mainMenu);
        } else if(currentMenu == &noteEditContent) {
            noteEditContentFunc(root_dir.get(), currentMenu, &noteEditMenu);
        } else if(currentMenu == &noteEditInfo) {
            noteEditInfoFunc(root_dir, currentMenu);
        } else if(currentMenu == &noteDeleteMenu) {
            noteDeleteFunc(root_dir.get(), currentMenu, &mainMenu);
        } else if(currentMenu == &noteConvertToHTMLMenu) {
            noteConvertToHTMLFunc(root_dir.get(), currentMenu, &mainMenu);
        } else if(currentMenu == &noteImportNote) {
            noteImportFunc(root_dir.get(), currentMenu, &mainMenu, tags);
        } else {
            currentMenu->chooseCommand();
        }
    }
}