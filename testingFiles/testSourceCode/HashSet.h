/** @file */
#ifndef __HASHSET_H
#define __HASHSET_H

#include "Utility.h"
#include "HashMap.h"

/**
 * A HashSet holds elements using a hash table, providing facilities
 * to insert, remove an element into the container and search an
 * element within the container efficiently.
 *
 * We don't require an order in the iteration, but you should
 * guarantee all elements will be iterated.
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
 *      HashSet<int, Hashint> hash;
 * @endcode
 */
template <class T, class H> class HashSet {
    private:
    HashMap<T, bool, H>* map;

    int getMax(int a, int b) {
        if (a > b) return a;
        return b;
    }

    public:
    class ConstIterator {
        public:
        typename HashMap<T, bool, H>::ConstIterator mCItr;
        /**
         * Returns true if the iteration has more elements.
         * O(n) for iterating through the container
         */
        bool hasNext() {
            return mCItr.hasNext();
        }

        /**
         * Returns the next element in the iteration.
         * O(n) for iterating through the container.
         * @throw ElementNotExist
         */
        const T& next() {
            return mCItr.next().key;
        }
    };

    class Iterator {
        public:
        typename HashMap<T, bool, H>::Iterator mItr;
        /**
         * Returns true if the iteration has more elements.
         * O(n) for iterating through the container.
         */
        bool hasNext() {
            return mItr.hasNext();
        }

        /**
         * Returns the next element in the iteration.
         * O(n) for iterating through the container
         * @throw ElementNotExist
         */
        const T& next() {
            return mItr.next().key;
        }

        /**
         * Removes from the underlying collection the last element
         * returned by the iterator.
         * O(1)
         * @throw ElementNotExist
         */
        void remove() {
            mItr.remove();
        }
    };

    /**
     * Constructs a empty set with your own default capacity
     */
    HashSet() {
        map = new HashMap<T, bool, H>;
    }

    /**
     * Destructor
     */
    ~HashSet() { 
        delete map;
    }

    /**
     * Copy constructor
     */
    HashSet(const HashSet &c) {
        map = new HashMap<T, bool, H>(getMax(2 * c.size(), HashMap<T, bool, H>::DEFAULT_CAPCITY));
        addAll(*this, c);
    }

    /**
     * Assignment operator
     */
    HashSet& operator = (const HashSet &c) {
        *map = *(c.map);
        return *this;
    }

    /**
     * Constructs a new set containing the elements in the specified
     * collection.
     */
    template<class C> explicit HashSet(const C& c) {
        map = new HashMap<T, bool, H>(getMax(2 * c.size(), HashMap<T, bool, H>::DEFAULT_CAPCITY));
        addAll(*this, c);
    }

    /**
     * Constructs a new, empty set; the backing HashMap instance has the
     * specified capacity
     */
    HashSet(int capacity) {
        map = new HashMap<T, bool, H>(capacity);
    }

    /**
     * Adds the specified element to this set if it is not already present.
     * Returns false if element is previously in the set.
     * O(1) for average
     */
    bool add(const T& elem) {
        if (map->containsKey(elem)) return false;
        map->put(elem, true);
        return true;
    }

    /**
     * Removes all of the elements from this set.
     */
    void clear() {
        map->clear();
    }

    /**
     * Returns true if this set contains the specified element.
     * O(1) for average
     */
    bool contains(const T& elem) const {
        return map->containsKey(elem);
    }

    /**
     * Returns true if this set contains no elements.
     * O(1)
     */
    bool isEmpty() const {
        return map->isEmpty();
    }

    /**
     * Returns an iterator over the elements in this set.
     */
    Iterator iterator() {
        Iterator Itr;
        Itr.mItr = map->iterator();
        return Itr;
    }

    /**
     * Returns an const iterator over the elements in this set.
     */
    ConstIterator constIterator() const {
        ConstIterator CItr;
        CItr.mCItr = map->constIterator();
        return CItr;
    }

    /**
     * Removes the specified element from this set if it is present.
     * O(1) for average
     */
    bool remove(const T& elem) {
        if (!map->containsKey(elem)) return false;
        map->remove(elem);
        return true;
    }

    /**
     * Returns the number of elements in this set (its cardinality).
     * O(1)
     */
    int size() const {
        return map->size();
    }
};

#endif
