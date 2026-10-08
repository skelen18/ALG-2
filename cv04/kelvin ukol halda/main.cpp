#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <string>

using std::vector;

class Heap{
private:
    size_t n;
    vector<int> data;
    
    size_t parent(size_t index) const {
        if(index == 0){ return 0; }
        return (index - 1) / n;
    }

    void heapify(size_t index){
        size_t largest = index;

        for(size_t k = 1; k <= n; k++){
            size_t child = n * index + k;
            if(child >= data.size()){
                break;
            }

            if(data[child] > data[largest]){
                largest = child;
            }
        }

        if(largest != index){
            std::swap(data[index], data[largest]);
            heapify(largest);
        }
    }

    void makeHeap(){
        if(data.empty()) return;

        for(int i = (int)(data.size() - 1) / (int)n; i >= 0; i--){
            heapify(i);
        }
    }

public:
    Heap(size_t n_val = 2) : n(n_val) {}

    Heap(size_t n_val, const vector<int>& input) : n(n_val), data(input) {
        makeHeap();
    }

    Heap(size_t n_val, vector<int>&& input) : n(n_val), data(std::move(input)) {
        makeHeap();
    }

    void insert(const int value){
        size_t currentIndex = data.size();
        data.push_back(value);
        size_t parentIndex = parent(currentIndex);

        while(currentIndex != 0 and data[currentIndex] > data[parentIndex]){
            std::swap(data[currentIndex], data[parentIndex]);
            currentIndex = parentIndex;
            parentIndex = parent(currentIndex);
        }
    }

    int getMax(){
        if( data.empty() ){
            throw std::out_of_range("Empty heap");
        }
        int maxValue = data[0];

        data[0] = data.back();
        data.pop_back();

        if( data.size() > 1){
            heapify(0);
        }

        return maxValue;
    }

    void print() const {
        for(const int item : data){
            std::cout << item << " ";
        }
        std::cout << "\n";
    }

};


int main(int argc, char* argv[]){
    if (argc < 3){
        return 1;
    }

    size_t n = std::stoul(argv[1]);
    std::string filename = argv[2];

    std::ifstream file(filename);
    if (!file.is_open()){
        return 1;
    }

    vector<int> numbers;
    int x;
    while(file >> x){
        numbers.push_back(x);
    }
    file.close();

    Heap heap(n, std::move(numbers));

    heap.print();

    heap.getMax();
    heap.print();

    heap.insert(42);
    heap.print();

    heap.insert(-5);
    heap.print();

    heap.getMax();
    heap.print();

    return 0;
}