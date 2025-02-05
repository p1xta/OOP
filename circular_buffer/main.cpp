#include "CircularBuffer.h"
#include <iostream>

int main() {

    CircularBuffer buffer_10(10);
    CircularBuffer empty_buffer(1);
    CircularBuffer full_a_buffer(15, 'a');
    CircularBuffer bad_buffer(-3);
    std::cout << std::endl;
    
    buffer_10.push_back('a');
    buffer_10.push_back('b');
    buffer_10.push_back('c');
    buffer_10.push_back('d');
    buffer_10.push_back('e');
    buffer_10.push_front('f');
    buffer_10.push_front('g');
    buffer_10.push_front('h');
    buffer_10.set_capacity(15);
    std::cout << buffer_10.front() << "- front  " << buffer_10.back() << "- back  " << std::endl;
    for (int i = buffer_10.front_index(); i != buffer_10.back_index(); i = (i + 1) % buffer_10.capacity()) {
        std::cout << i << buffer_10.at(i) << " ";
    }
    std::cout << std::endl;
    buffer_10.resize(10, 'x');
    std::cout << buffer_10.front() << "- front  " << buffer_10.back() << "- back  " << std::endl;
    for (int i = buffer_10.front_index(); i != buffer_10.back_index(); i = (i + 1) % buffer_10.capacity()) {
        std::cout << i << buffer_10.at(i) << " ";
    }
     std::cout << std::endl;
    buffer_10.erase(13, 2); // from b to x
    //buffer_10.pop_front();
    //buffer_10.resize(10, 'x');
    std::cout << buffer_10.front() << "- front  " << buffer_10.back() << "- back  " << std::endl;
    for (int i = buffer_10.front_index(); i != buffer_10.back_index(); i = (i + 1) % buffer_10.capacity()) {
        std::cout << i << buffer_10.at(i) << " ";
    }
    /* std::cout << "front i - " << buffer_10.front_index() << std::endl;
    std::cout << "back i - " << buffer_10.back_index() << std::endl;
    buffer_10.pop_back();
    buffer_10.pop_back();
    buffer_10.pop_front();
    buffer_10.pop_front();
    
    std::cout << std::endl;
    for (int i = buffer_10.front_index(); i != buffer_10.back_index(); i = (i + 1) % buffer_10.capacity()) {
        std::cout << buffer_10.at(i) << " ";
    }
    std::cout << std::endl;
    std::cout << "front i - " << buffer_10.front_index() << std::endl;
    std::cout << "back i - " << buffer_10.back_index() << std::endl;
    std::cout << "front - " << buffer_10.front() << std::endl;
    std::cout << "back - " << buffer_10.back() << std::endl;
 */
/*     for (int i = buffer_10.front_index(); i != buffer_10.back_index(); i = (i + 1) % buffer_10.capacity()) {
        std::cout << i << buffer_10.at(i) << " ";
    }
    std::cout << std::endl;
    std::cout << buffer_10.front() << "- front  " << buffer_10.back() << "- back  " << std::endl;
    std::cout << std::endl;
    buffer_10.set_capacity(15);
    buffer_10.resize(10, 'x');
    std::cout << buffer_10.front_index() << "- front  " << buffer_10.back_index() << "- back  " << std::endl;
    std::cout << std::endl;
    for (int i = buffer_10.front_index(); i != buffer_10.back_index()-1; i = (i + 1) % buffer_10.capacity()) {
        std::cout << i << buffer_10.at(i) << " ";
    }
    std::cout << std::endl;
    std::cout << buffer_10.front() << "- front  " << buffer_10.back() << "- back  " << std::endl;
     */
    return 0;
} 
