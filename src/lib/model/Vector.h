#ifndef XCF_VECTOR_H_
#define XCF_VECTOR_H_

#include "../../Common.h"

#include <vector>

DEF_NS_XCF_BEGIN

template <typename VALUE_TYPE>
class Vector {
    protected:
        typedef typename std::vector<VALUE_TYPE *>             SpecificVector;
        typedef typename std::vector<VALUE_TYPE *>::iterator   SpecificVectorIterator;
        SpecificVector *vector;
        ThreadLock     *lock;
    public:
        Vector(): vector(new SpecificVector()), lock(ThreadUtil::createLock()) {};
        virtual ~Vector() {};
        /**
         * Get the std::vector itself.
         */
        inline SpecificVector *getVector() {
            return this->vector;
        };
        /**
         * Get element from vector.
         */
        inline VALUE_TYPE *get(uint64_t pos) const {
            if (this->vector->size() <= pos) {
                return NULL;
            } else {
                return this->vector->at(pos);
            }
        };
        /**
         * Push element to the end of vector.
         */
        inline void push(VALUE_TYPE *value) {
            this->vector->push_back(value);
        };
        /**
         * Pop the last element out of the vector.
         */
        inline VALUE_TYPE *pop() {
            VALUE_TYPE * value = this->vector->back();
            this->vector->pop_back();
            return value;
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
            ThreadUtil::lock(this->lock);
            if (this->vector->size() <= pos) {
                ThreadUtil::unlock(this->lock);
                return false;
            } else {
                if (freeMemory) {
                    delete this->get(pos);
                }
                this->vector->erase(
                    this->vector->begin() + pos
                );
                ThreadUtil::unlock(this->lock);
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
            ThreadUtil::lock(this->lock);
            if (this->vector->size() > 0) {
                SpecificVectorIterator it;
                if (freeMemory) {
                    for (it = this->vector->begin(); it != this->vector->end(); ++it) {
                        delete *it;
                    }
                }
                this->vector->clear();
            }
            ThreadUtil::unlock(this->lock);
        };
        /**
         * Get the element count in the vector.
         */
        inline uint64_t count() const {
            return this->vector->size();
        };
};

DEF_NS_XCF_END

#endif /* XCF_VECTOR_H_ */
