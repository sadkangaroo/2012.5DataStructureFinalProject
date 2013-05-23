/** @file */
#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include "Utility.h"
#include<cstdio>

/**
 * A linked list.
 *
 * The iterator iterates in the order of the elements being loaded into this list.
 */
template <class T> class LinkedList {
    private:
    class Entry {
        public:
        T data;
        Entry *next, *previous;
        Entry() {}
        Entry(T _data) {
            data = _data;
            next = previous = NULL;
        }
    };
    Entry *first, *last;
    int sz;

    Entry* getEntry(int n) {
        Entry *e;
        if (n < sz / 2) {
            e = first;
            while (n--)
                e = e->next;
        }
        else {
            e = last;
            while (++n < sz)
                e = e->previous;
        }
        return e;
    }

    void removeEntry(Entry *e) {
        sz--;
        if (sz == 0) first = last = NULL;
        else {
            if (e == first) {
                first = first->next;
                first->previous = NULL;
                delete e;
            }
            else if (e == last) {
                last = last->previous;
                last->next = NULL;
                delete e;
            }
            else {
                e->next->previous = e->previous;
                e->previous->next = e->next;
                delete e;
            }
        }
    }

    void addLastEntry(Entry *e) {
       if (sz == 0) first = last = e;
       else {
           e->previous = last;
           last->next = e;
           last = e;
       }
       sz++;
    }

    std::string toString(int x) const {
        char s[20];
        sprintf(s, "%d", x);
        return std::string(s);
    }

    void checkBoundInclusive(int index) const {
        if (index < 0 || index > sz)
            throw IndexOutOfBound("\nIndex out of Bound >>> Index: " + toString(index) + ", Size: " + toString(sz) + "\n");
    }

    void checkBoundExclusive(int index) const {
        if (index < 0 || index >= sz) 
            throw IndexOutOfBound("\nIndex out of Bound >>> Index: " + toString(index) + ", Size: " + toString(sz) + "\n");
    }

    public:
    class ConstIterator {
        private:
        Entry *cnt, *lastRet;

        public:
        void init(Entry* _cnt) {
            cnt = _cnt; lastRet = NULL;
        }
        /**
         * Returns true if the iteration has more elements.
         * O(1)
         */
        bool hasNext() { 
            return (cnt != NULL);
        }

        /**
         * Returns the next element in the iteration.
         * O(1)
         * @throw ElementNotExist
         */
        T& next() { 
            if (cnt == NULL)
                throw ElementNotExist("\nNo Such Element\n");
            lastRet = cnt; cnt = cnt->next;
            return lastRet->data;
        }
    };

    class Iterator {
        private:
        LinkedList *lnk;
        Entry *cnt, *lastRet;

        public:
        void init(Entry* _cnt, LinkedList* _lnk) {
            cnt = _cnt; lnk = _lnk; lastRet = NULL;
        }
        /**
         * Returns true if the iteration has more elements.
         * O(1)
         */
        bool hasNext() { 
            return (cnt != NULL);
        }

        /**
         * Returns the next element in the iteration.
         * O(1)
         * @throw ElementNotExist
         */
        T& next() { 
            if (cnt == NULL)
                throw ElementNotExist("\nNo Such Element\n");
            lastRet = cnt; cnt = cnt->next;
            return lastRet->data;
        }

        /**
         * Removes from the underlying collection the last element returned by the iterator (optional operation).
         * O(n)
         * @throw ElementNotExist
         */
        void remove() { 
            if (lastRet == NULL)
                throw ElementNotExist("\nNo such Element\n");
            lnk->removeEntry(lastRet);
            lastRet = NULL;
        }
    };

    void init() {
        sz = 0;
        first = last = NULL;
    }

    /**
     * Constructs an empty list
     */
    LinkedList() {
        init();
    }

    /**
     * Copy constructor
     * You may utilize the ``addAll'' function from Utility.h
     */
    LinkedList(const LinkedList<T> &c) {
        init();
        addAll(*this, c);
    }

    /**
     * Assignment operator
     * You may utilize the ``addAll'' function from Utility.h
     */
    LinkedList<T>& operator = (const LinkedList<T> &c) {
        clear();
        addAll(*this, c);
        return *this;
    }

    /**
     * Constructs a list containing the elements of the specified
     * collection, in the order they are returned by the collection's
     * const iterator.
     * You may utilize the ``addAll'' function from Utility.h
     */
    template <class C> LinkedList(const C& c) {
        init();
        addAll(*this, c);
    }

    /**
     * Desturctor
     */
    ~LinkedList() {
       clear(); 
    }

    /**
     * Inserts the specified element at the specified position in
     * this list.
     * O(n)
     * @throw IndexOutOfBound exception when index is out of bound
     */
    void add(int index, const T& elem) {
        checkBoundInclusive(index);
        Entry *e = new Entry(elem);
        if (index < sz) {
            Entry *after = getEntry(index);
            e->next = after;
            e->previous = after->previous;
            if (after->previous == NULL)
                first = e;
            else after->previous->next = e;
            after->previous = e;
            sz++;
        }
        else addLastEntry(e);
    }

    /**
     * Appends the specified element to the end of this list.
     * O(1).
     * Always returns true;
     */
    bool add(const T& elem) {
        addLastEntry(new Entry(elem));
        return true;
    }

    /**
     * Inserts the specified element at the beginning of this list.
     * O(1).
     */
    void addFirst(const T& elem) {
        add(0, elem);
    }

    /**
     * Removes all of the elements from this list.
     * O(n).
     */
    void clear() {
        while (sz) removeEntry(first);
    }

    /**
     * Returns true if this list contains the specified element.
     * O(n).
     */
    bool contains(const T& elem) const {
        Entry *e = first;
        while (e != NULL) {
            if (e->data == elem) return true;
            e = e->next;
        }
        return false;
    }

    /**
     * Returns a reference to the element at the specified position.
     * O(n).
     * @throw IndexOutOfBound exception when index is out of bound
     */
    T& get(int index) {
        checkBoundExclusive(index);
        return getEntry(index)->data;
    }

    /**
     * Returns a const reference to the element at the specified position.
     * O(n).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const {
        checkBoundExclusive(index);
        return getEntry(index)->data;
    }

    /**
     * Returns a reference to the first element.
     * O(1).
     * @throw ElementNotExist
     */
    T& getFirst() {
        if (sz == 0) throw ElementNotExist("\nNo Such Element\n");
        return first->data;
    }

    /**
     * Returns a const reference to the first element.
     * O(1).
     * @throw ElementNotExist
     */
    const T& getFirst() const {
        if (sz == 0) throw ElementNotExist("\nNo Such Element\n");
        return first->data;
    }

    /**
     * Returns a reference to the last element.
     * O(1).
     * @throw ElementNotExist
     */
    T& getLast() {
        if (sz == 0) throw ElementNotExist("\nNo Such Element\n");
        return last->data;
    }

    /**
     * Returns a const reference to the last element.
     * O(1).
     * @throw ElementNotExist
     */
    const T& getLast() const {
        if (sz == 0) throw ElementNotExist("\nNo Such Element\n");
        return last->data;
    }

    /**
     * Returns the index of the first occurrence of the specified element
     * O(1).
     * in this list, or -1 if this list does not contain the element.
     */
    int indexOf(const T& elem) const {
        int index = 0;
        Entry *e = first;
        while (e != NULL) {
            if (elem == e->data) return index;
            index++; e = e->next;
        }
        return -1;
    }

    /**
     * Returns true if this list contains no elements.
     * O(1).
     */
    bool isEmpty() const {
        return sz == 0;
    }

    /**
     * Returns an iterator
     * O(1).
     */
    Iterator iterator() {
        Iterator Itr;
        Itr.init(first, this);
        return Itr;
    }

    /**
     * Returns an const iterator
     * O(1).
     */
    ConstIterator constIterator() const {
        ConstIterator CItr;
        CItr.init(first);
        return CItr;
    }

    /**
     * Removes the element at the specified position in this list.
     * O(n).
     * @throw IndexOutOfBound exception when index is out of bound
     */
    T removeIndex(int index) {
        checkBoundExclusive(index);
        Entry *e = getEntry(index);
        T res = e->data;
        removeEntry(e);
        return res;
    }

    /**
     * Removes the first occurrence of the specified element from this
     * O(n).
     * list, if it is present.
     */
    bool remove(const T& elem) {
        Entry *e = first;
        while (e != NULL) {
            if (elem == e->data) {
                removeEntry(e);
                return true;
            }
            e = e->next;
        }
        return false;
    }

    /**
     * Removes and returns the first element from this list.
     * O(1).
     * @throw ElementNotExist
     */
    T removeFirst() {
        if (sz == 0) throw ElementNotExist("\nNo Such Element\n");
        sz--;
        T res = first->data;
        if (first->next != NULL)
            first->next->previous = NULL;
        else last = NULL;
        first = first->next;
        return res;
    }

    /**
     * Removes and returns the last element from this list.
     * O(1).
     * @throw ElementNotExist
     */
    T removeLast() {
        if (sz == 0) throw ElementNotExist("\nNo Such Element\n");
        sz--;
        T res = last->data;
        if (last->previous != NULL) 
            last->previous->next = NULL;
        else first = NULL;
        last = last->previous;
        return res;
    }

    /**
     * Replaces the element at the specified position in this list with
     * the specified element.
     * O(n).
     * @throw IndexOutOfBound exception when index is out of bound
     */
    T set(int index, const T& elem) {
        checkBoundExclusive(index);
        Entry *e = getEntry(index);
        T old = e->data;
        e->data = elem;
        return old;
    }

    /**
     * Returns the number of elements in this list.
     */
    int size() const {
        return sz;
    }

    /**
     * Returns a view of the portion of this list between the specified
     * fromIndex, inclusive, and toIndex, exclusive.
     * O(n).
     * @throw IndexOutOfBound
     */
    LinkedList<T> subList(int fromIndex, int toIndex) {
        checkBoundExclusive(fromIndex);
        checkBoundInclusive(toIndex);
        int len = toIndex - fromIndex;
        if (len >= 0) {
            LinkedList res;
            Entry *e = getEntry(fromIndex);
            for (int i = 0; i < len; ++i) {
                res.add(e->data);
                e = e->next;
            }
            return res;
        }
        else {
            throw IndexOutOfBound("\nIllegal Segment\n");
        }
    }
};
#endif
