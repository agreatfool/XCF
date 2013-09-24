#ifndef XCF_DEQUE_H_
#define XCF_DEQUE_H_

#include "../../Common.h"

#include <deque>

DEF_NS_XCF_BEGIN

template <typename VALUE_TYPE>
class Deque {
    protected:
        typedef typename std::deque<VALUE_TYPE *>             SpecificDeque;
        typedef typename std::deque<VALUE_TYPE *>::iterator   SpecificDequeIterator;
        SpecificDeque *deque;
        ThreadLock    *lock;
    public:
        Deque(): deque(new SpecificDeque()), lock(NULL) {
            this->lock = (ThreadLock *) malloc(sizeof(ThreadLock));
            pthread_mutex_init(this->lock, NULL);
        };
        virtual ~Deque() {};
        /**
         * Get the std::deque itself.
         */
        inline SpecificDeque *getDeque() {
            return this->deque;
        };
        /**
         * Get element from deque.
         */
        inline VALUE_TYPE *get(uint64_t pos) const {
            if (this->deque->size() <= pos) {
                return NULL;
            } else {
                return this->deque->at(pos);
            }
        };
        /**
         * Push element to the end of deque.
         */
        inline void pushBack(VALUE_TYPE *value) {
            this->deque->push_back(value);
        };
        /**
         * Pop the last element out of the deque.
         */
        inline VALUE_TYPE *popBack() {
            VALUE_TYPE * value = this->deque->back();
            this->deque->pop_back();
            return value;
        };
        /**
         * Push element to the front of deque.
         */
        inline void pushFront(VALUE_TYPE *value) {
            this->deque->push_front(value);
        };
        /**
         * Pop the first element out of the deque.
         */
        inline VALUE_TYPE *popFront() {
            VALUE_TYPE * value = this->deque->front();
            this->deque->pop_front();
            return value;
        };
        /**
         * Find element with the same value in deque.
         * And return the iterator pointing to it
         */
        inline SpecificDequeIterator find(VALUE_TYPE *value) const {
            return std::find(this->deque->begin(), this->deque->end(), value);
        };
        /**
         * Find element and return bool to identify found it or not.
         */
        inline bool found(VALUE_TYPE *value) {
            if (this->find(value) != this->deque->end()) {
                return true;
            } else {
                return false;
            }
        };
        /**
         * Remove the element by pos. And free memory.
         */
        inline bool remove(uint64_t pos) {
            return this->remove(pos, true);
        };
        /**
         * Remove the element by pos. And delete the pointer of the element if "freeMemory" is true.
         */
        inline bool remove(uint64_t pos, bool freeMemory) {
            pthread_mutex_lock(this->lock);
            if (this->deque->size() <= pos) {
                pthread_mutex_unlock(this->lock);
                return false;
            } else {
                if (freeMemory) {
                    delete this->get(pos);
                }
                this->deque->erase(
                    this->deque->begin() + pos
                );
                pthread_mutex_unlock(this->lock);
                return true;
            }
        };
        /**
         * Clear elements and free memory.
         */
        inline void clear() {
            this->clear(true);
        };
        /**
         * Clear elements and free memory if "freeMemory" is true.
         */
        inline void clear(bool freeMemory) {
            pthread_mutex_lock(this->lock);
            if (!this->deque->empty()) {
                SpecificDequeIterator it;
                if (freeMemory) {
                    for (it = this->deque->begin(); it != this->deque->end(); ++it) {
                        delete *it;
                    }
                }
                this->deque->clear();
            }
            pthread_mutex_unlock(this->lock);
        };
        /**
         * Get the element count in the deque.
         */
        inline uint64_t count() const {
            return this->deque->size();
        };
        /**
         * Check the deque is empty or not.
         */
        inline bool empty() const {
            return this->deque->empty();
        };
};

DEF_NS_XCF_END

#endif /* XCF_DEQUE_H_ */
