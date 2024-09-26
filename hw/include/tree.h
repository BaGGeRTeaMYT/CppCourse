#ifndef treeHEADER
#define treeHEADER

#include <node.h>
#include <stack>
#include <optional>

template<class K, class V>
class iterator {
    using value_type = std::pair<K, V>;
    using pointer = std::pair<K, V>*;
    using reference = std::pair<K, V>&;
    using difference_type = std::size_t;
    using iterator_category = std::forward_iterator_tag;
    // + karma
    public:
        std::pair<K, V>* m_data;
        std::optional< std::stack<std::pair<K, V>> > m_inorder;
        explicit iterator(TreeNode<K, V>* data, bool makeStack = false);
        explicit iterator(std::pair<K, V>* data);
        std::pair<K, V>& operator*();
        std::pair<K, V>* operator->();
        iterator& operator++();
        iterator operator++(int);
        bool operator!=(iterator);
};

template<class K, class V>
bool iterator<K, V>::operator!=(iterator secondOperand) {
    if (m_data->first != secondOperand.m_data->first) {
        return true;
    }
    return false;
}

template<class K, class V>
void DFS(std::stack<std::pair<K, V>>& st, TreeNode<K, V>* curNode) {
    std::cout << "Now in DFS" << std::endl;
    if (curNode->m_left) {
        DFS(st, curNode->m_left.get());
    }
    st.push(std::make_pair(curNode->getKey(), curNode->getValue()));
    std::cout << "Added " << curNode->getKey() << " to stack" << std::endl;
    if (curNode->m_right) {
        DFS(st, curNode->m_right.get());
    }
}

template<class K, class V>
iterator<K, V>::iterator(TreeNode<K, V>* data, bool makeStack) {
    if (data != nullptr) {
        m_inorder = {};
        std::cout << "Creating iterator for " << data->getKey() << " with value " << data->getValue() << std::endl;
        if (makeStack) {
            std::cout << "Starting stack creation" << std::endl;
            DFS(m_inorder.value(), data);
            m_inorder.value().pop();
        }
        m_data = new std::pair<K, V>( std::make_pair(data->getKey(), data->getValue()) );
    }
}

template<class K, class V>
iterator<K, V>::iterator(std::pair<K, V>* data): m_data(data) {}

template<class K, class V>
std::pair<K, V>& iterator<K, V>::operator*() {
    return *m_data;
}

template<class K, class V>
std::pair<K, V>* iterator<K, V>::operator->() {
    return m_data;
}

template<class K, class V>
iterator<K, V>& iterator<K, V>::operator++() {
    m_data = &m_inorder.value().top();
    m_inorder.value().pop();
    return std::move(iterator<K, V>(m_data));
}

template<class K, class V>
iterator<K, V> iterator<K, V>::operator++(int) {
    auto saveValue = m_data;
    m_data = m_inorder.value().top();
    m_inorder.value().pop();
    return saveValue;
}

template<class K, class V>
class SearchingTree {
    public:
        SearchingTree(const TreeNode<K, V>&);
        SearchingTree(const K& key, const V& value);
        void insert(const K& key, const V& value);
        void erase(const K& key);
        std::shared_ptr<TreeNode<K, V>> find(const K& key);
        using iterator = iterator<K, V>;
        iterator begin(); // return begin;
        iterator end();
    private:
        std::shared_ptr<TreeNode<K, V>> m_root;
        iterator m_begin;
        iterator m_end; // fake node
};

// left print right

template<class K, class V>
iterator<K, V> SearchingTree<K, V>::begin() {
    return m_begin;
}

template<class K, class V>
iterator<K, V> SearchingTree<K, V>::end() {
    return m_end;
}

template<class K, class V>
SearchingTree<K, V>::SearchingTree(const TreeNode<K, V>& root):
    m_root(std::make_shared<TreeNode<K, V>>(root)),
    m_begin(nullptr, false),
    m_end(nullptr, false) {
    K tmpKey = m_root->getKey();
    V tmpValue = m_root->getValue();
    m_begin = iterator(m_root.get());
    TreeNode<K, V> fakeNode(tmpKey, tmpValue);
    fakeNode.m_left = m_root;
    m_end = iterator(&fakeNode);
}

template<class K, class V>
SearchingTree<K, V>::SearchingTree(const K& key, const V& value): SearchingTree<K, V>(TreeNode<K, V>(key, value)) {}

template<class K, class V>
void SearchingTree<K, V>::insert(const K& key, const V& value) {
    std::cout << "Entered insert" << std::endl;

    std::shared_ptr<TreeNode<K, V>> parent = m_root;
    while (true) {

        if (parent->getKey() > key) {
            if (parent->m_left) {
                parent = parent->m_left;
            } else {
                // updateBegin
                TreeNode<K, V> toInsert(key, value);
                m_begin = iterator(&toInsert, true);
                std::cout << "Now begin holds key " << (*m_begin).first << std::endl;
                parent->m_left = std::make_shared<TreeNode<K, V>>(toInsert);
                break;
            }
        } else if (parent->getKey() < key) {
            if (parent->m_right) {
                parent = parent->m_right;
            } else {
                parent->m_right = std::make_shared<TreeNode<K, V>>(key, value);
                break;
            }
        } else {
            break;
        }

    }
    std::cout << "Insert completed" << std::endl;
}

template<class K, class V>
void SearchingTree<K, V>::erase(const K& key) {
    
}

template<class K, class V>
std::shared_ptr<TreeNode<K, V>> SearchingTree<K, V>::find(const K& key) {
    std::shared_ptr<TreeNode<K, V>> currentNode = m_root;
    while (currentNode) {
        if (key > currentNode->getKey()) {
            currentNode = currentNode->m_right;
        } else if (key < currentNode->getKey()) {
            currentNode = currentNode->m_left;
        } else {
            return currentNode;
        }
    }
    return nullptr;
}

#endif // treeHEADER