#include <iostream>
#include <string>
#include <set>
#include <map>

using namespace std;

class BasicErrorsException : public exception
{
private:
    string str;
public:
    BasicErrorsException(string mes) : str(mes) {}

    virtual const char* what() const 
    {
        return str.c_str(); 
    }
};


//функции map
template <class KeyType, class ValType>
KeyType valueSearch(const map<KeyType, ValType>& map, const ValType& value)
{
    for (auto it = map.begin(); it != map.end(); it++)
    {
        if (it->second == value)
            return it->first;
    }

    throw BasicErrorsException("Error: Value " + to_string(value) + " does not exist.");
}

template <class KeyType, class ValType>
ValType keySearch(const map<KeyType, ValType>& map, const KeyType& key)
{
    auto it = map.find(key);
    if (it != map.end()) 
        return it->second;
    
    throw BasicErrorsException("Error: Key '" + key + "' does not exist.");
}

template <class KeyType, class ValType>
void print(const map<KeyType, ValType>& map)
{
    for (auto it = map.begin(); it != map.end(); it++)
        cout << it->first << ": " << it->second << "\n";
}

bool thresholdFilter(int value)
{
    int threshold = 2000000;
    return value > threshold;
}

template <class KeyType, class ValType>
map<KeyType, ValType> filter(const map<KeyType, ValType>& map, bool(*ptr)(ValType))
{
    std::map<KeyType, ValType> res;

    for (auto it = map.begin(); it != map.end(); it++)
    {
        if (ptr(it->second))
            res[it->first] = it->second;
    }

    return res;
}

template <class KeyType, class ValType>
void add(map<KeyType, ValType>& map, const KeyType& key, const ValType& value)
{
    if (map.find(key) == map.end())
    {
        map[key] = value;
        return;
    }
    throw BasicErrorsException(key + " already exists.");
}

template <class KeyType, class ValType>
set<ValType> getValues(const map<KeyType, ValType>& map)
{
    set<ValType> res;
    for (auto it = map.begin(); it != map.end(); it++)
        res.insert(res.end(), it->second);
    return res;
}

void basic_map()
{
    try
    {
        map<string, int> map;
        add<string, int>(map, "Hyundai", 5000000);
        add<string, int>(map, "Chery", 1500000);
        add<string, int>(map, "Audi", 1300000);
        add<string, int>(map, "Infiniti", 3000000);
        add<string, int>(map, "Opel", 2100000);

        cout << "Map test:\n";
        print(map);

        cout << "\nFiltered: \n";
        auto filt_map = filter(map, thresholdFilter);
        print(filt_map);

        cout << "\nValue (key = 'Hyundai'): " << keySearch<string, int>(map, "Hyundai") << "\n";
        cout << "Key (value = 3000000): " << valueSearch<string, int>(map, 3000000) << "\n";

        cout << "All values: ";
        auto val = getValues(map);
        for (auto it = val.begin(); it != val.end(); it++)
        {
            cout << *it << " ";
        }

        cout << "\n\nAdding a value: ";
        add<string, int>(filt_map, "Hyundai", 5000000);
    }
    catch (BasicErrorsException e)
    {
        cout << e.what();
    }
}




//функции multimap
template <class KeyType, class ValType>
KeyType valueSearch(const multimap<KeyType, ValType>& map, const ValType& value)
{
    for (auto it = map.begin(); it != map.end(); it++)
    {
        if (it->second == value)
            return it->first;
    }
    
    throw BasicErrorsException("Error: Value " + to_string(value) + " does not exist.");
}

template <class KeyType, class ValType>
set<ValType> keySearch(const multimap<KeyType, ValType>& map, const KeyType& key)
{
    set<ValType> res;

    for (auto it1 = map.begin(); it1 != map.end(); it1++)
    {
        if (it1->first == key)
            res.insert(res.end(), it1->second);
    }

    if(res.empty())
        throw BasicErrorsException("Error: Key '" + key + "' does not exist.");
    return res;
}

template <class KeyType, class ValType>
void print(const multimap<KeyType, ValType>& map)
{
    for (auto it = map.begin(); it != map.end(); it++)
        cout << it->first << ": " << it->second << "\n";
}

template <class KeyType, class ValType>
multimap<KeyType, ValType> filter(const multimap<KeyType, ValType>& map, bool(*ptr)(ValType))
{
    multimap<KeyType, ValType> res;
    for (auto it = map.begin(); it != map.end(); it++)
    {
        if (ptr(it->second))
            res.insert({ it->first, it->second });
    }
    return res;
}

template <class KeyType, class ValType>
void add(multimap<KeyType, ValType>& map, const KeyType& key, const ValType& value)
{
    map.insert({ key, value });
    return;
}

template <class KeyType, class ValType>
set<ValType> getValues(const multimap<KeyType, ValType>& map)
{
    set<ValType> res;
    for (auto it = map.begin(); it != map.end(); it++)
        res.insert(res.end(), it->second);
    return res;
}

void multi_map()
{
    try
    {
        multimap<string, int> multi_map;
        add<string, int>(multi_map, "Hyundai", 5000000);
        add<string, int>(multi_map, "Hyundai", 3500000);
        add<string, int>(multi_map, "Chery", 1500000);
        add<string, int>(multi_map, "Audi", 1300000);
        add<string, int>(multi_map, "Infiniti", 3000000);
        add<string, int>(multi_map, "Opel", 2100000);

        cout << "Multimap test:\n";
        print(multi_map);

        auto filt_map = filter(multi_map, thresholdFilter);
        cout << "\nFiltered: \n";
        print(filt_map);

        auto val1 = keySearch<string, int>(multi_map, "Hyundai");
        cout << "\nValue (key = 'Hyundai'): ";
        for (auto val_it = val1.begin(); val_it != val1.end(); val_it++)
            cout << *val_it << " ";

        cout << "\nKey (value = 3000000): " << valueSearch<string, int>(multi_map, 3000000) << "\n";

        cout << "All values: ";
        auto val2 = getValues(multi_map);
        for (auto it = val2.begin(); it != val2.end(); it++)
            cout << *it << " ";

        cout << "\n\nAdding a value: ";
        add<string, int>(filt_map, "Mercedes", 5000000);
        cout << "Successfully added.\n\n";

        cout << "Key (value = 3000000): " << valueSearch<string, int>(multi_map, 2340000) << "\n";
    }
    catch (BasicErrorsException e)
    {
        cout << e.what();
    }
}



int main()
{
    basic_map();
    cout << endl << endl;
    multi_map();
}