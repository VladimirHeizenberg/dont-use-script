#pragma once

#include <any>
#include <iostream>
#include <vector>

class Value {
public:
    Value() = default;
    Value(double v) : value_(v) {}
    Value(bool v) : value_(v) {}
    Value(const std::string& v) : value_(v) {}
    Value(const char* v) : value_(std::string(v)) {}
    Value(const std::vector<Value>& v) : value_(v) {}

    template<typename T>
    bool Is() const {
        return value_.type() == typeid(T);
    }

    template <typename T>
    const T& As() const {
        if (!Is<T>()) {
            throw std::runtime_error("Wrong types\n");
        }
        return std::any_cast<const T&>(value_);
    }

    template <typename T>
    Value& operator=(const T& other) {
        value_ = other;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Value& v) {
        if (v.Is<double>())      os << v.As<double>();
        else if (v.Is<bool>())   os << (v.As<bool>() ? "true" : "false");
        else if (v.Is<std::string>()) os << "\"" << v.As<std::string>() << "\"";
        else if (v.Is<std::vector<Value>>()) {
            os << "[";
            for (const auto& c : v.As<std::vector<Value>>()) {
                os << c << ", ";
            }
            os << "]";
        }
        else os << "<unknown>";
        return os;
    }

    Value operator+(const Value& rhs) const {
        if (Is<double>() && rhs.Is<double>()) {
            return As<double>() + rhs.As<double>();
        }
        if (Is<double>() && rhs.Is<bool>()) {
            return As<double>() + (rhs.As<bool>() ? 1 : 0);
        }
        if (Is<std::string>() && rhs.Is<std::string>()) {
            return As<std::string>() + rhs.As<std::string>();
        }
        if (Is<std::vector<Value>>() && rhs.Is<std::vector<Value>>()) {
            auto result = As<std::vector<Value>>();
            result.insert(
                result.end(), 
                rhs.As<std::vector<Value>>().begin(),
                rhs.As<std::vector<Value>>().end()
            );
            return result;
        }
        throw std::runtime_error("TypeError");
    }

    Value operator-(const Value& rhs) const {
        if (Is<double>() && rhs.Is<double>()) {
            return As<double>() - rhs.As<double>();
        }
        if (Is<double>() && rhs.Is<bool>()) {
            return As<double>() - (rhs.As<bool>() ? 1 : 0);
        }
        // if (Is<std::string>() && rhs.Is<std::string>()) {
        //     return As<std::string>() + rhs.As<std::string>();
        // }
        throw std::runtime_error("TypeError");
    }

    Value operator*(const Value& rhs) const {
        if (Is<double>() && rhs.Is<double>()) {
            return As<double>() * rhs.As<double>();
        }
        if (Is<double>() && rhs.Is<bool>()) {
            return As<double>() * (rhs.As<bool>() ? 1 : 0);
        }
        if (Is<std::string>() && rhs.Is<double>()) {
            Value result = *this;
            double repeat = rhs.As<double>();
            int x = 0;
            // TODO: add += so not too much memory to store new result every time
            while (x < repeat) {
                result = result + result;
                ++x;
            }
            return result;
        }
        if (Is<std::vector<Value>>() && rhs.Is<double>()) {
            Value result = *this;
            double repeat = rhs.As<double>();
            int x = 0;
            // TODO: add += so not too much memory to store new result every time
            while (x < repeat) {
                result = result + result;
                ++x;
            }
            return result;
        }
        throw std::runtime_error("TypeError");
    }

    Value operator/(const Value& rhs) const {
        if (Is<double>() && rhs.Is<double>()) {
            return As<double>() / rhs.As<double>();
        }
        if (Is<double>() && rhs.Is<bool>()) {
            return As<double>() / (rhs.As<bool>() ? 1 : 0);
        }
        throw std::runtime_error("TypeError");
    }
private:
    std::any value_;
};