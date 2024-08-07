#include "text.h"
#include "todolist.h"
#include "shoppinglist.h"

/* This class represents a directory in a file system. 
 * It contains directories (subdirectories) and notes. This class provides methods to manage (add, remove, search) directories and notes 
 */
class Directory;

shared_ptr<Directory> Directory::createPath(const string & path, const shared_ptr<Directory> & root) {
    shared_ptr<Directory> current_dir = root;
    stringstream ss(path);
    string dir_name;
    while(getline(ss, dir_name, '/')) {
        auto existing_subdir = current_dir->findSubdir(dir_name);
        if(existing_subdir) {
            current_dir = existing_subdir;
        } else {
            auto new_subdir = make_shared<Directory>(dir_name);
            current_dir->addSubdir(new_subdir);
            current_dir = new_subdir;
        }
    }
    return current_dir;
}

Directory::Directory(const string &name) : m_name(name) {}

void Directory::addSubdir(const shared_ptr<Directory> & subdir) {
    m_subdirs.push_back(subdir);
}

shared_ptr<Directory> Directory::createSubdir(const string & name) {
    auto new_subdir = make_shared<Directory>(name);
    addSubdir(new_subdir);
    return new_subdir;
}


void Directory::addNote(const shared_ptr<Note> & note) {
    m_notes[note->getId()] = note;
}

void Directory::removeNoteById(int id) {
    for(auto & iter : m_subdirs) {
        iter->removeNoteById(id);
    }
    auto note_iter = m_notes.find(id);
    if(note_iter != m_notes.end()) {
        m_notes.erase(note_iter);
    }
}

vector<shared_ptr<Note>> Directory::findNotes(const string & search_note_name) const {
    vector<shared_ptr<Note>> result;
    for(const auto & iter : m_subdirs) {
        auto iter_result = iter->findNotes(search_note_name);
        result.insert(result.end(), iter_result.begin(), iter_result.end());
    }
    for(const auto & iter2 : m_notes) {
        if(iter2.second->noteGetName().find(search_note_name) != string::npos) {
            result.push_back(iter2.second);
        }
    }
    return result;
}

string Directory::getName() const {
    return m_name;
}

shared_ptr<Directory> Directory::findSubdir(const string & subdir_name) const {
    for(const auto & iter : m_subdirs) {
        if (iter->m_name == subdir_name) {
            return iter;
        }
    }
    return nullptr;
}

const vector<shared_ptr<Directory>>& Directory::getSubdirectories() const {
    return m_subdirs;
}

vector<shared_ptr<Note>> Directory::getNotes() const {
    vector<shared_ptr<Note>> notes;
    for(const auto & iter : m_notes) {
        notes.push_back(iter.second);
    }
    return notes;
}

vector<shared_ptr<Note>> Directory::getNotesInPath(const string& path) const {
    vector<shared_ptr<Note>> found_notes;
    if(path.empty()) {
        found_notes = getNotes();
        for(const auto & iter : m_subdirs) {
            vector<shared_ptr<Note>> subdir_notes = iter->getNotesInPath("");
            found_notes.insert(found_notes.end(), subdir_notes.begin(), subdir_notes.end());
        }
        } else {
            stringstream ss(path);
            string dir_name;
            getline(ss, dir_name, '/');
            string remain;
            if(ss.good()) {
                remain = ss.str().substr(dir_name.size() + 1);
            } else {
                remain = ss.str().substr(dir_name.size());
            }
            for(const auto & iter2 : m_subdirs) {
                if(iter2->getName() == dir_name) {
                    vector<shared_ptr<Note>> subdir_notes = iter2->getNotesInPath(remain);
                    found_notes.insert(found_notes.end(), subdir_notes.begin(), subdir_notes.end());
                }
                vector<shared_ptr<Note>> subdir_notes = iter2->getNotesInPath(path);
                found_notes.insert(found_notes.end(), subdir_notes.begin(), subdir_notes.end());
            }
        }
    return found_notes;
}

vector<shared_ptr<Note>> Directory::findNotesByTag(const string &tag) const {
    vector<shared_ptr<Note>> found_notes;
    for(const auto & iter : m_notes) {
        if(iter.second->noteGetTags().count(tag) > 0) {
            found_notes.push_back(iter.second);
        }
    }
    for(const auto & iter3 : m_subdirs) {
        auto subdir_result = iter3->findNotesByTag(tag);
        found_notes.insert(found_notes.end(), subdir_result.begin(), subdir_result.end());
    }
    return found_notes;
}

vector<shared_ptr<Note>> Directory::findNotesByCreationDate(const string &creation_date) const {
    vector<shared_ptr<Note>> found_notes;
    for(const auto &iter : m_notes) {
        if (formatTime(iter.second->noteGetCreationTime()) == creation_date) {
            found_notes.push_back(iter.second);
        }
    }
    for(const auto & iter2 : m_subdirs) {
        auto subdir_result = iter2->findNotesByCreationDate(creation_date);
        found_notes.insert(found_notes.end(), subdir_result.begin(), subdir_result.end());
    }
    return found_notes;
}

shared_ptr<Note> Directory::findNoteById(int id) const {
    auto it = m_notes.find(id);
    if(it != m_notes.end()) {
        return it->second;
    }
    for(const auto & iter : m_subdirs) {
        auto iter_found = iter->findNoteById(id);
        if(iter_found) {
            return iter_found;
        }
    }
    return nullptr;
}

vector<shared_ptr<Note>> Directory::findNotesByContent(const string & search_str) const {
    vector<shared_ptr<Note>> found_notes;
    for(const auto & iter : m_notes) {
        if(!iter.second->findContent(search_str).empty()) {
            found_notes.push_back(iter.second);
        }
    }
    for(const auto & iter3 : m_subdirs) {
        auto subdir_result = iter3->findNotesByContent(search_str);
        found_notes.insert(found_notes.end(), subdir_result.begin(), subdir_result.end());
    }
    return found_notes;
}

shared_ptr<Note> Directory::createNoteFromHTML(const string &name, const set<string> & tags, const string & filename, const int & type) {
    shared_ptr<Note> note = nullptr;
    if(type == 1) {
        note = make_shared<Text>(name, tags);
        note->importHTMLToNote(filename);
    } else if(type == 2) {
        note = make_shared<ToDoList>(name, tags);
        note->importHTMLToNote(filename);
    } else if(type == 3) {
        note = make_shared<ShoppingList>(name, tags);
        note->importHTMLToNote(filename);
    } else {
        throw invalid_argument("Invalid note type.");
    }
    addNote(note);
    return note;
}