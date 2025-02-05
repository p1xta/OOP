#include <iostream>
#include <exception>

class FileException : public std::exception {
private:
    std::string message;
public:
    explicit FileException (std::string msg) : message(msg) {}
    std::string what() noexcept {
        return message;
    }
    virtual ~FileException() noexcept = default;
};

class FileOpenException : public FileException {
public:
    explicit FileOpenException(std::string msg) : FileException(msg) {}
};

class FileReadException : public FileException {
public:
    explicit FileReadException(std::string msg) : FileException(msg) {}
};


class ConverterException : public std::exception {
private:
    std::string message;
public:
    explicit ConverterException(std::string msg) : message(msg) {}
    std::string what() noexcept {
        return message;
    }
    virtual ~ConverterException() noexcept = default;
};

class MuteConverterException : public ConverterException {
public:
    explicit MuteConverterException(std::string msg) : ConverterException(msg) {}
};

class MixConverterException : public ConverterException {
public:
    explicit MixConverterException(std::string msg) : ConverterException(msg) {}
};

class PitchShifterException : public ConverterException {
public:
    explicit PitchShifterException(std::string msg) : ConverterException(msg) {}
};


class ConfigException : public std::exception {
private:
    std::string message;
public:
    explicit ConfigException(std::string msg) : message(msg) {}
    std::string what() noexcept {
        return message;
    }
    virtual ~ConfigException() noexcept = default;
};