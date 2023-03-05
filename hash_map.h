#include <memory>
#include <vector>


template<typename KT, typename VT, class HT = std::hash<KT>>
class HashMap {
private:
    std::vector<std::unique_ptr<std::pair<const KT, VT> > > data_;
    std::vector<bool> tomb_stones_;
    std::vector<int> robin_bobin;

    HT hash_;
    size_t size_ = 0;
    size_t capacity_ = 0;
public:
    class iterator {
    private:
        HashMap *hm_;
        size_t ind_;

    public:
        iterator() = default;

        explicit iterator(HashMap *hm) : iterator(hm, 0) {

        }

        iterator(HashMap *hm, size_t ind) {
            hm_ = hm;
            ind_ = ind;
            for (auto i = ind; i < hm_->capacity_; ++i) {
                if (hm_->data_[i] != nullptr) {
                    ind_ = i;
                    return;
                }
            }
            ind_ = hm->capacity_;
        }

        iterator &operator++() {
            for (auto i = ind_ + 1; i < hm_->capacity_; ++i) {
                if (hm_->data_[i] != nullptr) {
                    ind_ = i;
                    return *this;
                }
            }
            ind_ = hm_->capacity_;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        std::pair<const KT, VT> *operator->() {
            return &(*hm_->data_[ind_]);
        }

        std::pair<const KT, VT> &operator*() {
            return *hm_->data_[ind_];
        }

        bool operator==(const iterator &rhs) const {
            return hm_ == rhs.hm_ && ind_ == rhs.ind_;
        }

        bool operator!=(const iterator &rhs) const {
            return !(*this == rhs);
        }
    };

    class const_iterator {
    private:
        const HashMap *hm_;
        size_t ind_;

    public:
        const_iterator() = default;

        const_iterator(const HashMap *hm, size_t ind) {
            hm_ = hm;
            ind_ = ind;
            for (auto i = ind; i < hm_->capacity_; ++i) {
                if (hm_->data_[i] != nullptr) {
                    ind_ = i;
                    return;
                }
            }
            ind_ = hm->capacity_;
        }

        const_iterator &operator++() {
            for (auto i = ind_ + 1; i < hm_->capacity_; ++i) {
                if (hm_->data_[i] != nullptr) {
                    ind_ = i;
                    return *this;
                }
            }
            ind_ = hm_->capacity_;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        const std::pair<const KT, VT> *operator->() const {
            return &(*hm_->data_[ind_]);
        }

        const std::pair<const KT, VT> &operator*() const {
            return *hm_->data_[ind_];
        }


        bool operator==(const const_iterator &rhs) const {
            return hm_ == rhs.hm_ && ind_ == rhs.ind_;
        }

        bool operator!=(const const_iterator &rhs) const {
            return !(*this == rhs);
        }
    };


    explicit HashMap(HT hash = HT()) : hash_(hash) {
        capacity_ = 1;
        data_.resize(capacity_);
        tomb_stones_.resize(capacity_);
        robin_bobin.resize(capacity_);
    }

    HashMap(std::initializer_list<std::pair<KT, VT>> list, HT hash = HT()) : HashMap(hash) {
        for (auto &i: list) {
            insert(i);
        }
    }

    template<typename IT>
    HashMap(IT begin, IT end, HT hash = HT()) : HashMap(hash) {
        for (auto i = begin; i != end; ++i) {
            insert(*i);
        }
    }


    HT hash_function() const {
        return hash_;
    }

    void insert(const std::pair<KT, VT> &pair) {
        std::unique_ptr<std::pair<const KT, VT> > pr(new std::pair<const KT, VT>(pair));
        int current_rb = 0;
        for (auto i = hash_(pr->first) % capacity_; i < capacity_; ++i) {
            if (data_[i] == nullptr) {
                tomb_stones_[i] = false;
                data_[i].swap(pr);
                robin_bobin[i] = current_rb;
                ++size_;
                return;
            } else if (data_[i] != nullptr && robin_bobin[i] < current_rb) {
                std::swap(current_rb, robin_bobin[i]);
                pr.swap(data_[i]);
            }
            ++current_rb;
        }
        std::vector<std::unique_ptr<std::pair<const KT, VT>>> tmp;
        for (auto &i: data_) {
            if (i != nullptr) {
                tmp.push_back(std::move(i));
            }
        }
        data_.clear();
        capacity_ *= 2;
        size_ = 0;
        data_.resize(capacity_);
        tomb_stones_.resize(capacity_, false);
        robin_bobin.resize(capacity_);
        for (auto &i: tmp) {
            if (i != nullptr) {
                insert(*i);
            }
        }
        insert(*pr);
    }


    void erase(const KT &key) {
        for (auto i = hash_(key) % capacity_; i < capacity_; ++i) {
            if (data_[i] == nullptr && !tomb_stones_[i]) {
                return;
            }
            if (data_[i] != nullptr && data_[i]->first == key) {
                data_[i] = nullptr;
                tomb_stones_[i] = true;
                --size_;
                return;
            }
        }
    }


    void clear() {
        data_.resize(1);
        data_[0] = nullptr;
        tomb_stones_.assign(1, false);
        robin_bobin.assign(1, 0);
        capacity_ = 1;
        size_ = 0;
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    iterator find(const KT &key) {
        for (auto i = hash_(key) % capacity_; i < capacity_; ++i) {
            if (data_[i] == nullptr && !tomb_stones_[i]) {
                return end();
            }
            if (data_[i] != nullptr && data_[i]->first == key) {
                return iterator(this, i);
            }
        }
        return end();
    }

    const_iterator find(const KT &key) const {
        for (auto i = hash_(key) % capacity_; i < capacity_; ++i) {
            if (data_[i] == nullptr && !tomb_stones_[i]) {
                return end();
            }
            if (data_[i]->first == key) {
                return const_iterator(this, i);
            }
        }
        return end();
    }

    VT &operator[](const KT &key) {
        auto it = find(key);
        if (it == end()) {
            insert({key, VT()});
        }
        return find(key)->second;
    }

    const VT &at(const KT &key) const {
        for (auto i = hash_(key) % capacity_; i < capacity_; ++i) {
            if (data_[i] == nullptr && !tomb_stones_[i]) {
                throw std::out_of_range("out of range");
            }
            if (data_[i]->first == key) {
                return data_[i]->second;
            }
        }
        throw std::out_of_range("out of range");
    }

    const_iterator begin() const {
        return const_iterator(this, 0);
    }

    const_iterator end() const {
        return const_iterator(this, capacity_);
    }

    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        return iterator(this, capacity_);
    }

    HashMap(const HashMap<KT, VT> &other) {
        *this = other;
    }

    HashMap &operator=(const HashMap<KT, VT> &other) {
        if (this != &other) {
            hash_ = other.hash_;
            capacity_ = other.capacity_;
            data_.resize(capacity_);
            tomb_stones_.resize(capacity_);
            robin_bobin.resize(capacity_);
            for (size_t i = 0; i < data_.size(); ++i) {
                if (other.data_[i]) {
                    data_[i] = std::make_unique<std::pair<const KT, VT>>(other.data_[i]->first, other.data_[i]->second);
                }
                tomb_stones_[i] = other.tomb_stones_[i];
                robin_bobin[i] = other.robin_bobin[i];
            }
        }
        return *this;
    }

    ~HashMap() {
        for (size_t i = 0; i < data_.size(); ++i) {
            data_[i].reset();
        }
    }
};
