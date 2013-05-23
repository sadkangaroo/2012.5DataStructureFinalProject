/** @file */

#ifndef __TREESET_H
#define __TREESET_H

#include "Utility.h"
#include "TreeMap.h"

/**
 * A set implemented by balanced tree,
 * the elements being putted must guarantee operator'<'
 *
 * The iterator must iterates in the order defined by the operator'<' (from the smallest to the biggest)
 */

template <class E>
class TreeSet {
    private:
        TreeMap<E, bool>* map;
    public:
    class ConstIterator {
        public:
        typename TreeMap<E, bool>::ConstIterator mCItr;
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
        const E& next() {
            return mCItr.next().key;
        }
    };

    class Iterator {
        public:
        typename TreeMap<E, bool>::Iterator mItr;
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
        const E& next() {
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
     * Constructs a new, empty tree set, sorted according to the natural ordering of its elements.
     */
    TreeSet() { 
        map = new TreeMap<E, bool>;
    }

    /**
     * Constructs a set containing the elements of the specified collection, in
     * the order they are returned by the collection's iterator.
     */
    template <class E2>
    explicit TreeSet(const E2& x) { 
        map = new TreeMap<E, bool>;
        addAll(*this, x);
    }

    /**
     * Destructor
     */
    ~TreeSet() { 
        delete map;
    }

    /**
     * Assignment operator
     */
    TreeSet& operator = (const TreeSet& x) { 
        *map = *(x.map);
        return *this;
    }

    /**
     * Copy-constructor
     */
    TreeSet(const TreeSet& x) { 
        map = new TreeMap<E, bool>;
        addAll(*this, x);
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
     * Adds the specified element to this set if it is not already present.
     * Returns true if this set did not already contain the specified element.
     * O(logn)
     */
    bool add(const E& e) { 
        if (map->containsKey(e)) return false;
        map->put(e, true);
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
     * O(logn)
     */
    bool contains(const E& e) const { 
        return map->containsKey(e);
    }

    /**
     * Returns a const reference to the first (lowest) element currently in this set.
     * O(logn)
     * @throw ElementNotExist
     */
    const E& first() const { 
        if (map->size() == 0) throw ElementNotExist("\nNo Such Element\n");
        return map->firstKey();
    }

    /**
     * Returns true if this set contains no elements.
     * O(1)
     */
    bool isEmpty() const { 
        return map->isEmpty();
    }

    /**
     * Returns a const reference to the last (highest) element currently in this set.
     * O(logn)
     * @throw ElementNotExist
     */
    const E& last() const { 
        if (map->size() == 0) throw ElementNotExist("\nNo Such Element\n");
        return map->lastKey();
    }

    /**
     * Removes the specified element from this set if it is present.
     * O(logn)
     */
    bool remove(const E& e) { 
        if (!map->containsKey(e)) return false;
        map->remove(e);
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
