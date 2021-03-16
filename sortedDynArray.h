#ifndef SORTEDDYNARRAY_H
#define SORTEDDYNARRAY_H

#endif // SORTEDDYNARRAY_H
//list provides means to add, find and remove data
//on addition it searches for the correct spot of the new item using binary search
//when the item place is found, it shifts the array to the right and inserts new item
// in the freed space
//to delete item shift to the left is performed

template <class T>
class SortedList
{
public:
        SortedList()
        {
                _capacity = 0;
                _size = 0;
                _data = new T[_capacity];
                _valCompares = 0;
        }
        SortedList(const SortedList<T>& other)
        {
                this->_capacity = other._capacity;
                this->_size = other._size;
                this->_data = other._data;
                this->_valCompares = other._valCompares;
        }
        ~SortedList()
        {
                delete[] _data;
        }

        void add(T data)
        {
                if (_size >= _capacity)
                        resize();

                _data[_size] = data;
                _size++;
                sort();
        }
        T* at(int index)
        {
                return &_data[index];
        }
        void setAt(int index, T data)
        {
                if (index >= 0 && index <= _size)
                {
                        _data[index] = data;
                }
        }
        void remove(T* element)
        {
                for (int i = 0; i < _size; i++)
                {
                        _valCompares++;
                        if (_data[i] == *element)
                        {
                                shiftLeft(i);
                                return;
                        }
                }
        }
        void removeAt(int index)
        {
                shiftLeft(index);
        }
        void clear(bool resetCapacity = true)
        {
                _size = 0;
                if (resetCapacity)
                        _capacity = 1;

                delete[] _data;
                _data = new T[_capacity];
        }
        bool contains(T element)
        {
                return indexOf(&element) != -1;
        }
        int indexOf(T* element)
        {
                int start = 0;
                int end = (_size);

                while (start < end)
                {
                        int middle = (start + end) / 2;

                        _valCompares++;
                        if (*element == _data[middle])
                        {
                                return middle;
                        }
                        _valCompares++;
                        if (*element < _data[middle])
                        {
                                end = middle;
                        }
                        else
                        {
                                start = middle + 1;
                        }
                }

                return -1;
        }

        unsigned int size() { return _size; }
        unsigned int capacity() { return _capacity; }
        unsigned int getValCompares() { return _valCompares; }
        void resetStats() { _valCompares = 0; }

private:
        void sort()
        {
                if (_size <= 1)
                {
                        return;
                }

                T key;
                int j;

                for (unsigned int i = 0; i < _size; i++)
                {
                        key = _data[i];
                        j = i - 1;

                        while (j >= 0 && _data[j] > key)
                        {
                                _valCompares++;
                                _data[j + 1] = _data[j];
                                j--;
                        }
                        _data[j + 1] = key;
                }
        }
        void resize()
        {
                int newCapacity;
                if (_capacity <= 0)
                {
                        _capacity = 1;
                        _data = new T[_capacity];
                        return;
                }
                else
                        newCapacity = _capacity * 2;

                T* newData = new T[newCapacity];
                for (unsigned int i = 0; i < _size; i++)
                {
                        newData[i] = _data[i];
                }

                delete[] _data;
                _data = newData;
                _capacity = newCapacity;

        }
        void shiftLeft(int index)
        {
                if (index < _capacity)
                {
                        int counter = 0;
                        T* newData = new T[_capacity];
                        for (int i = 0; i < _size; i++)
                        {
                                if (i != index)
                                {
                                        newData[counter] = _data[i];
                                        counter++;
                                }
                        }

                        delete[] _data;
                        _data = newData;
                        _size = counter;
                }
        }

        T* _data;
        unsigned int _size;
        unsigned int _capacity;

        unsigned int _valCompares;
};
