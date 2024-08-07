#ifndef DISPLAY_H
#define DISPLAY_H

#include <iomanip>
#include "note.h"


/** Display class contains methods for displaying information about Note objects. */
class Display {
public:
    /** 
     * @brief Displays general info about a Note.
     * @param[in] note Reference to a Note object.
     */
    static void displayInfo(const Note& note);
    /** 
     * @brief Displays changes made to a Note.
     * @param[in] note Reference to a Note object.
     */
    static void displayChanges(const Note& note);
};

#endif  // DISPLAY_H