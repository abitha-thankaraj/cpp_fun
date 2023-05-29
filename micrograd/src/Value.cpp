#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_set>


class Value {
    
    double _data; // value of the node
    double _grad; // gradient of output w.r.t. this node
    std::vector<Value*> _prev; // previous nodes in graph
    std::string _op; // operation that produced this node; For debugging
    std::function<void()> _backward_function; // function to backprop to input nodes

    public:

        Value(double data, const std::vector<Value*>& children = {}, std::string op = "")
            : _data(data), _grad(0.0), _prev(children), _op(op) {}

        Value operator+(Value& other) {
            Value out(_data + other._data, std::vector<Value*>{this, &other}, "+");
            out._backward_function = [this, &out, &other]() {
                _grad += out._grad;
                other._grad += out._grad;
            };
            return out;
        }

        Value operator*(Value& other) {
            Value out(_data * other._data, std::vector<Value*>{this, &other}, "*");
            out._backward_function = [this, &out, &other]() {
                _grad += other._data * out._grad;
                other._grad += _data * out._grad;
            };
            return out;
        }

        Value pow(double n) {
            Value out(std::pow(_data, n), std::vector<Value*>{this}, "pow");
            out._backward_function = [this, &out, n]() {
                _grad += n * std::pow(_data, n - 1) * out._grad;
            };
            return out;
        }

        Value relu() {
            Value out(_data < 0 ? 0 : _data, std::vector<Value*>{this}, "ReLU");
            out._backward_function = [this, &out]() {
                _grad += (out._data > 0) * out._grad;
            };
            return out;
        }

        // Equivalent of Python's __repr__ method
        friend std::ostream& operator<<(std::ostream& os, const Value& value) {
            os << "Value: data=" << value._data << ", grad=" << value._grad << ", op="<<value._op<< std::endl;

            // TODO fix recursive; This one is only upto the first child.
            // TODO: Can we visualize this with graphviz?
            if (!value._prev.empty()) {
                os << "Children:" << std::endl;
                for (const Value* child : value._prev) {
                    if (child != nullptr) {
                        os << "  " << *child;  // Recursive call to operator<<
                    }
                }
            }

            return os;
        }

};

int main() {
    
    Value x(3.0), y(2.0);
    Value k = x * y;
    std::cout << k << std::endl;
    
    Value z = x + y;
    std::cout << z << std::endl;

    Value m = x.pow(2);
    std::cout << m << std::endl;

    Value n = x.relu();
    std::cout << n << std::endl;
   
    return 0;
}
