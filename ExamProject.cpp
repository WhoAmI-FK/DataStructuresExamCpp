#include <iostream>
#include <stack>
#include <string>

namespace whoamifk_calc {
    template<typename T,class _Alloc=std::allocator<T>>
    class stack {
    public:
        typedef stack<T, _Alloc> _stk;
        typedef T _val_type;
        typedef _Alloc _allocator_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef std::size_t size_type;
        stack() 
            :_size(0),_stack(_Alloc().allocate(20)),_capacity(20)
        {

        }
        ~stack() {
            if (!empty()) {
                for (iterator i = begin(); i != end(); ++i) {
                    _Alloc().destroy(i);
                }
                _Alloc().deallocate(_stack, _capacity);
            }
        }
        stack(const _stk& rhs) {
            _stack = _Alloc().allocate(rhs._capacity);
            for (int i = 0; i < rhs._size; ++i) {
                _Alloc().construct(&_stack[i],rhs._stack[i]);
            }
            _size = rhs._size;
            _capacity = rhs._capacity;
        }
        _stk& operator=(const _stk& rhs) {
            if (this == &rhs) {
                throw std::invalid_argument("Impossible to assign to itself");
            }
            else {
                if (_size != 0) {
                    for (iterator i = begin(); i != end(); ++i) {
                        _Alloc().destroy(i);
                    }
                    _Alloc().deallocate(_stack, _capacity);

                }
                _stack = _Alloc().allocate(rhs._capacity);
                for (int i = 0; i < rhs._size; ++i) {
                    _Alloc().construct(&_stack[i], rhs._stack[i]);
                }
                _size = rhs._size;
                _capacity = rhs._capacity;
            }
            return *this;
        }
        bool empty() const {
            return _size == 0;
        }
        size_type size() const {
            return _size;
        }
        _val_type& top() {
            if (!empty()) {
                return _stack[_size-1];
            }
            else {
                throw std::runtime_error("Impossible to get an element from an empty stack");
            }
        }
        void pop() {
            if (!empty()) {
                --_size;
            }
            else {
                throw std::runtime_error("Trying to pop the empty stack");
            }
        }
        void push(const _val_type& _val) {
            if (++_size >= _capacity) {
                reserve(_capacity * 2);
            }
            _Alloc().construct(&_stack[_size - 1], _val);
        }
    private:
        size_type _size,_capacity;
        T* _stack;
        void reserve(size_type _cap) {
            int count = 0;
            if (_cap < _capacity) return;
            pointer buff = _Alloc().allocate(_cap);
            for (iterator i = begin(); i != end(); ++i, ++count)
                _Alloc().construct(&buff[count], *i);
            std::swap(_stack, buff);
            for (iterator i = &buff[0]; i != &buff[_size]; ++i)
                _Alloc().destroy(i);
            _Alloc().deallocate(buff, _capacity);
            _capacity = _cap;
        }
        iterator begin() {
            return &_stack[0];
        }
        iterator end() {
            return &_stack[_size];
        }
    };
    
    template<typename T>
    class Calculator {
    private:
        std::string _digitPanel;
        std::stack<T> _nums;
    //    std::stack<char> _operators;
        bool isOperator(char c) {
            return (c == '+') || (c == '-') || (c == '/') || (c == '*');
        }
        T calc(T op1, T op2, char op) {
            switch (op) {
            case '+':
                return op1 + op2;
                break;
            case '-':
                return op1 - op2;
                break;
            case '*':
                return op1 * op2;
                break;
            case '/':
                return op1 / op2;
                break;
            }
        }
    public:
        explicit Calculator()
        {

        }
        /*
        // get rid of second stack
        T evaluate(std::string _dp) {
            _digitPanel = _dp;
            char _previous_op,_cur_op;
            size_t _len = _digitPanel.length();
            std::string sub;
            for (size_t i = 0; i < _len; ++i) {
                if (_digitPanel[i] != ' ') {
                    if (_digitPanel[i] >= '0' && _digitPanel[i] <= '9') {
                        size_t j = i;
                        size_t sub_i = 0;
                        while (_digitPanel[j] != ' ' && !isOperator(_digitPanel[j])) {
                            sub[sub_i++] = _digitPanel[j++];
                        }
                        _nums.push(stoi(sub));
                        i = j;
                    }
                    else if (isOperator(_digitPanel[i])) {
                        //_operators.push(_digitPanel[i]);
                        _cur_op = _digitPanel[i];
                        _previous_op = _digitPanel[i];
                    }
                    else if (_digitPanel[i] == 'p') {
                        _operators.push(_previous_op);
                    }
                    if (!_operators.empty() && (!_nums.empty() && _nums.size() >= 2)) {
                            T op1 = _nums.top();
                            _nums.pop();
                            T op2 = _nums.top();
                            _nums.pop();
                            //char op = _operators.top();
                            char op = _cur_op;
                            _operators.pop();
                            T res = calc(op2, op1, op);
                            _nums.push(res);
                    }
                }
            }
            return _nums.top();
        }
        */
        T evaluate(std::string _dp) {
            _digitPanel = _dp;
            char _previous_op;
            size_t _len = _digitPanel.length();
            std::string sub;
            for (size_t i = 0; i < _len; ++i) {
                if (_digitPanel[i] != ' ') {
                    if (_digitPanel[i] >= '0' && _digitPanel[i] <= '9') {
                        size_t j = i;
                        size_t sub_i = 0;
                        while (_digitPanel[j] != ' ' && !isOperator(_digitPanel[j])) {
                            sub[sub_i++] = _digitPanel[j++];
                        }
                        _nums.push(stoi(sub));
                        i = j;
                    }
                    else if (isOperator(_digitPanel[i]) && (!_nums.empty() && _nums.size() >= 2)) {
                        T op1 = _nums.top();
                        _nums.pop();
                        T op2 = _nums.top();
                        _nums.pop();
                        T res = calc(op2, op1, _digitPanel[i]);
                        _nums.push(res);
                        _previous_op = _digitPanel[i];
                    }
                    else if (_digitPanel[i] == 'p') {
                        T op1 = _nums.top();
                        _nums.pop();
                        T op2 = _nums.top();
                        _nums.pop();
          
                        T res = calc(op2, op1, _previous_op);
                        _nums.push(res);
                    }
                 
                }
            }
            return _nums.top();
        }
    };
    
}

int main()
{
  
    whoamifk_calc::Calculator<int> c;
    std::cout << c.evaluate("5 5 +") << std::endl;
    std::cout << c.evaluate("8 2 /") << std::endl;
    std::cout << c.evaluate("5 4 *") << std::endl;
    std::cout << c.evaluate("5 2 3 + *") << std::endl;
    std::cout << c.evaluate("5 2 + 3 *") << std::endl;
    std::cout << c.evaluate("1 2 3 4 + + +") << std::endl;
    std::cout << c.evaluate("1 2 + 3 + 4 +") << std::endl;
    std::cout << c.evaluate("1 2 + 3 p") << std::endl;
    std::cout << c.evaluate("5 4 3 2 1 + p * p") << std::endl;

    return 0;
 }