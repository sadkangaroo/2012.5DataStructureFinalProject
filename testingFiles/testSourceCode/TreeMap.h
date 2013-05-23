/** @file */
#ifndef __TREEMAP_H
#define __TREEMAP_H

#include "Utility.h"

/**
 * A map is a sequence of (key, value) entries that provides fast retrieval
 * based on the key. At most one value is held for each key.
 *
 * TreeMap is the balanced-tree implementation of map. The iterators must
 * iterate through the map in the natural order (operator<) of the key.
 */
template<class K, class V> class TreeMap {
    private:
    static const int RED = -1, BLACK = 1; 
    template <class K2, class V2>
    class Node: public Entry<K2, V2> {
        public:
        int color;
        Node<K2, V2> *left, *right, *parent;
        Node(): Entry<K2, V2>(K(), V()) {
            color = BLACK;
            left = right = parent = this;
        }
        Node(K2 _key, V2 _value, int _color, Node<K2, V2>* _left, Node<K2, V2>* _right, Node<K2, V2>* _parent): Entry<K2, V2>(_key, _value) {
           color = _color; 
           left = _left; right = _right; parent = _parent;
        }
    };
    Node<K, V> *nil, *root;
    int sz;

    void fabricateTree() {
        nil = new Node<K, V>;
        root = nil;
        sz = 0;
    }

    Node<K, V>* firstNode() const {
        Node<K, V> *node = root;
        while (node->left != nil) 
            node = node->left;
        return node;
    }

    Node<K, V>* lastNode() const {
        Node<K, V> *node = root;
        while (node->right != nil)
            node = node->right;
        return node;
    }

    void removeNode(Node<K, V>* node) {
        Node<K, V> *splice, *child;
        sz--;
        if (node->left == nil) {
            splice = node;
            child = node->right;
        }
        else if (node->right == nil) {
            splice = node;
            child = node->left;
        }
        else {
            splice = node->left;
            while (splice->right != nil) 
                splice = splice->right;
            child = splice->left;
            Node<K, V> *newNode = new Node<K, V>(splice->key, splice->value, node->color, node->left, node->right, node->parent);
            if (newNode->left != nil) newNode->left->parent = newNode;
            if (newNode->right != nil) newNode->right->parent = newNode;
            if (newNode->parent != nil) {
                if (newNode->parent->left == node) newNode->parent->left = newNode;
                else newNode->parent->right = newNode;
            }
            if (root == node) root = newNode;
            delete node; node = newNode;
        }

        Node<K, V> *parent = splice->parent;
        if (child != nil) child->parent = parent;
        if (parent == nil) {
            root = child;
            delete splice;
            return;
        }
        if (splice == parent->left) 
            parent->left = child;
        else parent->right = child;
        if (splice->color == BLACK)
            deleteFixup(child, parent);
        delete splice;
    }

    Node<K, V>* getNode(K key) const {
        Node<K, V> *current = root;
        while (current != nil) {
            if (key > current->key) current = current->right;
            else if (key < current->key) current = current->left;
            else return current;
        }
        return current;
    }

    Node<K, V>* successor(Node<K, V>* node) const {
        if (node->right != nil) {
            node = node->right;
            while (node->left != nil)
                node = node->left;
            return node;
        }
        Node<K, V>* parent = node->parent;
        while (node == parent->right) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }

    void rotateLeft(Node<K, V>* node) {
        Node<K, V> * child = node->right;
        node->right = child->left;
        if (child->left != nil)
            child->left->parent = node;
        child->parent = node->parent;
        if (node->parent != nil) {
            if (node == node->parent->left)
                node->parent->left = child;
            else node->parent->right = child;
        }
        else root = child;
        child->left = node;
        node->parent = child;
    }

    void rotateRight(Node<K, V>* node) {
        Node<K, V> *child = node->left;
        node->left = child->right;
        if (child->right != nil)
            child->right->parent = node;
        child->parent = node->parent;
        if (node->parent != nil) {
            if (node == node->parent->right)
                node->parent->right = child;
            else node->parent->left = child;
        }
        else root = child;
        child->right = node;
        node->parent = child;
    }

    void insertFixup(Node<K, V>* node) {
        while (node->parent->color == RED && node->parent->parent != nil) {
            if (node->parent == node->parent->parent->left) {
                Node<K, V> *uncle = node->parent->parent->right;
                if (uncle->color == RED) {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    uncle->parent->color = RED;
                    node = uncle->parent;
                }
                else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        rotateLeft(node);
                    }
                    node->parent->color = BLACK ;
                    node->parent->parent->color = RED;
                    rotateRight(node->parent->parent);
                }
            }
            else {
                Node<K, V> *uncle = node->parent->parent->left;
                if (uncle->color == RED) {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    uncle->parent->color = RED;
                    node = uncle->parent;
                }
                else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        rotateRight(node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateLeft(node->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void deleteFixup(Node<K, V>* node, Node<K, V> *parent) {
        while (node != root && node->color == BLACK) {
            if (node == parent->left) {
                Node<K, V> *sibling = parent->right;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    parent->color = RED;
                    rotateLeft(parent);
                    sibling = parent->right;
                }
                if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                    sibling->color = RED;
                    node = parent;
                    parent = parent->parent;
                }
                else {
                    if (sibling->right->color == BLACK) {
                        sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = parent->right;
                    }   
                    sibling->color = parent->color;
                    parent->color = BLACK;
                    sibling->right->color = BLACK;
                    rotateLeft(parent);
                    node = root;
                }
            }
            else {
                Node<K, V>* sibling = parent->left;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    parent->color = RED;
                    rotateRight(parent);
                    sibling = parent->left;
                }
                if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
                    sibling->color = RED;
                    node = parent;
                    parent = parent->parent;
                }
                else {
                    if (sibling->left->color == BLACK) {
                        sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = parent->left;
                    }
                    sibling->color = parent->color;
                    parent->color = BLACK;
                    sibling->left->color = BLACK;
                    rotateRight(parent);
                    node = root;
                }
            }
        }
        node->color = BLACK;
    }

    public:
    class ConstIterator {
        private:
            Node<K, V> *last, *nxt;
            const TreeMap *treeM;
        public:
        void init(Node<K, V> *_nxt, const TreeMap *_treeM) {
            nxt = _nxt; last = NULL; treeM = _treeM;
        }
        /**
         * Returns true if the iteration has more elements.
         * Amortized O(1).
         */
        bool hasNext() {
            return nxt != treeM->nil;
        }

        /**
         * Returns a reference to the next element in the iteration.
         * Amortized O(1).
         * @throw ElementNotExist
         */
        Entry<K, V>& next() {
            if (nxt == treeM->nil) throw ElementNotExist("\nNo Such Element\n");
            last = nxt;
            nxt = treeM->successor(last);
            return *last;
        }

    };

    class Iterator {
        private:
            Node<K, V> *last, *nxt;
            TreeMap *treeM;
        public:
        void init(Node<K, V> *_nxt, TreeMap *_treeM) {
            nxt = _nxt; last = NULL; treeM = _treeM;
        }
        /**
         * Returns true if the iteration has more elements.
         * Amortized O(1).
         */
        bool hasNext() {
            return nxt != treeM->nil;
        }

        /**
         * Returns a reference to the next element in the iteration.
         * Amortized O(1).
         * @throw ElementNotExist
         */
        Entry<K, V>& next() {
            if (nxt == treeM->nil) throw ElementNotExist("\nNo Such Element\n");
            last = nxt;
            nxt = treeM->successor(last);
            return *last;
        }

        /**
         * Removes from the underlying collection the last element
         * returned by the iterator
         * Amortized O(1).
         * @throw ElementNotExist
         */
        void remove() {
            if (last == NULL) throw ElementNotExist("\nIllegal State\n");
            treeM->removeNode(last);
            last = NULL;
        }
    };

    /**
     * Constructs an empty map
     */
    TreeMap() { 
        fabricateTree();
    }

    /**
     * Copy constructor
     */
    TreeMap(const TreeMap &c) {
        fabricateTree();
        addAll(*this, c);
    }

    /**
     * Destructor
     */
    ~TreeMap() { 
        delete nil;
        clear();
    }

    /**
     * Assignment operator
     */
    TreeMap& operator=(const TreeMap &c) {
        clear();
        addAll(*this, c);
        return *this;
    }

    /**
     * Constructs a new tree map containing the same mappings as the
     * given map
     */
    template <class C> TreeMap(const C& c) {
        fabricateTree();
        addAll(*this, c);
    }

    /**
     * Returns an iterator over the elements in this map.
     * O(1).
     */
    Iterator iterator() {
        Iterator Itr;
        Itr.init(firstNode(), this);
        return Itr;
    }

    /**
     * Returns an const iterator over the elements in this map.
     * O(1).
     */
    ConstIterator constIterator() const {
        ConstIterator CItr;
        CItr.init(firstNode(), this);
        return CItr;
    }

    /**
     * Removes all of the mappings from this map.
     * O(n).
     */
    void clear() {
        while (sz) {
            removeNode(root);
        }
    }

    /**
     * Returns true if this map contains a mapping for the specified key.
     * O(logn).
     */
    bool containsKey(const K& key) const {
        return getNode(key) != nil;
    }

    /**
     * Returns true if this map contains a mapping for the specified value.
     * O(n).
     */
    bool containsValue(const V& value) const {
        Node<K, V> *node = firstNode();
        while (node != nil) {
            if (value == node->value)
                return true;
            node = successor(node);
        }
        return false;
    }

    /**
     * Returns a key-value mapping associated with the least key in
     * this map.
     * O(logn).
     * @throw ElementNotExist
     */
    const Entry<K, V>& firstEntry() const {
        Node<K, V> *node = firstNode();
        if (node == nil) throw ElementNotExist("\nNo Such Element\n");
        return *node;
    }

    /**
     * Returns the first (lowest) key currently in this map.
     * O(logn).
     * @throw ElementNotExist
     */
    const K& firstKey() const {
        if (root == nil) throw ElementNotExist("\nNo Such Element\n");
        return firstNode()->key;
    }

    /**
     * Returns a reference to the value which the specified key is mapped
     * O(logn).
     * @throw ElementNotExist
     */
    V& get(const K& key) {
        if (!containsKey(key)) throw ElementNotExist("\nNo Such Element\n");
        return getNode(key)->value;
    }

    /**
     * Returns a reference to the value which the specified key is mapped
     * O(logn).
     * @throw ElementNotExist
     */
    const V& get(const K& key) const {
        if (!containsKey(key)) throw ElementNotExist("\nNo Such Element\n");
        return getNode(key)->value;
    }

    /**
     * Returns a key-value mapping associated with the greatest key
     * in this map.
     * O(logn).
     * @throw ElementNotExist
     */
    const Entry<K, V>& lastEntry() const {
        Node<K, V> *node = lastNode();
        if (node == nil) throw ElementNotExist("\nNo Such Element\n");
        return *node;
    }

    /**
     * Returns the last (highest) key currently in this map.
     * O(logn).
     * @throw ElementNotExist
     */
    const K& lastKey() const {
        if (root == nil) throw ElementNotExist("\nNo Such Element\n");
        return lastNode()->key;
    }

    void add(const Entry<K, V>& e) {
        put(e.key, e.value);
    }

    /**
     * Associates the specified value with the specified key in this map.
     * Returns the previous value, if not exist, a value returned by the
     * default-constructor.
     * O(logn).
     */

    V put(const K& key, const V& value) {
        Node<K, V> *current = root, *parent = nil;
        while (current != nil) {
            parent = current;
            if (key > current->key) current = current->right;
            else if (key < current->key) current = current->left;
            else {
                V res = current->value;
                current->value = value;
                return res;
            }
        }
        Node<K, V> *node = new Node<K, V>(key, value, RED, nil, nil, parent);
        sz++;
        if (parent == nil) {
            root = node;
            V res;
            return res;
        }
        if (key > parent->key) parent->right = node;
        else parent->left = node;
        insertFixup(node);
        V res; return res;
    }

    /**
     * Removes the mapping for this key from this TreeMap if present.
     * O(logn).
     * @throw ElementNotExist
     */
    V remove(const K& key) {
        Node<K, V> *node = getNode(key);
        if (node == nil) throw ElementNotExist("\nNo Such Element\n");
        V res = node->value;
        removeNode(node);
        return res;
    }

    bool isEmpty() {
        return sz == 0;
    }

    /**
     * Returns the number of key-value mappings in this map.
     * O(logn).
     */
    int size() const {
        return sz;
    }
};

#endif
