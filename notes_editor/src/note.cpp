#include "note.h"

/* This class represents a Note object.
 * It contains an ID, name, set of tags, and a creation time.
 * The class provides methods to get and set these properties 
 */
int Note::s_nextId = 2;

Note::Note(const string & name, const set<string> & tags) : m_name(name), m_tags(tags), m_creation_time(time(nullptr)), m_id(s_nextId++) {}

string Note::noteGetName() const {
    return m_name;
}

void Note::noteSetName(const string & name) {
    m_name = name;
}

set<string> Note::noteGetTags() const {
    return m_tags;
}

void Note::noteSetTags(const set<string> & tags) {
    m_tags = tags;
}

time_t Note::noteGetCreationTime() const {
    return m_creation_time;
}

void Note::noteSetCreationTime(const time_t & creation_date) {
    m_creation_time = creation_date;
}

int Note::getId() const {
    return m_id;
}

vector<string> Note::getChanges() const {
    return changes;
}