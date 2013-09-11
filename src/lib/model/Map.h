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
    public:
        Map(): map(new SpecificMap()) {};
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
        VALUE_TYPE *get(KEY_TYPE key) const {
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
        bool remove(KEY_TYPE key, bool freeMemory) {
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
        /**
         * Find element and return bool to identify found it or not.
         */
        bool found(KEY_TYPE key) const {
            SpecificMapIterator it = this->find(key);
            if (it != this->map->end()) { // found the element
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
        void clear(bool freeMemory) {
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
        };
        /**
         * Get the element count in the map.
         */
        inline uint32_t count() const {
            return this->map->size();
        };
};

DEF_NS_XCF_END

#endif /* XCF_MAP_H_ */
