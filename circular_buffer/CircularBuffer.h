typedef char value_type;

class CircularBuffer {
  value_type * buffer_;
  int front_;
  int back_;
  int capacity_;
  int size_;

public:
  CircularBuffer();
  ~CircularBuffer();
  //Copies the contents of buffer cb to current buffer.
  CircularBuffer(const CircularBuffer & cb);
  //initializer with initializer_list
  CircularBuffer(std::initializer_list<value_type> init_list);

  //Constructs a buffer with given capacity.
  explicit CircularBuffer(int capacity);
  //  //Constructs a buffer with given capacity, fills it up with element elem.
  CircularBuffer(int capacity, const value_type & elem);

  //Access by index i. Does not check if endex is valid.
  value_type & operator[](int i);
  const value_type & operator[](int i) const;
  
  //Access by index i. Throws exception if index is invalid.
  value_type & at(int i);
  const value_type & at(int i) const;
  //Returns a pointer to a first element.
  value_type & front();
  //Returns a pointer to a last element.
  value_type & back();
  //Returns an index of a first element.
  int front_index();
  //Returns an index of a last element.
  int back_index();

  const value_type & front() const;
  const value_type & back() const;

  //Rotates the buffer so its first element moves to the beginning 
  //of allocated memory. Returns a pointer to the first element.
  value_type * linearize();
  //Checks whether the buffer is linearized.
  bool is_linearized() const;
  //Rotates the buffer so the first element is the one
  //with an index new_begin.
  void rotate(int new_begin);
  //Amout of elements currently stored in the buffer.
  int size() const;
  bool empty() const;
  //Returns true if size of the buffer equals to its capacity.
  bool full() const;
  //Amount of empty cells in the buffer.
  int reserve() const;
  //returns capacity of the buffer.
  int capacity() const;

  void set_capacity(int new_capacity);
  //Changes the size of the buffer.
  //If the buffer expands, new elements are filled with element elem.
  void resize(int new_size, const value_type & item = value_type());
  //Assignment operator
  CircularBuffer & operator=(const CircularBuffer & cb);
  //Exchanges contents of the buffer with buffer cb
  void swap(CircularBuffer & cb);

  //Adds an element to the end of the buffer.
  //If the buffer is full, the first element 
  //is overwritten.
  void push_back(const value_type & item = value_type());
  //Adds an element before the first element of the buffer.
  //If the buffer is full, the last element
  //is overwritten.
  void push_front(const value_type & item = value_type());
  //Deletes the last element of the buffer.
  void pop_back();
  //Deletes the first element of the buffer.
  void pop_front();

  //Inserts element item by index pos.
  //Capacity of the buffer remains the same.
  void insert(int pos, const value_type & item = value_type());
  //Deletes elements of the buffer in the interval [first, last).
  void erase(int first, int last);
  //Clears the buffer.
  void clear();
};
//Equality operator.
bool operator==(const CircularBuffer & a, const CircularBuffer & b);
//Ineaquality operator.
bool operator!=(const CircularBuffer & a, const CircularBuffer & b);

