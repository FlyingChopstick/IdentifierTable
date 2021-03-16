#ifndef HASHTABLE_H
#define HASHTABLE_H

#endif // HASHTABLE_H
#include <string>

class HashTable
{
public:
        HashTable()
        {
                _capacity = TABLE_CAPACITY;
                _size = 0;
                resetStats();

                _keys = new unsigned int[_capacity];
                _values = new std::string[_capacity];

                for (unsigned int i = 0; i < TABLE_CAPACITY; i++)
                {
                        _keys[i] = KEY_EMPTY;
                }
        }
        HashTable(const HashTable& other)
        {
                this->_keyCompares = other._keyCompares;
                this->_valCompares = other._valCompares;
                this->_capacity = other._capacity;
                this->_size = other._size;
                this->_keys = other._keys;
                this->_values = other._values;
        }
        ~HashTable()
        {
                delete[] _keys;
                delete[] _values;
        }

        unsigned int add(std::string* element)
        {
                unsigned int hash = hashString(lowerString(element));
                unsigned int index = hash - HASH_MIN;

                _keyCompares++;
                if (_keys[index] != KEY_EMPTY)
                {
                        _valCompares++;
                        if (_values[index] != *element)
                        {
                                hash = rehash(hash);
                                index = hash - HASH_MIN;
                        }
                }

                _keys[index] = hash;
                _values[index] = *element;
                _size++;


                return hash;
        }
        void remove(std::string* element)
        {
                unsigned int hash = hashString(element);
                unsigned int index = hash - HASH_MIN;

                while (index < HASH_MAX-HASH_MIN)
                {
                        _keyCompares++;
                        if (_keys[index] == KEY_EMPTY)
                        {
                                index++;
                                continue;
                        }

                        _valCompares++;
                        if (_values[index] == *element)
                        {
                                _keys[index] = KEY_EMPTY;
                                _values[index] = "";
                                _size--;
                                return;
                        }
                        index++;
                }
                throw;
        }
        void clear()
        {
                _size = 0;

                delete[] _keys;
                delete[] _values;

                _keys = new unsigned int[_capacity];
                _values = new std::string[_capacity];
        }
        bool contains(std::string* element)
        {
                unsigned int hash = hashString(element);
                unsigned int index = hash - HASH_MIN;

                while (index < HASH_MAX)
                {
                        _keyCompares++;
                        if (_keys[index] != KEY_EMPTY)
                        {
                                _valCompares++;
                                if (_values[index] == *element)
                                {
                                        return true;
                                }
                                index++;
                        }
                        else
                        {
                                return false;
                        }
                }
                return false;
        }

        unsigned int size() { return _size; }

        void resetStats() { _keyCompares = 0; _valCompares = 0; }
        unsigned int getKeyCompares() { return _keyCompares; }
        unsigned int getValCompares() { return _valCompares; }

        static unsigned int hashString(std::string* str)
        {
                //sum of the first, 1/3rd and the last chars
                int first = (int)str->at(0);
                int second = (int)str->at(str->length() / 3);
                int third = (int)str->at(str->length() - 1);

                unsigned int hash_ = first + second + third;

                return hash_;

                //int b = 378551;
                //int a = 63689;
                //unsigned int hash = 0;
                //for (std::size_t i = 0; i < str->length(); i++)
                //{
                //	hash = hash * a + str->at(i);
                //	a *= b;
                //}
                ////set to always positive by bit masking
                //return (hash & 0x7FFFFFFF);
        }

private:
        std::string* lowerString(std::string* line)
        {
                for (int i = 0; i < line->length(); i++)
                {
                        line->at(i) = std::tolower(line->at(i));
                }
                return line;
        }
        unsigned int rehash(unsigned int hash)
        {
                int i = 1;

                //(h(A)N*i) mod N`m

                while (hash < HASH_MAX)
                {
                        hash = (hash * i) % PRIME;
                        if (_keys[hash-HASH_MIN] == KEY_EMPTY)
                        {
                                return hash;
                        }
                        i++;
                }
                throw;

                //while (++hash < HASH_MAX)
                //{
                //	int index = hash - HASH_MIN;
                //	auto t = _keys[index];

                //	_keyCompares++;
                //	if (_keys[index] == KEY_EMPTY)
                //	{
                //		return hash;
                //	}
                //	//hash++;
                //}
                //throw;
        }


        //Hash min value (offset to get the array index)
        const unsigned int HASH_MIN = 144;
        //Hash max value
        const unsigned int HASH_MAX = 366;
        //Greatest prime number <HASH_MAX
        const unsigned int PRIME = 359;
        //This value is considered an empty cell
        const unsigned int KEY_EMPTY = 400;
        //The capacity for _keys and _values
        const unsigned int TABLE_CAPACITY = 256;

        unsigned int* _keys;
        std::string* _values;
        unsigned int _size;
        unsigned int _capacity;

        unsigned int _keyCompares;
        unsigned int _valCompares;
};
