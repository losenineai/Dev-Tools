#include <string.h>
#include <stdlib.h>
#include "dx_anti_stee.h"

template<class T>
class DXArrayList
{
public:
    T* _array;
    int _capacity;
    int _currentNum;

    STEE
    DXArrayList()
    {
        this->_array = nullptr;
        this->_capacity = 0;
        this->_currentNum = 0;
    }

    STEE
    ~DXArrayList()
    {
        if (this->_array != nullptr) {
            free(this->_array);
        }
    }

    STEE
    bool ensureCapacity(int capacity)
    {
        if (this->_capacity < capacity)
        {
            T* oldArray = this->_array;
            T* newArray = new T[capacity];

            if (newArray == nullptr) {
                return false;
            }

            memset((void* )newArray, 0, sizeof(T) * capacity);
            if (this->_currentNum > 0)
            {
                memcpy((void* )newArray,
                       (void* )oldArray,
                       sizeof(T) * this->_currentNum);
            }

            if (oldArray)
            {
                delete[] oldArray;
            }

            this->_array = newArray;
            this->_capacity = capacity;
        }
        return true;
    }

    STEE
    void add(T* pValue)
    {
        bool flag = true;

        if (this->_currentNum == this->_capacity)
        {
            int newSize = this->_capacity + 40;
            flag = ensureCapacity(newSize);
        }

        if (flag) {
            this->_array[this->_currentNum] = *pValue;
            this->_currentNum++;
        }
    }

    STEE
    T remove(int index)
    {
        T elementItem = this->_array[index];

        this->_currentNum--;
        if (this->_currentNum > index)
        {
            memcpy((void* )&this->_array[index],
                   (void* )&this->_array[index + 1],
                   sizeof(T) * (this->_currentNum - index));
        }

        return elementItem;
    }
};
