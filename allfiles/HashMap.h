/** @file */

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include "Utility.h"
#include<cmath>

/**
 * HashMap is a map implemented by hashing. Also, the 'capacity' here means the
 * number of buckets in your inner implemention, not the current number of the
 * elements.
 *
 * Template argument H are used to specify the hash function.
 * H should be a class with a static function named ``hashcode'',
 * which takes a parameter of type T and returns a value of type int.
 * For example, the following class
 * @code
 *      class Hashint {
 *      public:
 *          static int hashcode(int obj) {
 *              return obj;
 *          }
 *      };
 * @endcode
 * specifies an hash function for integers. Then we can define:
 * @code
 *      HashMap<int, int, Hashint> hash;
 * @endcode
 *
 * We don't require an order in the iteration, but you should
 * guarantee all elements will be iterated.
 */
template <class K, class V, class H>
class HashMap {
    public:
    static const int DEFAULT_CAPCITY = 11;
    static const double DEFAULT_LOAD_FACTOR = 0.75;
    private:
    template <class K2, class V2>
    class HashEntry: public Entry<K2, V2> {
        public:
        HashEntry<K2, V2>* next;
        HashEntry() {}
        HashEntry(K2 _key, V2 _value): Entry<K2, V2>(_key, _value) {}
    };
    int threshold, cap;
    double loadFactor;
    HashEntry<K, V>** buckets;
    int sz;

    int getMax(int a, int b) {
        if (a > b) return a;
        return b;
    }

    void addEntry(const K& key, const V& value, int idx) {
        HashEntry<K, V> *e = new HashEntry<K, V>(key, value);
        e->next = buckets[idx];
        buckets[idx] = e;
    }

    void rehash() {
        HashEntry<K, V>** oldBuckets = buckets;
        int oldCap = cap;
        cap = cap * 2 + 1;
        threshold = (int)(cap * loadFactor);
        buckets = new HashEntry<K, V>*[cap];
        for (int i = 0; i < cap; ++i) buckets[i] = NULL;
        for (int i = oldCap - 1; i >= 0; --i) {
            HashEntry<K, V> *e = oldBuckets[i];
            while (e != NULL) {
                int idx = hash(e->key);
                HashEntry<K, V> *nxt = e->next;
                e->next = buckets[idx];
                buckets[idx] = e;
                e = nxt;
            }
        }
        delete [] oldBuckets;
    }

    void getSpace() {
        for (int i = 0; i < cap; ++i) {
            HashEntry<K, V> *e = buckets[i], *nxt;
            while (e != NULL) {
                nxt = e->next;
                delete e;
                e = nxt;
            }
        }
    }

    public:
    class ConstIterator {
        private:
            int cnt, idx;
            HashEntry<K, V> *last, *nxt;
            const HashMap *hashM;
        public:
        void init(const HashMap* _hashM, int _cnt, int _idx) {
            hashM = _hashM; cnt = _cnt; idx = _idx;
            last = nxt = NULL;
        }
        /**
         * Returns true if the iteration has more elements.
         * O(n) for iterating through the container.
         */
        bool hasNext() { 
            return cnt > 0;
        }

        /**
         * Returns a const reference the next element in the iteration.
         * O(n) for iterating through the container.
         * @throw ElementNotExist
         */
        Entry<K, V>& next() { 
            if (cnt == 0) throw ElementNotExist("\nNo Such Element\n");
            cnt--;
            HashEntry<K, V>* e = nxt;
            while (e == NULL) e = hashM->buckets[--idx];
            nxt = e->next;
            last = e;
            return *e;
        }
    };

    class Iterator
    {
        private:
            int cnt, idx;
            HashEntry<K, V> *last, *nxt;
            HashMap *hashM;
        public:
        void init(HashMap* _hashM, int _cnt, int _idx) {
            hashM = _hashM; cnt = _cnt; idx = _idx;
            last = nxt = NULL;
        }
        /**
         * Returns true if the iteration has more elements.
         * O(n) for iterating through the container.
         */
        bool hasNext() { 
            return cnt > 0;
        }

        /**
         * Returns a const reference the next element in the iteration.
         * O(n) for iterating through the container.
         * @throw ElementNotExist
         */
        Entry<K, V>& next() { 
            if (cnt == 0) throw ElementNotExist("\nNo Such Element\n");
            cnt--;
            HashEntry<K, V>* e = nxt;
            while (e == NULL) e = hashM->buckets[--idx];
            nxt = e->next;
            last = e;
            return *e;
        }

        /**
         * Removes from the underlying collection the last element returned by the iterator (optional operation).
         * O(1)
         * @throw ElementNotExist
         */
        void remove() { 
            if (last == NULL) throw ElementNotExist("\nIllegal State\n");
            hashM->remove(last->key);
            last = NULL;
        }
    };

    void init(int iniCap, double loadFac) {
        if (iniCap == 0) iniCap = 1; 
        cap = iniCap;
        buckets = new HashEntry<K, V>*[iniCap];
        loadFactor = loadFac;
        threshold = (int)(iniCap * loadFac);
        clear2();
    }

    /**
     * Constructs an empty list with an initial capacity.
     * You can construct it with your own initial capacity.
     */
    HashMap() { 
        init(DEFAULT_CAPCITY, DEFAULT_LOAD_FACTOR);
    }

    /**
     * Copy-constructor
     */
    HashMap(const HashMap& x) { 
        init(getMax(x.size() * 2, DEFAULT_CAPCITY), DEFAULT_LOAD_FACTOR);
        addAll(*this, x);
    }

    /**
     * Constructs an empty HashMap with the specified initial capacity
     */
    HashMap(int initialCapacity) { 
        init(initialCapacity, DEFAULT_LOAD_FACTOR);
    }

    /**
     * Constructs a new HashMap with the same mappings as the specified Map.
     */
    template <class E2>
    explicit HashMap(const E2 &m) {
        init(getMax(m.size() * 2, DEFAULT_CAPCITY), DEFAULT_LOAD_FACTOR);
        addAll(*this, m);
    }

    /**
     * assignment operator
     */
    HashMap& operator = (const HashMap& x) { 
        clear();
        addAll(*this, x);
        return *this;
    }

    /**
     * destructor
     */
    ~HashMap() { 
        getSpace();
        delete [] buckets;
    }

    /**
     * Returns an iterator over the elements in this map in proper sequence.
     */
    Iterator iterator() { 
        Iterator Itr;
        Itr.init(this, sz, cap);
        return Itr;
    }

    /**
     * Returns an CONST iterator over the elements in this map in proper sequence.
     */
    ConstIterator constIterator() const { 
        ConstIterator CItr;
        CItr.init(this, sz, cap);
        return CItr;
    }

    int hash(const K &key) const{
        return abs(H::hashcode(key) % cap);
    }

    /**
     * Removes all of the mappings from this map.
     */

    void clear() { 
        getSpace();
        for (int i = 0; i < cap; ++i) buckets[i] = NULL;
        sz = 0;
    }

    void clear2() {
        for (int i = 0; i < cap; ++i) buckets[i] = NULL;
        sz = 0;
    }

    /**
     * Returns true if this map contains a mapping for the specified key.
     * O(1) for average
     */
    bool containsKey(const K& key) const { 
        int idx = hash(key);
        HashEntry<K, V> *e = buckets[idx];
        while (e != NULL) {
           if (key == e->key) return true;
           e = e->next;
        }
        return false;
    }

    /**
     * Returns true if this map maps one or more keys to the specified value.
     * O(n)
     */
    bool containsValue(const V& value) const { 
        for (int i = cap - 1; i >= 0; --i) {
            HashEntry<K, V> *e = buckets[i];
            while (e != NULL) {
                if (value == e->value) return true;
                e = e->next;
            }
        }
        return false;
    }

    /**
     * Returns a reference to the value to which the specified key is mapped.
     * O(1) for average
     * @throw ElementNotExist
     */
    V& get(const K& key) { 
        if (!containsKey(key)) throw ElementNotExist("\nNo Such Key\n");
        int idx = hash(key);
        HashEntry<K, V> *e = buckets[idx];
        while (e != NULL) {
            if (key == e->key) return e->value;
            e = e->next;
        }
    }

    /**
     * Returns a const reference to the value to which the specified key is mapped.
     * O(1) for average
     * @throw ElementNotExist
     */
    const V& get(const K& key) const { 
        if (!containsKey(key)) throw ElementNotExist("\nNo Such Key\n");
        int idx = hash(key);
        HashEntry<K, V> *e = buckets[idx];
        while (e != NULL) {
            if (key == e->key) return e->value;
            e = e->next;
        }
    }

    /**
     * Returns true if this map contains no key-value mappings.
     * O(1)
     */
    bool isEmpty() const { 
        return sz == 0;
    }

    void add(const Entry<K, V>& e) {
        put(e.key, e.value);
    }

    /**
     * Associates the specified value with the specified key in this map.
     * Returns the previous value, if not exist, a value returned by the default-constructor.
     * O(1)
     */
    V put(const K& key, const V& value) {
        int idx = hash(key);
        HashEntry<K, V> *e = buckets[idx];
        while (e != NULL) {
            if (key == e->key) {
                V r = e->value;
                e->value = value;
                return r;
            }
            else e = e->next;
        }
        if (++sz > threshold) {
            rehash();
            idx = hash(key);
        }
        addEntry(key, value, idx);
        V r; return r;
    }

    /**
     * Removes the mapping for the specified key from this map if present.
     * Returns the previous value.
     * O(1) for average
     * @throw ElementNotExist
     */
    V remove(const K& key) { 
        if (!containsKey(key)) throw ElementNotExist("\nNo Such Key\n");
        int idx = hash(key);
        HashEntry<K, V> *e = buckets[idx], *last = NULL;
        while (e != NULL) {
            if (key == e->key) {
                if (last == NULL) buckets[idx] = e->next;
                else last->next = e->next;
                sz--;
                int r = e->value;
                delete e;
                return r;
            }
            last = e;
            e = e->next;
        }
    }

    /**
     * Returns the number of key-value mappings in this map.
     * O(1)
     */
    int size() const { 
        return sz;
    }
};
#endif
