#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <algorithm>

using std::vector, std::string, std::cout;

class cuckooTable {
private:
    vector<std::optional<string> > table1;
    vector<std::optional<string> > table2;

    size_t numOfKeys;
    double maxOccupancy;
    double resizeFactor;
    int max_loops;

    size_t h1(const string& key) const {
        return std::hash<string>{}(key) % table1.size();
    }

    size_t h2(const string& key) const {
        return std::hash<string>{}(key) % table2.size();
    }

    bool needsResizeWhenAddingAkey() const {
        return numOfKeys / static_cast<double>(table1.size() + table2.size()) > maxOccupancy;
    }

    void resize(){
        size_t newSize1 = static_cast<size_t>(table1.size() * resizeFactor);
        size_t newSize2 = static_cast<size_t>(table2.size() * resizeFactor);

        auto oldTable1 = std::move(table1);
        auto oldTable2 = std::move(table2);

        table1.assign(newSize1, std::nullopt);
        table2.assign(newSize2, std::nullopt);

        numOfKeys = 0;

        for(const auto& item : oldTable1){
            if(item.has_value()){
                insert(item.value());
            }
        }
        for(const auto& item : oldTable2){
            if(item.has_value()){
                insert(item.value());
            }
        }
    }

public:
    cuckooTable(size_t size1, size_t size2, double maxOcc, double resizeF, int mLoops = 5) :
        table1(size1, std::nullopt), table2(size2, std::nullopt),
        numOfKeys(0), maxOccupancy(maxOcc), resizeFactor(resizeF), max_loops(mLoops) {}

    bool lookup(const string& key) const {
        size_t idx1 = h1(key);
        if(table1[idx1].has_value() and table1[idx1].value() == key){
            return true;
        }

        size_t idx2 = h2(key);
        if(table2[idx2].has_value() and table2[idx2].value() == key){
            return true;
        }

        return false;
    }

    void insert(string x){
        if( lookup(x) ){
            return;
        }

        numOfKeys += 1;
        if( needsResizeWhenAddingAkey() ){
            resize();
        }

        for(int i = 0; i < max_loops; i++){
            size_t idx1 = h1(x);
            if( not table1[idx1].has_value() ){
                table1[idx1] = x;
                return;
            }

            std::swap(x, table1[idx1].value());

            size_t idx2 = h2(x);
            if( not table2[idx2].has_value() ){
                table2[idx2] = x;
                return;
            }

            std::swap(x, table2[idx2].value());
        }

        numOfKeys -= 1;
        resize();
        insert(x);
    }

    bool remove(const string& key){
        size_t idx1 = h1(key);
        if(table1[idx1].has_value() and table1[idx1].value() == key){
            table1[idx1] = std::nullopt;
            numOfKeys -= 1;
            return true;
        }

        size_t idx2 = h2(key);
        if(table2[idx2].has_value() and table2[idx2].value() == key){
            table2[idx2] = std::nullopt;
            numOfKeys -= 1;
            return true;
        }

        return false;
    }

    void print() const {
        cout << "Table1:\n";
        for(size_t i = 0; i < table1.size(); i++){
            if(table1[i].has_value()){
                cout << i << ": " << table1[i].value() << " ";
            }
        }
        cout << "\nTable2:\n";
        for(size_t i = 0; i < table2.size(); i++){
            if(table2[i].has_value()){
                cout << i << ": " << table2[i].value() << " ";
            }
        }
        cout << "\n";
    }

};


int main(){
    cuckooTable table(10, 11, 0.5, 1.5, 5);

    const auto inputs = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",};
    for(const auto& item : inputs){
        table.insert(item);
    }
    table.print();
    cout << "\n";

    const auto inputs2 = { "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen",
    "seventeen", "eighteen", "nineteen", "twenty",
    "twenty-one", "twenty-two", "twenty-three", "twenty-four", "twenty-five",
    "twenty-six", "twenty-seven", "twenty-eight", "twenty-nine",};

    for(const auto& item : inputs2){
        table.insert(item);
    }
    table.print();
    cout << "\n";

    const auto removals = { "twenty-one", "twenty-two", "twenty-three", "twenty-four", "twenty-five", 
        "twenty-six", "twenty-seven", "twenty-eight", "twenty-nine",
        "aardvark", "Bene Gesserit", "crane", "dog", "element"};

    for(const auto& item : removals){
        table.remove(item);
    }
    table.print();
    cout << "\n";


    return 0;
}