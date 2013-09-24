#ifndef XCF_QUEUE_H_
#define XCF_QUEUE_H_

#include "../../Common.h"

#include <queue>

DEF_NS_XCF_BEGIN

template <typename VALUE_TYPE>
class Queue {
    protected:
        typedef typename std::queue<VALUE_TYPE *> SpecificQueue;
        SpecificQueue *queue;
        ThreadLock    *lock;
    public:
        Queue(): queue(new SpecificQueue()), lock(NULL) {
            this->lock = (ThreadLock *) malloc(sizeof(ThreadLock));
            pthread_mutex_init(this->lock, NULL);
        };
        virtual ~Queue() {};
        /**
         * Get the std::queue itself.
         */
        inline SpecificQueue *getQueue() {
            return this->queue;
        };
        /**
         * Push element to the end of queue.
         */
        inline void push(VALUE_TYPE *value) {
            this->queue->push(value);
        };
        /**
         * Pop the first element out of the queue.
         */
        inline VALUE_TYPE *pop() {
            VALUE_TYPE * value = this->queue->front();
            this->queue->pop();
            return value;
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
            while (!this->queue->empty()) {
                if (freeMemory) {
                    VALUE_TYPE *first = this->queue->front();
                    delete first;
                }
                this->queue->pop();
            }
            pthread_mutex_unlock(this->lock);
        };
        /**
         * Get the element count in the queue.
         */
        inline uint64_t count() const {
            return this->queue->size();
        };
        /**
         * Check the queue is empty or not.
         */
        inline bool empty() const {
            return this->queue->empty();
        };
};

DEF_NS_XCF_END

#endif /* XCF_QUEUE_H_ */
