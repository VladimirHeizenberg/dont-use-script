#include "MakeValue.h"


namespace dont_use_script::value {
    
ValuePtr MakeString(const std::string& value) {
    return std::make_shared<String>(value);
}

ValuePtr MakeDouble(double num) {
    return std::make_shared<Double>(num);
}

ValuePtr MakeBool(bool value) {
    return std::make_shared<Bool>(value);
}

ValuePtr MakeFunction(std::unique_ptr<ast::Statement> function_body,
                           std::vector<std::string> arguments) {
    return std::make_shared<Function>(std::move(function_body), std::move(arguments));
}

ValuePtr MakeSystemFunction(std::function<ValuePtr(const std::vector<ValuePtr>&)> function) {
    return std::make_shared<SystemFunction>(std::move(function));
}

ValuePtr MakeNull() {
    return std::make_shared<Null>();
}

ValuePtr MakeArray() {
    return std::make_shared<Array>();
}

ValuePtr MakeArray(std::vector<ValuePtr> values) {
    return std::make_shared<Array>(std::move(values));
}

} // namespace dont_use_script::value