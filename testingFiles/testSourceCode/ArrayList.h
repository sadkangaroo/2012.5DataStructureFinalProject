/** @file */
#ifndef __ARRAYLIST_H
#define __ARRAYLIST_H

#include "Utility.h"
#include<cstring>
#include<cstdio>

/**
 * The ArrayList is just like vector in C++.
 * You should know that "capacity" here doesn't mean how many elements are now in this list, it means
 * the length of the array of your inner implemention
 * For example, even if the capacity is 10, the method "isEmpty()" may still return true.
 *
 * The iterator iterates in the order of the elements being loaded into this list
 */
template <class E>
class ArrayList {
    private:
    static const int DEFAULT_CAPACITY = 10;
    int sz, cap;
    E* data;

    int getMax(int x, int y) {
        if (x > y) return x;
        return y;
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
        int pos, size, last;
        const ArrayList *arr;

        public:
        void init(const ArrayList<E> *_arr, int _pos, int _size, int _last) {
            arr = _arr; pos = _pos; size = _size; last = _last;
        }

        /**
         * Returns true if the iteration has more elements.
         * O(1)
         */
        bool hasNext() { 
            return pos < size;
        }

        /**
         * Returns the next element in the iteration.
         * O(1)
         * @throw ElementNotExist
         */
        const E& next() { 
            if (pos == size)
                throw ElementNotExist("\nNo Such Element\n");
            last = pos;
            return arr->get(pos++);
        }
    };

    class Iterator {
        private:
        int pos, size, last;
        ArrayList* arr;

        public:
        void init(ArrayList<E> *_arr, int _pos, int _size, int _last) {
            arr = _arr; pos = _pos; size = _size; last = _last;
        }

        /**
         * Returns true if the iteration has more elements.
         * O(1)
         */
        bool hasNext() { 
            return pos < size;
        }

        /**
         * Returns the next element in the iteration.
         * O(1)
         * @throw ElementNotExist
         */
        E& next() { 
            if (pos == size)
                throw ElementNotExist("\nNo Such Element\n");
            last = pos;
            return arr->get(pos++);
        }

        /**
         * Removes from the underlying collection the last element returned by the iterator (optional operation).
         * O(n)
         * @throw ElementNotExist
         */
        void remove() { 
            if (last < 0)
                throw ElementNotExist("\nIllegal State\n");
            arr->removeIndex(last);
            pos--; size--; last = -1;
        }
    };

    void init(int initialCapacity) {
        cap = initialCapacity;
        data = new E[cap];
        sz = 0;
    }

    /**
     * Constructs an empty list with the specified initial capacity.
     */
    /**
     * Constructs an empty list with an initial capacity of ten.
     */
    ArrayList(int initialCapacity = DEFAULT_CAPACITY) { 
        init(initialCapacity);
    }

    /**
     * Constructs a list containing the elements of the specified collection, in
     * the order they are returned by the collection's iterator.
     */
    template <class E2>
    explicit ArrayList(const E2& x) { 
        init(x.size());
        addAll(*this, x);
    }

    /**
     * Destructor
     */
    ~ArrayList() { 
        delete [] data;
    }

    /**
     * Assignment operator
     */
    ArrayList& operator = (const ArrayList& x) { 
        clear();
        addAll(*this, x);
        return *this;
    }

    /**
     * Copy-constructor
     */
    ArrayList(const ArrayList& x) { 
        init(x.size());
        addAll(*this, x);
    }

    /**
     * Returns an iterator over the elements in this list in proper sequence.
     */
    Iterator iterator() { 
        Iterator Itr;
        Itr.init(this, 0, size(), -1);
        return Itr;
    }

    /**
     * Returns an CONST iterator over the elements in this list in proper sequence.
     */
    ConstIterator constIterator() const { 
        ConstIterator CItr;
        CItr.init(this, 0, size(), -1);
        return CItr;
    }

    /**
     * Appends the specified element to the end of this list.
     * O(1)
     */
    bool add(const E& e) { 
        if (sz == cap) ensureCapacity(sz + 1);
        data[sz++] = e;
        return true;
    }

    /**
     * Inserts the specified element at the specified position in this list.
     * The range of index is [0, size].
     * O(n)
     * @throw IndexOutOfBound
     */

    void add(int index, const E& element) { 
        checkBoundInclusive(index);
        if (sz == cap)
            ensureCapacity(sz + 1);
        if (index != sz)
            memmove(data + index + 1, data + index, (sz - index) * sizeof(E));
        data[index] = element;
        sz++;
    }

    /**
     * Removes all of the elements from this list.
     */
    void clear() {
        sz = 0;
    }

    /**
     * Returns true if this list contains the specified element.
     * O(n)
     */
    bool contains(const E& e) const { 
        return indexOf(e) != -1;
    }

    /**
     * Increases the capacity of this ArrayList instance, if necessary, to ensure that it can hold at least the number of elements specified by the minimum capacity argument.
     */
    void ensureCapacity(int minCapacity) { 
        if (minCapacity > cap) {
            E* newData = new E[cap = getMax(cap * 2, minCapacity)];
            memmove(newData, data, sz * sizeof(E));
            delete [] data;
            data = newData;
        }
    }

    /**
     * Returns a reference to the element at the specified position in this list.
     * O(1)
     * @throw IndexOutOfBound
     */
    E& get(int index) { 
       checkBoundExclusive(index);
       return data[index];
    }

    /**
     * Returns a const reference to the element at the specified position in this list.
     * O(1)
     * @throw IndexOutOfBound
     */
    const E& get(int index) const { 
       checkBoundExclusive(index);
       return data[index];
    }

    /**
     * Returns the index of the first occurrence of the specified element in this list, or -1 if this list does not contain the element.
     * O(n)
     */
    int indexOf(const E& e) const {
        for (int i = 0; i < sz; ++i)
            if (e == data[i]) return i;
        return -1;
    }

    /**
     * Returns true if this list contains no elements.
     * O(1)
     */
    bool isEmpty() const { 
        return sz == 0;
    }

    /**
     * Returns the index of the last occurrence of the specified element in this list, or -1 if this list does not contain the element.
     * O(n)
     */
    int lastIndexOf(const E& e) const {
        for (int i = sz - 1; i >= 0; --i) 
            if (e == data[i]) return i;
        return -1;
    }

    /**
     * Removes the element at the specified position in this list.
     * Returns the element that was removed from the list.
     * O(n)
     * @throw IndexOutOfBound
     */
    E removeIndex(int index) {
        checkBoundExclusive(index);
        E r = data[index];
        if (index != --sz) 
           memmove(data + index, data + index + 1, (sz - index) * sizeof(E)); 
        return r;
    }

    /**
     * Removes the first occurrence of the specified element from this list, if it is present.
     * O(n)
     */
    bool remove(const E& e) { 
        int pos = indexOf(e);
        if (pos == -1) return false;
        removeIndex(pos); return true;
    }

    /**
     * Removes from this list all of the elements whose index is between fromIndex, inclusive, and toIndex, exclusive.
     * O(n)
     * @throw IndexOutOfBound
     */
    void removeRange(int fromIndex, int toIndex) {
        checkBoundExclusive(fromIndex);
        checkBoundInclusive(toIndex);
        int change = toIndex - fromIndex;
        if (change > 0) {
            memmove(data + fromIndex, data + toIndex, (sz - toIndex) * sizeof(E));
            sz -= change;
        }
        else if (change < 0) {
            throw IndexOutOfBound("\nIllegal Segment\n");
        }
    }

    /**
     * Replaces the element at the specified position in this list with the specified element.
     * Returns the element previously at the specified position.
     * O(1)
     * @throw IndexOutOfBound
     */
    E set(int index, const E& element) {
        checkBoundExclusive(index);
        E r = data[index];
        data[index] = element;
        return r;
    }

    /**
     * Returns the number of elements in this list.
     * O(1)
     */
    int size() const { 
        return sz;
    }

    /**
     * Returns a view of the portion of this list between the specified fromIndex, inclusive, and toIndex, exclusive.
     * O(n)
     * @throw IndexOutOfBound
     */
    ArrayList subList(int fromIndex, int toIndex) const {
        checkBoundExclusive(fromIndex);
        checkBoundInclusive(toIndex);
        int len = toIndex - fromIndex;
        if (len >= 0) {
            ArrayList res(len);
            for (int i = fromIndex; i < toIndex; ++i)
                res.add(data[i]);
            return res;
        }
        else {
            throw IndexOutOfBound("\nIllegal Segment\n");
        }
    }
};
#endif
