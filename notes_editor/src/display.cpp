#include "display.h"
#include "text.h"
#include "todolist.h"
#include "shoppinglist.h"
#include <iostream>

using namespace std;

void Display::displayInfo(const Note & note) {
    cout << "---------------------------------------------------------------" << endl;
    cout << "Note name: " << note.noteGetName() << endl;
    cout << "Tags: ";
    for(const auto & tag : note.noteGetTags()) {
        if(!tag.empty()) {
            cout << "#" << tag << " ";
        }
    }
    cout << endl;
    cout << "Creation time: " << formatTime(note.noteGetCreationTime()) << endl;
    cout << "ID: " << note.getId() << endl;
    cout << "---------------------------------------------------------------" << endl;
}

void Display::displayChanges(const Note& note) {
    cout << "---------------------------------------------------------------" << endl;
    for (const auto & change : note.getChanges()) {
        cout << change << endl;
    }
    cout << "---------------------------------------------------------------" << endl;
}