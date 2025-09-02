#pragma once
#include <Arduino.h>

/*
#### Stack<T, I> T - element, I - index
+ clear() / size()
+ push(T) --> [ sp ]
+ *pop () <-- [++sp]
+ *head() <== [sp+1]
+ *top () <== [ sp ]
+ each() / find() / findindex()
+ erase(I) / insert_next(T*) / at(I)
*/
template <typename T, typename I>
class Stack {
protected:
  T *_stack = nullptr;
  I _size = 0;

public:
  Stack() {}
  Stack(uint16_t sp) : _stack((T *)(sp - sizeof(T))) {}

  void init(uint16_t sp) { _stack = (T *)(sp - sizeof(T)); _size = 0; }
  void clear() { _stack -= _size; _size = 0; }
  I size() { return _size; }
  void push(T data) { *_stack-- = data; _size++; }
  T *push() { T *ptr = _stack--; _size++; return ptr; }
  T *pop() { _size--; return ++_stack; }
  T *top() { return _stack; }
  T *head() { return _stack + 1; }

public:
  void each(void callback(T *ptr));
  I findindex(bool callback(T *ptr));
  T *find(bool callback(T *ptr));
  void erase(I index);
  T *insert_post(T *ptr);
  T *at(I index);
};

template <typename T, typename I>
void Stack<T, I>::each(void callback(T *ptr))
{
  I index = _size;
  T *ptr = _stack;
  while (index--) callback(++ptr);
}

template <typename T, typename I>
I Stack<T, I>::findindex(bool callback(T *ptr))
{
  I index = _size;
  T *ptr = _stack;
  while (index) {
    if (callback(++ptr)) break;
    index--;
  }
  return index;
}

template <typename T, typename I>
T *Stack<T, I>::find(bool callback(T *ptr))
{
  I index = _size;
  T *ptr = _stack;
  while (index) {
    if (callback(++ptr)) break;
    index--;
  }
  return index ? ptr : nullptr;
}

template <typename T, typename I>
void Stack<T, I>::erase(I index)
{
  if (index > _size || index == 0) return;
  T *ptr = _stack + _size - index + 1;
  index -= _size;
  while (index++) {
    *ptr = *(ptr - 1);
    ptr--;
  }
  _stack++;
  _size--;
}

template <typename T, typename I>
T *Stack<T, I>::insert_post(T *ptr)
{
  T *new_element = ptr - 1;
  ptr = _stack;
  while (ptr != new_element) {
    *ptr = *(ptr + 1);
    ptr++;
  }
  _stack--;
  _size++;
  return new_element;
}

template <typename T, typename I>
T *Stack<T, I>::at(I index)
{
  if (index > _size || index == 0) return nullptr;
  return _stack + _size - index + 1;
}
