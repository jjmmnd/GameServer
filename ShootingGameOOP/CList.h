#pragma once
#include <iostream>

template <typename T>
class CList
{
public:
    struct Node
    {
        T _data;
        Node* _prev;
        Node* _next;
    };

    class iterator
    {
    private:
        // 체인 기능, 리스트를 바라보도록 (나중에 추가) 
        Node* _node;
    public:
        iterator(Node* node = nullptr)
        {
            // 인자로 들어온 Node 포인터를 저장
            _node = node;
        }
        iterator operator++(int)
        {
            // 현재 노드를 다음 노드로 이동
            // Node* tempNode = _node;
            iterator tempiter = _node;
            _node = _node->_next;
            return tempiter;
        }
        iterator& operator++()
        {
            // 예외 발생 
            _node = _node->_next;
            return *this;
        }
        iterator operator--(int)
        {
            // 현재 노드를 다음 노드로 이동
            // Node* tempNode = _node;
            iterator tempiter(_node);
            _node = _node->_prev;
            return tempiter;
        }
        iterator& operator--()
        {
            _node = _node->_prev;
            return *this;
        }
        T& operator*()
        {
            return _node->_data;
        }
        bool operator ==(const iterator& other)
        {
            if (_node == other._node)
                return true;
            return false;
        }
        bool operator !=(const iterator& other)
        {
            if (_node != other._node)
                return true;
            return false;
        }
        Node* operator&()
        {
            return _node;
        }

        friend class CList;
    };

public:
    CList() : _size(0)
    {
        _head._data = NULL;
        _tail._data = NULL;
        _head._prev = &_head;
        _head._next = &_tail;
        _tail._prev = &_head;
        _tail._next = &_tail;
    }
    ~CList()
    {
        // 리스트의 _size가 0이 아니라면 clear()
        if (empty())
        {
            clear();
        }
    }
    CList(const CList& other)
    {
        // 복사생성자
        // 깊은 복사 ?
        _size = other._size;

        iterator iter = other._head._next;    //begin();

        Node* beforeNode = new Node;
        beforeNode->_data = iter._node->_data;
        beforeNode->_prev = &_head;

        _head._next = beforeNode;

        for (int i = 0; i < _size - 1; i++)
        {
            ++iter;
            Node* newNode = new Node;
            newNode->_data = iter._node->_data;
            newNode->_prev = beforeNode;
            beforeNode->_next = newNode;
            beforeNode = newNode;
        }

        beforeNode->_next = &_tail;
        _tail._prev = beforeNode;

        /****************************************************/
        // T가 포인터형인 경우는 애초에 복사생성자를 안 호출하나?
        /****************************************************/
    }
    CList& operator=(const CList& other)
    {
        // 객체 자체를 똑같이 만들고 만들어진 객체 자체를 반환
        // 대입 연산자 오버로딩
        // 깊은 복사
        if (_head._next != &_tail)
        {
            clear();
        }

        _size = other._size;
        iterator iter = other._head._next;    //begin();

        Node* beforeNode = new Node;
        beforeNode->_data = iter._node->_data;
        beforeNode->_prev = &_head;

        _head._next = beforeNode;

        for (int i = 0; i < _size - 1; i++)
        {
            ++iter;
            Node* newNode = new Node;
            newNode->_data = iter._node->_data;
            newNode->_prev = beforeNode;
            beforeNode->_next = newNode;
            beforeNode = newNode;
        }

        beforeNode->_next = &_tail;
        _tail._prev = beforeNode;

        // 객체 자신을 가리키는 레퍼런스를 반환
        return *this;
    }
    iterator begin()
    {
        // 리스트의 첫번째 요소를 반환
        return _head._next;
    }
    iterator end()
    {
        // 테일노드자체를 반환 
        return &_tail;
    }
    void push_front(T data)
    {
        Node* newNode = new Node;
        newNode->_data = data;
        newNode->_next = _head._next;
        newNode->_prev = &_head;

        _head._next->_prev = newNode;
        _head._next = newNode;

        ++_size;
    }
    void push_back(T data)
    {
        Node* newNode = new Node;
        newNode->_data = data;
        newNode->_prev = _tail._prev;
        newNode->_next = &_tail;

        _tail._prev->_next = newNode;
        _tail._prev = newNode;

        ++_size;
    }
    void pop_front()
    {
        Node* deleteNode = _head._next;
        _head._next = deleteNode->_next;
        deleteNode->_next->_prev = &_head;
        delete deleteNode;

        --_size;
    }
    void pop_back()
    {
        Node* deleteNode = _tail._prev;
        _tail._prev = deleteNode->_prev;
        deleteNode->_prev->_next = &_tail;
        delete deleteNode;

        --_size;
    }
    void clear()
    {
        iterator iter;
        for (iter = begin(); iter != end();)
        {
            // iter에는 자동으로 다음 노드를 가리키는 iterator가 들어감
            // 이 iter가 &tail을 가리키면 반복문을 나감 
            iter = erase(iter);
        }

        _size = 0;
        _head._next = &_tail;
        _tail._prev = &_head;
    }
    int size() { return _size; };
    bool empty()
    {
        if (_size == 0)
            return true;
        return false;
    };
    // 리스트의 노드만이 erase (delete)
    iterator erase(iterator& eraseiter)
    {
        --_size;

        iterator iter = eraseiter._node->_next;
        eraseiter._node->_prev->_next = eraseiter._node->_next;
        eraseiter._node->_next->_prev = eraseiter._node->_prev;
        delete eraseiter._node;
        return iter;
    }
    void remove(T Data)
    {
        CList<T>::iterator iter;
        for (iter = begin(); iter != end();)
        {
            if (*iter == Data)
            {
                iter = erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }
private:
    int _size;
    Node _head;
    Node _tail;
};