#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <iterator>
#include <stdexcept>

template<std::size_t Idx = 0, typename... Types>
void print_tuple(std::ostream& os, const std::tuple<Types...>& t) {
    if constexpr (Idx < sizeof...(Types)) {
        if constexpr (Idx > 0) {
            os << ", ";
        }
        os << std::get<Idx>(t);
        print_tuple<Idx + 1>(os, t);
    }
}

template<typename... Types>
std::ostream& operator<<(std::ostream& os, const std::tuple<Types...>& t) {
    print_tuple(os, t);
    return os;
}

template<typename... Args>
class ParserIterator {
public:
    using ValueType = std::tuple<Args...>;
    using PointerType = ValueType*;
    using ReferenceType = ValueType&;

    ParserIterator() : ended(true) {}
    ParserIterator(std::istream& is, char delim = ',', char escape_char = '"') : input_stream(&is), ended(false),\
        delimeter(delim), escape_char(escape_char), current_line_number(1) {
        parseLine();
    }
    ~ParserIterator() = default;
    
private:
    bool CheckNextItem(std::istringstream& istream, std::string& item) {
        char c;
        item.clear();
        while (istream.get(c)) {
            if (c == escape_char) {
                if (istream.get(c))
                    item += c;
            }
            else if (c == delimeter)
                break;
            else
                item += c;
        }
        return !(item.empty() && istream.eof());
    }

    template<size_t Idx, typename Tuple>
    void parseTupleItem(std::istringstream& istream, Tuple& tuple) {
        if constexpr (Idx < std::tuple_size_v<Tuple>) {
            std::string item;
            if (!CheckNextItem(istream, item)) {
                throw std::runtime_error("Error in line " + std::to_string(current_line_number+1) + " in column "\
                    + std::to_string(Idx+1) + "\n");
            }
            std::istringstream items_stream(item);
            items_stream >> std::get<Idx>(tuple);
            parseTupleItem<Idx+1>(istream, tuple);
        }
    }

    void parseLine() {
        if (std::getline(*input_stream, current_line)) {
            current_line_number++;
            std::istringstream line(current_line);
            parseTupleItem<0>(line, current_tuple);
        }
        else {
            ended = true;
        }
    }
public:
    ValueType& operator* () { return current_tuple; }

    ParserIterator& operator++() {
        parseLine();
        return *this;
    }

    bool operator==(ParserIterator& other) const { return this->ended == other.ended; }
    bool operator!=(ParserIterator& other) const { return !(*this==other); }

private:
    std::istream* input_stream = nullptr;
    std::string current_line;
    size_t current_line_number;
    ValueType current_tuple;
    bool ended = false;
    char delimeter;
    char escape_char;
};

template <typename... Args>
class CSVParser {
public:
    CSVParser() = default;
    CSVParser(std::istream& is, size_t skip = 0, char delim = ',', char escape_char='"') : input_stream(is),\
        skip_lines(skip), delimeter(delim), escape_char(escape_char) {
        for (int i = 0; i < skip_lines; i++) {
            std::string skip;
            std::getline(input_stream, skip);
        }
    }

    ParserIterator<Args...> begin() {
        return ParserIterator<Args...>(input_stream, delimeter, escape_char);
    }

    ParserIterator<Args...> end() {
        return ParserIterator<Args...>();
    }

    const ParserIterator<Args...> begin() const {
        return ParserIterator<Args...>(input_stream, delimeter, escape_char);
    }

    const ParserIterator<Args...> end() const {
        return ParserIterator<Args...>();
    }

private:
    std::istream& input_stream;
    int skip_lines;
    char delimeter;
    char escape_char;
};