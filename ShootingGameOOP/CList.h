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
        // ü�� ���, ����Ʈ�� �ٶ󺸵��� (���߿� �߰�) 
        Node* _node;
    public:
        iterator(Node* node = nullptr)
        {
            // ���ڷ� ���� Node �����͸� ����
            _node = node;
        }
        iterator operator++(int)
        {
            // ���� ��带 ���� ���� �̵�
            // Node* tempNode = _node;
            iterator tempiter = _node;
            _node = _node->_next;
            return tempiter;
        }
        iterator& operator++()
        {
            // ���� �߻� 
            _node = _node->_next;
            return *this;
        }
        iterator operator--(int)
        {
            // ���� ��带 ���� ���� �̵�
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
        // ����Ʈ�� _size�� 0�� �ƴ϶�� clear()
        if (empty())
        {
            clear();
        }
    }
    CList(const CList& other)
    {
        // ���������
        // ���� ���� ?
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
        // T�� ���������� ���� ���ʿ� ��������ڸ� �� ȣ���ϳ�?
        /****************************************************/
    }
    CList& operator=(const CList& other)
    {
        // ��ü ��ü�� �Ȱ��� ����� ������� ��ü ��ü�� ��ȯ
        // ���� ������ �����ε�
        // ���� ����
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

        // ��ü �ڽ��� ����Ű�� ���۷����� ��ȯ
        return *this;
    }
    iterator begin()
    {
        // ����Ʈ�� ù��° ��Ҹ� ��ȯ
        return _head._next;
    }
    iterator end()
    {
        // ���ϳ����ü�� ��ȯ 
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
            // iter���� �ڵ����� ���� ��带 ����Ű�� iterator�� ��
            // �� iter�� &tail�� ����Ű�� �ݺ����� ���� 
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
    // ����Ʈ�� ��常�� erase (delete)
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