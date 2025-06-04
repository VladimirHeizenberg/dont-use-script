#pragma once

#include <istream>

class CharSource {
public:
    virtual ~CharSource() = default;
    virtual char peek() = 0;
    virtual char get() = 0;
    [[nodiscard]] virtual bool eof() const = 0;
};

class StreamCharSource: public CharSource {
public:
    StreamCharSource(std::istream& stream)
    : stream_(stream) {}

    char peek() override {
        return stream_.peek();
    }

    [[nodiscard]] char get() override {
        return stream_.get();
    }

    [[nodiscard]] bool eof() const override {
        return stream_.eof();
    }
private:
    std::istream& stream_;
};