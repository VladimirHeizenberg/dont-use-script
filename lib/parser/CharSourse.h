#pragma once

#include <istream>

class CharSource {
public:
    virtual ~CharSource() = default;
    virtual char peek() = 0;
    virtual char get() = 0;
    virtual bool eof() const = 0;
};

class StreamCharSource: public CharSource {
public:
    StreamCharSource(std::istream& stream)
    : stream_(stream) {}

    char peek() {
        return stream_.peek();
    }

    char get() {
        return stream_.get();
    }

    bool eof() const {
        return stream_.eof();
    }
private:
    std::istream& stream_;
};