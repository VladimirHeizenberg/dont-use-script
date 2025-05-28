#pragma once

#include <any>
#include <iostream>
#include <vector>

class Value {
public:
    using Array = std::vector<Value>;

    Value() = default;
    Value(double v) : value_(v) {}
    Value(bool v) : value_(v) {}
    Value(const std::string& v) : value_(v) {}
    Value(const char* v) : value_(std::string(v)) {}
    Value(const Array& v) : value_(v) {}

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

    operator bool() const {
        if (Is<bool>()) return As<bool>();
        if (Is<double>()) return As<double>() != 0;
        if (Is<Array>()) return As<Array>().empty();
        if (Is<std::string>()) return As<std::string>().empty();
        throw std::runtime_error("illegal cast");
    }

    friend std::ostream& operator<<(std::ostream& os, const Value& v) {
        if (v.Is<double>())      os << v.As<double>();
        else if (v.Is<bool>())   os << (v.As<bool>() ? "true" : "false");
        else if (v.Is<std::string>()) os << "\"" << v.As<std::string>() << "\"";
        else if (v.Is<Array>()) {
            os << "[";
            for (const auto& c : v.As<Array>()) {
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
        if (Is<Array>() && rhs.Is<Array>()) {
            auto result = As<Array>();
            result.insert(
                result.end(), 
                rhs.As<Array>().begin(),
                rhs.As<Array>().end()
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
        if (Is<Array>() && rhs.Is<double>()) {
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

    bool operator<(const Value& rhs) const {
        if (Is<double>() || Is<bool>()) {
            return As<double>() < rhs.As<double>();
        }
        if (Is<std::string>() && rhs.Is<std::string>()) {
            return As<std::string>() < rhs.As<std::string>();
        }
        throw std::runtime_error("Invalid operands for operator<");
    }

    bool operator>(const Value& rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Value& rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Value& rhs) const {
        return !(*this < rhs);
    }

    bool operator==(const Value& rhs) const {
        if (Is<double>() || Is<bool>()) {
            return As<double>() == rhs.As<double>();
        }
        if (Is<std::string>() && rhs.Is<std::string>()) {
            return As<std::string>() == rhs.As<std::string>();
        }
        if (Is<Array>() && rhs.Is<Array>()) {
            return As<Array>() == rhs.As<Array>();
        }
        if (Is<bool>() && rhs.Is<bool>()) {
            return As<bool>() == rhs.As<bool>();
        }
        throw std::runtime_error("incomparable types\n");
    }

    bool operator!=(const Value& rhs) const {
        return !(*this == rhs);
    }

    Value operator&(const Value& rhs) const {
        return static_cast<bool>(*this) && static_cast<bool>(rhs);
    }

    Value operator|(const Value& rhs) const {
        return static_cast<bool>(*this) || static_cast<bool>(rhs);
    }

    Value operator!() const {
        return !static_cast<bool>(*this);
    }

private:
    std::any value_;
};