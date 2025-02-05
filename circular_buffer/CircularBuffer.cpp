#include "CircularBuffer.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <initializer_list>

CircularBuffer::CircularBuffer() {
  capacity_ = 0;
  try {
    buffer_ = new value_type[capacity_];
  }
  catch(const std::bad_alloc& e) {
    std::cerr << e.what();
  }
  front_ = 0;
  back_ = 0;
  size_ = 0;
}

CircularBuffer::~CircularBuffer() {
  front_ = 0;
  back_ = 0;
  capacity_ = 0;
  size_ = 0;
  delete[] buffer_;
}

CircularBuffer::CircularBuffer(const CircularBuffer & cb) {
  try {
    this->buffer_ = new value_type[cb.capacity_];
  }
  catch(const std::bad_alloc& e) {
    std::cerr << e.what();
  }
  this->capacity_ = cb.capacity_;
  this->front_ = cb.front_;
  this->back_ = cb.back_;
  this->size_ = cb.size_;
  std::memcpy(cb.buffer_, this->buffer_, sizeof(cb.buffer_));
}

CircularBuffer::CircularBuffer(int capacity){
  try {
    buffer_ = new value_type[capacity];
  }
  catch(const std::bad_alloc& e) {
    std::cerr << e.what();
  }
  front_ = 0;
  back_ = 0;
  capacity_ = capacity;
  size_ = 0;
}

CircularBuffer::CircularBuffer(int capacity, const value_type & elem) {
  try {
    buffer_ = new value_type[capacity];
  }
  catch(const std::bad_alloc& e) {
    std::cerr << e.what();
  }
  front_ = 0;
  back_ = 0;
  capacity_ = capacity;
  size_ = capacity;
 std::memset(buffer_, elem, sizeof(value_type));
}

CircularBuffer::CircularBuffer(std::initializer_list<value_type> init_list) {
  capacity_ = init_list.size();
  size_ = init_list.size();
  front_ = 0;
  back_ = capacity_ - 1;
  try {
    buffer_ = new value_type[capacity_];
  }
  catch (const std::bad_alloc& e) {
    std::cerr << e.what();
  }
  std::memcpy(buffer_, init_list.begin(), capacity_ * sizeof(value_type));
}

value_type & CircularBuffer::operator[](int i) {
  i = i % capacity_;
  return buffer_[i];
}

const value_type & CircularBuffer::operator[](int i) const {
  i = i % capacity_;
  return buffer_[i];
}

value_type & CircularBuffer::at(int i) {
  if (i < 0 or i > capacity_) {
    throw std::out_of_range("Invalid index."); 
  }
  else {
    return buffer_[i];
  }
}

const value_type & CircularBuffer::at(int i) const {
  if (i < 0 or i > capacity_) {
    throw std::out_of_range("Invalid index.");
  }
  else {
    return buffer_[i];
  }
}

value_type & CircularBuffer::front() {
  return buffer_[front_];
}
value_type & CircularBuffer::back() {
  return buffer_[(back_ - 1 + capacity_) % capacity_];
}
const value_type & CircularBuffer::front() const {
  return buffer_[front_];
}
const value_type & CircularBuffer::back() const {
  return buffer_[(back_ - 1 + capacity_) % capacity_];
}

int CircularBuffer::front_index() {
  return front_;
}

int CircularBuffer::back_index() {
  return back_;
}

value_type * CircularBuffer::linearize() {
  value_type * new_buffer;
  try {
    new_buffer = new value_type[capacity_];
  }
  catch(const std::bad_alloc& e) {
    std::cerr << e.what();
  }

  int it = std::min(size_, capacity_ - front_);
  for (int i = 0; i < it; ++i) {
    new_buffer[i] = buffer_[front_+i];
  }
  if (capacity_-front_ < size_) {
    for(int i = 0; i < (size_ - it); ++i) {
      new_buffer[it + i] = buffer_[i];
    }
  }
  std::memcpy(new_buffer, this->buffer_, sizeof(new_buffer));
  front_ = 0;
  back_ = size_;
  return new_buffer;
}

bool CircularBuffer::is_linearized() const {
  for (int i = front_; i != back_; i = (i + 1) % capacity_) {
    if (i > (i + 1) % capacity_) { return false;}
  }
  return true;
}

void CircularBuffer::rotate(int new_begin) {
  if (front_ == new_begin) {
    return;
  }
  value_type * new_buffer = new value_type[capacity_];
   for (int i = 0; i < size_; ++i) {
    int old_index = (front_ + i) % capacity_;
      int new_index = (new_begin + i) % capacity_;
      new_buffer[new_index] = buffer_[old_index];
    }
  for (int i = 0; i < size_; ++i) {
    buffer_[(new_begin + i) % capacity_] = new_buffer[(new_begin + i) % capacity_];
  }
  front_ = new_begin;
  back_ = (new_begin + size_) % capacity_;
  delete[] new_buffer;
}

int CircularBuffer::size() const {
  return size_;
}
bool CircularBuffer::empty() const {
  return size_ == 0;
}

bool CircularBuffer::full() const {
  return size_ == capacity_;
}

int CircularBuffer::reserve() const {
  return (capacity_ - size_);
}

int CircularBuffer::capacity() const {
  return capacity_;
}

void CircularBuffer::set_capacity(int new_capacity) {
  value_type * new_buffer;
  try {
    new_buffer = new value_type[new_capacity];
  }
  catch(const std::bad_alloc& e) {
    std::cerr << e.what();
  }
  for (int i = front_; ; i++) {
    if (i % capacity_ == back_) break;
    new_buffer[(i + new_capacity - capacity_) % new_capacity] = buffer_[i % capacity_];
  }
  front_ = (front_ + (new_capacity - capacity_)) % new_capacity;
  capacity_ = new_capacity;
  delete[] buffer_;
  buffer_ = new_buffer;
}

void CircularBuffer::resize(int new_size, const value_type & item) {
  if (new_size > capacity_) {
    this->set_capacity(new_size);
  }
  if (new_size > size_) {
    for (int i = 0; i < new_size - size_; ++i) {
      buffer_[back_] = item;
      back_ = (back_ + 1) % capacity_;
    }
  }
  else { 
    back_ = (back_ - (size_ - new_size) + capacity_) % capacity_;
  }
}

CircularBuffer & CircularBuffer::operator=(const CircularBuffer & cb) {
  delete[] this->buffer_;
  try {
    this->buffer_ = new value_type[cb.capacity_];
  }
  catch(const std::bad_alloc& e) {
    std::cerr << e.what();
  }
  this->capacity_ = cb.capacity_;
  this->size_ = cb.size_;
  this->front_ = cb.front_;
  this->back_ = cb.back_;
  std::memcpy(cb.buffer_, this->buffer_, sizeof(cb.buffer_));
  return *this;
}

void CircularBuffer::swap(CircularBuffer & cb) {
  for (int i = 0; i < this->capacity_; ++i) {
    value_type temp = this->buffer_[i];
    this->buffer_[i] = cb.buffer_[i];
    cb.buffer_[i] = temp;
  }
  int tmp = this->capacity_;
  this->capacity_ = cb.capacity_;
  cb.capacity_ = tmp;

  tmp = this->size_;
  this->size_ = cb.size_;
  cb.size_ = this->size_;

  tmp = this->front_;
  this->front_ - cb.front_;
  cb.front_ = tmp;

  tmp = this->back_;
  this->back_ = cb.back_;
  cb.back_ = tmp;
}

void CircularBuffer::push_back(const value_type & item) {
  buffer_[back_] = item;
  back_ = (back_ + 1 + capacity_) % capacity_;
  if (size_ == capacity_) 
    front_ = (front_ + 1 + capacity_) % capacity_;
  else
    size_++;
}

void CircularBuffer::push_front(const value_type & item) {
  front_ = (front_ - 1 + capacity_) % capacity_; 
  buffer_[front_] = item;
  if (size_ == capacity_) 
    back_ = (back_ - 1 + capacity_) % capacity_;
  else 
    size_++;
}

void CircularBuffer::pop_back() {
  if (!empty()) {
    size_--;
    back_ = (back_ - 1 + capacity_) % capacity_;
  }
  else { 
    throw std::underflow_error("Buffer is empty");
  }
}

void CircularBuffer::pop_front() {
  if (!empty()) {
    size_--;
    front_ = (front_ + 1 + capacity_) % capacity_;
  }
  else {
    throw std::underflow_error("Buffer is empty");
  }
}

void CircularBuffer::insert(int pos, const value_type & item) {
  if (pos > back_ and pos > front_ or pos > back_ and pos < front_\
  or pos > capacity_) {
    throw std::invalid_argument("Unable to insert an element to this position.");
    return;
  }
  if (!full()) {
    back_ = (back_ + 1) % capacity_;
    for (int i = back_; i != pos; i = (i-1+capacity_)%capacity_) {
      buffer_[i] = buffer_[(i-1+capacity_)%capacity_];
    }
    buffer_[pos] = item;
    size_++;
  }
  else {
    throw std::overflow_error("Buffer is full.");
  }
}

void CircularBuffer::erase(int first, int last) {
  if (first == last) return; 
  int num_to_remove;
  if (last > first)
    num_to_remove = last - first;
  else 
    num_to_remove = (capacity_ - first) + last;
  for (int i = last; i != back_; i = (i + 1) % capacity_) {
    buffer_[first] = buffer_[i];
    first = (first + 1) % capacity_;
  }
  back_ = (back_ - num_to_remove + capacity_) % capacity_;
  size_ -= num_to_remove;
}

void CircularBuffer::clear() {
  size_ = 0;
  front_ = 0;
  back_ = 0;
}

bool operator==(const CircularBuffer & a, const CircularBuffer & b) {
  if (a.capacity() == b.capacity() and a.size() == b.size()) {
    for (int i = 0; i < a.size(); ++i) {
      if (a[i] != b[i]) return false;
    }
    return true;
  }
  return false;
}

bool operator!=(const CircularBuffer & a, const CircularBuffer & b) {
  return !(a == b); 
}