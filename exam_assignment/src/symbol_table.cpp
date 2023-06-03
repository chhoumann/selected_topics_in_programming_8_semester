#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <stdexcept>

template <typename T>
class SymbolTable {
private:
    std::unordered_map<std::string, T> table;

public:
    void store(const std::string &key, const T &value) {
        if (table.contains(key)) {
            throw std::runtime_error("Key already exists in symbol table");
        }

        table[key] = value;
    }

    T lookup(const std::string &key) const {
        if(table.contains(key)) {
            return table.at(key);
        } else {
            throw std::runtime_error("Key not found in symbol table");
        }
    }

    T update(const std::string &key, const T &value) {
        if (!table.contains(key)) {
            throw std::runtime_error("Key not found in symbol table");
        }

        table[key] = value;
        return value;
    }
};

#endif
