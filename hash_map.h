#include <functional>
#include <iostream>
#include <memory>
#include <vector>


template<typename KT, typename VT, class HT = std::hash<KT> >
class HashMap {
private:


public:
    class iterator {


    public:
        iterator() = default;

        explicit iterator(HashMap *hm) {
        }

        iterator(HashMap *hm, size_t ind) {
        }

        iterator &operator++() {
        }

        iterator operator++(int) {
        }

        std::pair<const KT, VT> *operator->() {
        }

        std::pair<const KT, VT> &operator*() {
        }


        bool operator==(const iterator &rhs) const {
        }

        bool operator!=(const iterator &rhs) const {
        }
    };

    class const_iterator {


    public:
        const_iterator() = default;


        const_iterator &operator++() {
        }

        const_iterator operator++(int) {
        }

        const std::pair<const KT, VT> *operator->() const {
        }

        const std::pair<const KT, VT> &operator*() const {
        }


        bool operator==(const const_iterator &rhs) const {
        }

        bool operator!=(const const_iterator &rhs) const {
        }
    };


    explicit HashMap(HT hash = HT()) {
    }

    HashMap(std::initializer_list<std::pair<KT, VT>> list, HT hash) {

    }

    HashMap(auto begin, auto end, HT hash) {
    }


    HT hash_function() const {
    }

    void insert(const std::pair<KT, VT> &pair) {
    }


    void erase(const KT &key) {
    }


    void clear() {
    }

    bool empty() const {
    }

    size_t size() const {
    }

    iterator find(const KT &key) {
    }

    const_iterator find(const KT &key) const {
    }

    VT &operator[](const KT &key) {
    }

    const VT &at(const KT &key) const {
    }

    const_iterator begin() const {
    }

    const_iterator end() const {
    }

    iterator begin() {
    }

    iterator end() {
    }

    HashMap(const HashMap<KT, VT> &other) {
    }

    HashMap &operator=(const HashMap<KT, VT> &other) {
    }

    ~HashMap() {
    }
};

