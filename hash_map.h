#include <iostream>
#include <vector>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> >
class HashMap {
    public:
        
    private:
        std::vector<std::pair<KeyType, ValueType> > data_;
};


int main() {

    return 0;
}
