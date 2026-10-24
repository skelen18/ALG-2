#include<vector>
#include<optional>
#include<string>
#include<algorithm> // std::find
#include<iostream>

using std::vector, std::string;

// naive hash table with chaining
class naiveHashTable{
    private:
    // we will only consider growing the table, no shrinking
    vector<vector<string> > table; // outer vector is the table, inner vector is the bucket
    size_t numOfKeys;
    double maxOccupancy;  // ratio of items to outer table size
    double resizeFactor;  // how much to resize the outer table

    size_t computeIndex(const string& key, const size_t modFactor) const {
        return std::hash<string>{}(key) % modFactor;
    }

    // super naive check if the table is too full, more realistic would take into account bucket sizes
    bool needsResizeWhenAddingAkey() const {
        return numOfKeys / static_cast<double>(table.size()) > maxOccupancy;
    }

    void resize(){ // 🦛
        size_t newSize = static_cast<size_t>(table.size() * resizeFactor);
        auto oldTable = std::move(table); // save the data from the old table
        table.assign(newSize, {}); // prepare new table with given size and empty buckets

	    // set the number of keys to zero, insert function increments it
	    numOfKeys = 0;
        // rehash all data after changing the size of the table
        for(const auto& bucket : oldTable){
            for(const auto& item : bucket){
                insert(item);
            }
        }
    }

    public:

    naiveHashTable(size_t size, double maxOcc, double resizeF) :
        table(size), numOfKeys(0), maxOccupancy(maxOcc), resizeFactor(resizeF) {}

    bool lookUp(const string& key) const { // 👀
        size_t index = computeIndex(key, table.size()); // returns the iterator to the first element in the bucket that matches the key, or the end of the bucket if not found
        return std::find(table[index].begin(), table[index].end(), key) != table[index].end(); // if the iterator is not equal to the end of the bucket, the key was found
    }

    bool insert(const string& key){ // 🦓
        if( lookUp(key) ){ return false;}
        numOfKeys += 1;
        // note we are resizing before adding the new key
        if( needsResizeWhenAddingAkey() ){
            resize();
        }
        size_t index = computeIndex(key, table.size());
        table[index].push_back(key);
        return true;
    }

    bool remove(const string& key){
        if( not lookUp(key) ){ return false;}

        size_t index = computeIndex(key, table.size()); // item is in the bucket on position index
        auto key_it = std::find(table[index].begin(), table[index].end(), key); // find the iterator to the key in the bucket
        
        // table[index].erase(key_it);
        // swap and delete from back is faster than erase, order in bucket does not matter
        std::swap(table[index].back(), *key_it);
        table[index].pop_back();

        numOfKeys -= 1;
        return true;
    }

    void print() const {
        for(size_t bucket = 0; bucket < table.size(); bucket++){
            std::cout << bucket << ": ";
            for(const auto& item : table[bucket]){
                std::cout << item << " ";
            }
            std::cout << "\n";
        }
        return;
    }
};

class linearProbingTable{
    private:

    enum class State{
        EMPTY,
        DELETED,
        OCCUPIED};

    struct Entry{
        string key;
        State state;
    };

    vector<Entry> table;
    size_t numOfKeys;
    double maxOccupancy;  // ratio of items to the table size
    double resizeFactor;  // how much to resize the table
    // TODO? max number of DELETED slots before we clean up the table

    bool needsResizeWhenAddingAkey() const {
        // TODO: resize based on too many DELETED in the table
        // Note that we could also count with numOfKeys + 1, depending on our design choice
        // we might also consider numOfKeys + numOfDeleted
        return numOfKeys / static_cast<double>(table.size()) > maxOccupancy;
    }

    void resize(){
        size_t newSize = static_cast<size_t>(table.size() * resizeFactor);

        vector<Entry> oldTable = std::move(table); // save the data from the old table
        table.assign(newSize, Entry{"", State::EMPTY}); // Re-initialize the table with new size
        numOfKeys = 0;

        // Re-hash all previously occupied elements into the new, larger table.
        // Note, that this also implcitly cleans up all DELETED slots.
        for(const auto& entry : oldTable){
            if(entry.state == State::OCCUPIED){
                insert(entry.key); // use the insert logic for re-hashing
            }
        }
    }

    size_t computeIndex(const string& key, size_t size) const {
        return std::hash<string>{}(key) % size;
    }

    // simple idea to clean up some of the DELETED slots
    void eliminateTrailingDeleted(size_t deletedIndex) {
        const size_t size = table.size();
        size_t endIndex = deletedIndex;

        // look forward to find if the tombstone is needed
        for(size_t scanned = 0; scanned < size; ++scanned){
            // probing leads to occupied slot, tombstone is needed, nothing to do
            if(table[endIndex].state == State::OCCUPIED){
                return;
            }
            // probing leads to empty slot, tombstone is not needed, we can clean up the DELETED slots leading to the empty slot
            if(table[endIndex].state == State::EMPTY){
                // clean up the DELETED slots leading to the empty slot
                size_t index = (endIndex + size - 1) % size; // step back on the last DELTED
                while(table[index].state == State::DELETED){
                    table[index].state = State::EMPTY; // mark as empty
                    index = (index + size - 1) % size; // step back
                }
                return;
            }
            endIndex = (endIndex + 1) % size;
        }

        // getting here means that all slots are deleted
        for(auto& entry : table){
            entry.state = State::EMPTY;
        }
    }

    public:

    linearProbingTable(size_t size, double resizeF, double maxOcc=0.7) :
        numOfKeys(0), maxOccupancy(maxOcc), resizeFactor(resizeF) {
            table.assign(size, Entry{"", State::EMPTY});
        }

    bool lookUp(const string& key) const {  // 👀
        size_t initialIndex = computeIndex(key, table.size()); // index of the first slot to check
        // linear probing
        for(size_t i = 0; i < table.size(); i++){
            size_t currentIndex = (initialIndex + i) % table.size(); // wrap/warp around the table
            const Entry& entry = table.at(currentIndex); // note the fancy Entry& instead of Entry to avoid copying the Entry object

            if(entry.state == State::EMPTY){
                return false;
            }
            if(entry.state == State::OCCUPIED and entry.key == key){
                return true;
            }
            // else we continue on our merry way, stepping on DELETED means we keep looking
        }
        return false; // probed the whole table and did not found key
    }

    bool insert(const string& key){
        if(needsResizeWhenAddingAkey()){
            resize();
        }

        size_t initialIndex = computeIndex(key, table.size());
        std::optional<size_t> insertIndex = std::nullopt; // optional istead of -1 or extra bool

        // linear probing
        for(size_t i = 0; i < table.size(); i++){
            size_t currentIndex = (initialIndex + i) % table.size();
            Entry& entry = table.at(currentIndex);

            if(entry.state == State::OCCUPIED and entry.key == key){
                return false; // key is already there, nothing to insert
            }

            if(entry.state == State::DELETED){
                // potential place for insertion, if the key is in fact not somewhere farther away
                // we save the position as candidate for insertion
                if( not insertIndex.has_value() ){ // demo for .has_value()
                    insertIndex = currentIndex;  // asignment to optional
                }
            }

            if(entry.state == State::EMPTY){
                // empty slot means either free estate to insert into, or need to go back to the first found insertIndex at DELETED position
                // insertion is deff happening
                if( not insertIndex.has_value() ){
                    insertIndex = currentIndex;
                }
                table[insertIndex.value()].key = key;
                table[insertIndex.value()].state = State::OCCUPIED;
                numOfKeys += 1;
                return true;
            }
        }

        // getting here means there was no EMPTY slot, we can safely insert to the first found DELETED slot
        if( insertIndex.has_value() ){
            table[insertIndex.value()].key = key;
            table[insertIndex.value()].state = State::OCCUPIED;
            numOfKeys += 1;
            return true;
        }
        std::cerr << "This should never happen\n"; // 😿
        return false;
    }

    bool remove(const string& key){
        size_t initialIndex = computeIndex(key, table.size());
        // linear probing
        for (size_t i = 0; i < table.size(); ++i) {
            size_t currentIndex = (initialIndex + i) % table.size();
            Entry& entry = table[currentIndex];

            if (entry.state == State::EMPTY) {
                return false; // Key not in table
            }
            if (entry.state == State::OCCUPIED && entry.key == key) {
                entry.state = State::DELETED; // Mark as deleted, don't set to empty
                entry.key.clear();     // "clear" the string
                numOfKeys -= 1;
                // eliminateTrailingDeleted(currentIndex);
                return true;
            }
        }
        return false; // Key not found after full table pass
    }

    void print() const { // 🦓
        std::cout << "Table with size: " << table.size() << " and " << numOfKeys << "keys.\n";
        for(size_t i = 0; i < table.size(); i++){
            std::cout << i << ": ";
            switch (table[i].state)
            {
            case State::EMPTY: std::cout << "empty\n"; break;
            case State::DELETED: std::cout << "deleted\n"; break;
            case State::OCCUPIED: std::cout << table[i].key << "\n"; break;
            default:
                std::cout << "what?\n";
                break;
            }
        }
    }

};


int main(){
    // generated demo
    linearProbingTable lp_table(10, 0.7, 2.0);

    std::cout << "Inserting initial keys...\n";
    lp_table.insert("apple");
    lp_table.insert("banana"); // collides with apple
    lp_table.insert("cherry");
    lp_table.insert("date");
    lp_table.insert("grape");
    lp_table.insert("fig");
    lp_table.insert("lemon"); // 7th element, triggers resize (7+1)/10 > 0.7
    lp_table.print();

    std::cout << "Looking up 'banana': " << (lp_table.lookUp("banana") ? "Found" : "Not Found") << std::endl;
    std::cout << "Looking up 'mango': " << (lp_table.lookUp("mango") ? "Found" : "Not Found") << std::endl;

    std::cout << "\nRemoving 'banana'...\n";
    lp_table.remove("banana");
    lp_table.print();

    std::cout << "Looking up 'cherry' (should still be findable after 'banana' deletion): "
              << (lp_table.lookUp("cherry") ? "Found" : "Not Found") << std::endl;

    std::cout << "\nInserting 'mango' (should fill the DELETED spot)...\n";
    lp_table.insert("mango");
    lp_table.print();

    return 0;
}
