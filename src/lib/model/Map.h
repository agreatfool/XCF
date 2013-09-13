#ifndef XCF_MAP_H_
#define XCF_MAP_H_

#include "../../Common.h"

#include <map>

DEF_NS_XCF_BEGIN

template <typename KEY_TYPE, typename VALUE_TYPE>
class Map {
    protected:
        typedef typename std::map<KEY_TYPE, VALUE_TYPE *>             SpecificMap;
        typedef typename std::map<KEY_TYPE, VALUE_TYPE *>::iterator   SpecificMapIterator;
        typedef typename std::map<KEY_TYPE, VALUE_TYPE *>::value_type SpecificValueType;
        typedef typename std::pair<SpecificMapIterator, bool>         SpecificPair;
        SpecificMap *map;
        ThreadLock  *lock;
    public:
        Map(): map(new SpecificMap()), lock(ThreadUtil::createLock()) {};
        virtual ~Map() {};
        /**
         * Get the std::map itself.
         */
        inline SpecificMap *getMap() {
            return this->map;
        };
        /**
         * Get element from map.
         */
        inline VALUE_TYPE *get(KEY_TYPE key) const {
            SpecificMapIterator it = this->find(key);
            if (it != this->map->end()) {
                return it->second;
            } else {
                return NULL;
            }
        };
        /**
         * Add element into map.
         */
        inline bool add(KEY_TYPE key, VALUE_TYPE *value) {
            SpecificPair ret = this->map->insert(SpecificValueType(key, value));
            return ret.second;
        };
        /**
         * Remove the element with "key" and delete the pointer of the element.
         * False returned if element corresponding to the "key" not found.
         */
        inline bool remove(KEY_TYPE key) {
            return this->remove(key, true);
        };
        /**
         * Remove the element with "key" and delete the pointer of the element if "freeMemory" is true.
         * False returned if element corresponding to the "key" not found.
         */
        inline bool remove(KEY_TYPE key, bool freeMemory) {
            SpecificMapIterator it = this->find(key);
            if (it != this->map->end()) { // found the element in the map & erase it
                if (freeMemory) {
                    delete it->second;
                }
                this->map->erase(it);
                return true;
            } else { // not found
                return false;
            }
        };
        /**
         * Find element and return the iterator pointing to it.
         */
        inline SpecificMapIterator find(KEY_TYPE key) const {
            return this->map->find(key);
        };
        inline SpecificMapIterator find(VALUE_TYPE *value) const {
            return std::find(this->map->begin(), this->map->end(), value);
        };
        /**
         * Find element and return bool to identify found it or not.
         */
        inline bool found(KEY_TYPE key) const {
            if (this->find(key) != this->map->end()) { // found the element
                return true;
            } else { // not found
                return false;
            }
        };
        inline bool found(VALUE_TYPE *value) const {
            if (this->find(value) != this->map->end()) { // found the element
                return true;
            } else { // not found
                return false;
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
            if (this->map->size() > 0) {
                SpecificMapIterator it;
                for (it = this->map->begin(); it != this->map->end(); /* no auto increment*/) {
                    if (freeMemory) {
                        delete it->second;
                    }
                    this->map->erase(it++);
                }
                this->map->clear();
            }
            ThreadUtil::unlock(this->lock);
        };
        /**
         * Get the element count in the map.
         */
        inline uint64_t count() const {
            return this->map->size();
        };
        /**
         * Check the map is empty or not.
         */
        inline bool empty() const {
            return this->map->empty();
        };
};

DEF_NS_XCF_END

#endif /* XCF_MAP_H_ */
