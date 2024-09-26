#ifndef nodeHEADER
#define nodeHEADER

#include <memory>
#include <iostream>

template<class K, class V>
class TreeNode {
    public:
        explicit TreeNode(const K& key, const V& value);
        inline K& getKey();
        inline V& getValue();
        void setValue(const V& newValue);
        std::shared_ptr<TreeNode<K, V>> m_left;
        std::shared_ptr<TreeNode<K, V>> m_right;
    private:
        std::pair<K, V> m_data;
};

template<class K, class V>
TreeNode<K, V>::TreeNode(const K& key, const V& value):
    m_left(nullptr),
    m_right(nullptr),
    m_data(std::make_pair(key, value)) {}

template<class K, class V>
inline K& TreeNode<K, V>::getKey() {
    return m_data.first;
}

template<class K, class V>
inline V& TreeNode<K, V>::getValue() {
    return m_data.second;
}

template<class K, class V>
void TreeNode<K, V>::setValue(const V& newValue) {
    m_value = newValue;
}

#endif // nodeHEADER