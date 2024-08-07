#ifndef SHOPPINGLIST_H
#define SHOPPINGLIST_H

#include <string>
#include <fstream>
#include <vector>
#include <set>
#include "note.h"

/** 
 * ShoppingList class derived from Note, represents a shopping list note.
 * Contains a list of items. Each item is defined by its price, quantity, and name.
 */
class ShoppingList: public Note {
public:
    /** 
     * @brief Struct representing an item, which consists of a price, quantity, and name.
     */
    struct Item {
        /**
         * Constructor for Item
         * @param price Price of the item
         * @param quantity Quantity of the item
         * @param name Name of the item
         */
        Item(double price, int quantity, const string & name);

        /**
         * Price of the item
         */
        double price;
        /**
         * Quantity of the item
         */
        int quantity;
        /**
         * Name of the item
         */
        string name;
    };

    /** 
     * @brief Constructor for ShoppingList
     * @param name Name of the ShoppingList
     * @param tags Set of tags associated with the ShoppingList
     */
    ShoppingList(const string & name, const set<string> & tags);

    /**
     * @brief Saves the state of the ShoppingList with the command provided.
     */
    void saveState(const string & command);
    /**
     * @brief Adds an item to the shopping list.
     */
    void addItem(const Item & item);
    /**
     * @brief Edits an item at the specified index in the shopping list.
     */
    void editItem(size_t index, double new_price, int new_quantity,  const string & new_name);
    /**
     * @brief Deletes an item at the specified index in the shopping list.
     */
    void deleteItem(size_t index);
    /**
     * @brief Returns the items in the shopping list.
     */
    vector<Item> getItems() const;
    /**
     * @brief Calculates and returns the total price of all items in the shopping list.
     */
    double getTotalPrice() const;
    /**
     * @brief Searches for content in the shopping list that matches the search string.
     */
    vector<string> findContent(const string & search_str) const override;
    /** 
     * @brief Modifies a ShoppingList note.
     * This function should be overridden with specific implementation in derived classes. 
     */
    void editNoteFunc() override;
    /** 
     * @brief Converts the current ShoppingList note into a string of HTML format.
     * @returns HTML string representing the current ShoppingList note.
     */
    string toHTML() const override;
    /** 
     * @brief Imports a ShoppingList note from an HTML file.
     * @param filename The name of the HTML file to import.
     */
    void importHTMLToNote(const string & filename) override;
    /**
     * @brief Displays the content of the ShoppingList note.
     * This method prints the tasks in the ShoppingList along with their associated times.
     */
    void displayContent() const override;
    friend class Import;
private:
    /**
     * vector of Item objects of the Shopping note 
     */
    vector<Item> items;
};

#endif