#include <iostream>
#include <vector>
#include <algorithm>

#ifndef DATABASE_H
#define DATABASE_H

#include <string>

enum Type { INT, DOUBLE, STRING, ARRAY };

struct Array {
    int size;
    Type type;
    void *items;
};

struct Entry {
    Type type;
    std::string key;
    void *value;
};

struct Database {
    std::vector<Entry*> entries;
};

Entry *create(Type type, std::string key, void *value) {
    Entry *entry = new Entry;
    entry->type = type;
    entry->key = key;
    entry->value = value;
    return entry;
}

void init(Database &database) {
    database.entries.clear();
}

void add(Database &database, Entry *entry) {
    database.entries.push_back(entry);
}

Entry *get(Database &database, std::string &key) {
    auto it = std::find_if(database.entries.begin(), database.entries.end(), [&key](Entry *entry) {
        return entry->key == key;
    });
    if (it != database.entries.end()) {
        return *it;
    } else {
        return nullptr;
    }
}

void remove(Database &database, std::string &key) {
    auto it = std::remove_if(database.entries.begin(), database.entries.end(), [&key](Entry *entry) {
        return entry->key == key;
    });
    database.entries.erase(it, database.entries.end());
}

void destroy(Database &database) {
    for (auto entry : database.entries) {
        delete entry;
    }
}

#endif
int main() {
    Database db;
    init(db);

    std::string command;
    while (true) {
        std::cout << "Command(list, add, get, del, exit): ";
        std::cin >> command;

        if (command == "list") {
            for (auto entry : db.entries) {
                std::cout << entry->key << ": ";
                if (entry->type == INT) {
                    std::cout << *(int*)(entry->value);
                } 
                else if (entry->type == DOUBLE) {
                    std::cout << *(double*)(entry->value);
                } 
                else if (entry->type == STRING) {
                    std::cout << *(std::string*)(entry->value);
                } 
                else if (entry->type == ARRAY) {
                    std::cout << "[Array]";
                }
                std::cout << std::endl;
            }
        } 
        else if (command == "add") {
            std::string key;
            std::string type_str;
            std::cout << "key: ";
            std::cin >> key;
            std::cout << "type(int, double, string, array): ";
            std::cin >> key >> type_str;
            Type type;
            if (type_str == "int") {
                type = INT;
                int *value = new int;
                std::cout << "value: ";
                std::cin >> *value;
                add(db, create(type, key, value));
            } 
            else if (type_str == "double") {
                type = DOUBLE;
                double *value = new double;
                std::cout << "value: ";
                std::cin >> *value;
                add(db, create(type, key, value));
            } 
            else if (type_str == "string") {
                type = STRING;
                std::string *value = new std::string;
                std::cout << "value: ";
                std::cin.ignore(); 
                std::getline(std::cin, *value);
                add(db, create(type, key, value));
            } 
            else {
                std::cerr << "Invalid type." << std::endl;
            }
        } 
        else if (command == "get") {
            std::string key;
            std::cout << "key: ";
            std::cin >> key;
            Entry *entry = get(db, key);
            if (entry) {
                std::cout << entry->key << ": ";
                if (entry->type == INT) {
                    std::cout << *(int*)(entry->value);
                } 
                else if (entry->type == DOUBLE) {
                    std::cout << *(double*)(entry->value);
                } 
                else if (entry->type == STRING) {
                    std::cout << *(std::string*)(entry->value);
                } 
                else if (entry->type == ARRAY) {
                    std::cout << "[Array]";
                }
                std::cout << std::endl;
            } 
            else {
                std::cout << "Entry not found" << std::endl;
            }
        } 
        else if (command == "del") {
            std::string key;
            std::cout << "key: ";
            std::cin >> key;
            remove(db, key);
        } 
        else if (command == "exit") {
            destroy(db);
            std::cout << "Exiting program" << std::endl;
            break;
        } 
        else {
            std::cout << "Invalid command" << std::endl;
        }
    }

    return 0;
}