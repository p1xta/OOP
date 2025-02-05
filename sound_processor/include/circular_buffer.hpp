typedef char value_type;

class CircularBuffer {
  value_type * buffer_;
  int front_;
  int back_;
  int capacity_;
  int size_;

public:
    explicit CircularBuffer(int capacity);
};