#ifndef TreapHEADER
#define TreapHEADER

#include <memory>
#include <vector>
#include <iostream>
#include <string>

constexpr int INF = 1e9;

template<class K, class V>
struct TreeNode {
    int prior;
    K key;
    V value;    
    std::pair<K, V>* m_data;
    TreeNode *m_left, *m_right;
    explicit TreeNode(K key, V value):
        prior(rand()),
        key(key),
        value(value),
        m_data(new std::pair<K, V>{key, value}),
        m_left(nullptr),
        m_right(nullptr) {}
};

template<class K, class V>
class Iterator {
private:
    using TreeNode = TreeNode<K, V>;
    std::vector<TreeNode*> prev = {nullptr};

public:
    Iterator(TreeNode* iter) {
        goLeft(iter);
    }

    void debugPrint() {
        for (int i = 0; i < prev.size(); i++) {
            if (prev[i]) {
                std::cout << i << ": " << prev[i]->key << " " << prev[i]->value << std::endl;
            }
        }
        std::cout << "Finished" << std::endl;
    }

    std::pair<K, V>& operator*() {
        return *(prev.back()->m_data);
    }
    std::pair<K, V>* operator->() {
        return prev.back()->m_data;
    }
    void goLeft(TreeNode* node) {
        while(node) {
            // std::cout << "Processing {" << node->key << ", " << node->value << "}" << std::endl;
            prev.push_back(node);
            node = node->m_left;
        }
    }
    Iterator& operator++() {
        TreeNode* node = prev.back();
        prev.pop_back();
        if (node->m_right) {
            goLeft(node->m_right);
        }
        return *this;
    }
    Iterator operator++(int) {
        Iterator temp = *this;
        TreeNode* node = prev.back();
        prev.pop_back();
        if (node->m_right) {
            goLeft(node->m_right);
        }
        return temp;
    }
    bool operator!=(const Iterator& other) const {
        return prev.back() != other.prev.back();
    }
    bool operator==(const Iterator& other) const {
        return prev.back() == other.prev.back();
    }
};

template<class K, class V>
class Treap {
public:
    using TreeNode = TreeNode<K, V>;
    TreeNode* m_root;
    Treap(): m_root(nullptr) {}
    TreeNode* merge(TreeNode*, TreeNode*);
    void split(TreeNode* t, K x, TreeNode*& a, TreeNode*& b);
    V find(const K &key);
    V get(TreeNode* t, K key);
    void insert(const K &key, const V &value);
    void erase(const K &key);
    void traverse(TreeNode* t);
    void traverse();
    TreeNode* getBegin(TreeNode* t);
    TreeNode* getEnd(TreeNode* t);

    using iterator = Iterator<K, V>;
    Treap range(const K& beginKey, const K& endKey);
    void createSubTree(const K& beginKey, const K& endKey, Treap& subTree, TreeNode* node);
    iterator begin(); // return begin;
    iterator end();
};

template<class K, class V>
void Treap<K, V>::createSubTree(const K& beginKey, const K& endKey, Treap& subTree, TreeNode* node) {
    if (node->key >= beginKey && node->key < endKey) {
        subTree.insert(node->key, node->value);
    }
    if (node->m_left) {
        createSubTree(beginKey, endKey, subTree, node->m_left);
    }
    if (node->m_right) {
        createSubTree(beginKey, endKey, subTree, node->m_right);
    }
}

template<class K, class V>
Treap<K, V> Treap<K, V>::range(const K& beginKey, const K& endKey) {
      Treap<K, V> tmpTree;
      createSubTree(beginKey, endKey, tmpTree, m_root);
      return tmpTree;
}

template<class K, class V>
Iterator<K, V> Treap<K, V>::begin() {
    return Iterator<K, V>(m_root);
}
template<class K, class V>
Iterator<K, V> Treap<K, V>::end() {
    return Iterator<K, V>(nullptr);
}

template<class K, class V>
TreeNode<K, V>* Treap<K, V>::getBegin(TreeNode* t) {
    if (!t->m_left) {
        return t;
    }
    return getBegin(t->m_left);
}

template<class K, class V>
TreeNode<K, V>* Treap<K, V>::getEnd(TreeNode* t) {
    if (!(t->m_right)) {
        return t;
    }
    return getEnd(t->m_right);
}

template<class K, class V>
TreeNode<K, V>* Treap<K, V>::merge(TreeNode* l, TreeNode* r) {
    if (!l) {
        return r;
    }
    if  (!r) {
        return l;
    }
    if (l->prior > r->prior) {
        l->m_right = merge(l->m_right, r);
        return l;
    } else {
        r->m_left = merge(l, r->m_left);
        return r;
    }
}

template<class K, class V>
void Treap<K, V>::split(TreeNode* t, K x, TreeNode*& a, TreeNode*& b) {
    if (!t) {
        a = 0;
        b = 0;
        return;
    }
    if (t->key < x) {
        split(t->m_right, x, t->m_right, b);
        a = t;
    }
    else {
        split(t->m_left, x, a, t->m_left);
        b = t;
    }
}

template<class K, class V>
V Treap<K, V>::find(const K &key) {
    int val = get(m_root, key);
    if (val == INF) {
        return INF;
    } else {
        return val;
    }
}

template<class K, class V>
V Treap<K, V>::get(TreeNode* t, K key) {
    if (!t) {
        return INF;
    }
    if (t->key == key) {
        return t->value;
    }
    if (t->key < key) {
        return get(t->m_right, key);
    } else {
        return get(t->m_left, key);
    }
}

template<class K, class V>
void Treap<K, V>::insert(const K &key, const V &value) {
    int val = get(m_root, key);
    if (val != INF) {
        return;
    }
    TreeNode *a, *b;
    split(m_root, key, a, b);
    TreeNode* t = new TreeNode(key, value);
    m_root = merge(a, merge(t, b));
}

template<class K, class V>
void Treap<K, V>::erase(const K &key) {
    TreeNode *a, *b, *cur;
    split(m_root, key, a, cur);
    split(cur, key + 1, cur, b);
    m_root = merge(a, b);
}

template<class K, class V>
void Treap<K, V>::traverse(TreeNode* t) {
    if (!t) {
        return;
    }
    traverse(t->m_left);
    std::cout << t->key << ' ' << t->value << std::endl;
    traverse(t->m_right);
}

template<class K, class V>
void Treap<K, V>::traverse() {
    traverse(m_root);
}


#endif // TreapHEADER