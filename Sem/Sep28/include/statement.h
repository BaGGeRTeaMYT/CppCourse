#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>

class Statement {  
public:  
    virtual std::vector<int> apply(std::vector<int> in) const = 0;  

    Statement() = default;  
    Statement(unsigned arguments, unsigned results, bool pure): arguments(arguments), results(results), pure(pure) {}  
 
    virtual ~Statement() = default;
 
    bool is_pure() const {  
        return pure;  
    }  
 
    unsigned get_arguments_count() const {  
        return arguments;  
    }  
 
    unsigned get_results_count() const {  
        return results;  
    }  
 
protected:  
    unsigned arguments;  
    unsigned results;  
    bool pure;  
};

class Combine : public Statement {
    public:
    Combine() = default;
    Combine(const Combine &c) = default;
    // l is executed first
    Combine(std::shared_ptr<Statement> l, std::shared_ptr<Statement> r): Statement(), l(l), r(r) {
        arguments = l->get_arguments_count() + r->get_arguments_count() - std::min(l->get_results_count(), r->get_arguments_count());
        results = l->get_results_count() + r->get_results_count() - std::min(l->get_results_count(), r->get_arguments_count());
        pure = l->is_pure() & r->is_pure();
    }
    std::vector<int> apply(std::vector<int> in) const override {
        return r->apply(l->apply(in));
    }
    private:
    std::shared_ptr<Statement> l;
    std::shared_ptr<Statement> r;
};

class ConstOp : public Statement {

    public:
    ConstOp(int v): Statement(0, 1, true), v(v) {}

    std::vector<int> apply(std::vector<int> in) const override {
        in.push_back(v);
        return in;
    }

    private:
    int v;

};

class BinOpPlus: public Statement {
    public:
    BinOpPlus(): Statement(2, 1, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int b = in.back();
        in.pop_back();
        int a = in.back();
        in.pop_back();
        in.push_back(a + b);
        return in;
    }
};

class BinOpMinus: public Statement {
    public:
    BinOpMinus(): Statement(2, 1, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int b = in.back();
        in.pop_back();
        int a = in.back();
        in.pop_back();
        in.push_back(a - b);
        return in;
    }
};

class BinOpMult: public Statement {
    public:
    BinOpMult(): Statement(2, 1, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int b = in.back();
        in.pop_back();
        int a = in.back();
        in.pop_back();
        in.push_back(a * b);
        return in;
    }
};

class BinOpDiv: public Statement {
    public:
    BinOpDiv(): Statement(2, 1, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int b = in.back();
        in.pop_back();
        int a = in.back();
        in.pop_back();
        in.push_back(a / b);
        return in;
    }
};

class BinOpMod: public Statement {
    public:
    BinOpMod(): Statement(2, 1, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int b = in.back();
        in.pop_back();
        int a = in.back();
        in.pop_back();
        in.push_back(a % b);
        return in;
    }
};

class UnOpAbs: public Statement {
    public:
    UnOpAbs(): Statement(1, 1, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int a = in.back();
        in.pop_back();
        return in;
    }
};

class Input: public Statement {
    public:
    Input(): Statement(0, 1, false) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int a;
        std::cin >> a;
        in.push_back(a);
        return in;
    }
};

class Dup: public Statement {
    public:
    Dup(): Statement(1, 2, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int a = in.back();
        in.pop_back();
        in.push_back(a);
        in.push_back(a);
        return in;
    }
}; 

class Nop: public Statement {
    public:
    Nop(): Statement(0, 0, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        return in;
    }
};

std::shared_ptr<Statement> compile(std::string_view str) {
    std::stringstream stream(static_cast<std::string>(str));
    std::string word;
    std::vector<int> in;
    std::shared_ptr<Statement> prevOp = std::make_shared<Nop>(Nop());
    std::shared_ptr<Statement> curOp;
    while (stream >> word) {
        if (isdigit(word[0])) {
            ConstOp operation(std::stoi(word));
            curOp = std::make_shared<ConstOp>(operation);
        } else if (word[0] == '+') {
            if (word.size() > 1) {
                word.erase(word.begin());
                int num = std::stoi(word);
                ConstOp operation(num);
                curOp = std::make_shared<ConstOp>(operation);
            } else {
                BinOpPlus operation;
                curOp = std::make_shared<BinOpPlus>(operation);
            }
        } else if (word[0] == '-') {
            if (word.size() > 1) {
                word.erase(word.begin());
                int num = -std::stoi(word);
                ConstOp operation(num);
                curOp = std::make_shared<ConstOp>(operation);
            } else {
                BinOpMinus operation;
                curOp = std::make_shared<BinOpMinus>(operation);
            }
        } else if (word[0] == '*') {
            BinOpMult operation;
            curOp = std::make_shared<BinOpMult>(operation);
        } else if (word[0] == '/') {
            BinOpDiv operation;
            curOp = std::make_shared<BinOpDiv>(operation);
        } else if (word[0] == '%') {
            BinOpMod operation;
            curOp = std::make_shared<BinOpMod>(operation);
        } else if (word[0] == 'a') {
            UnOpAbs operation;
            curOp = std::make_shared<UnOpAbs>(operation);
        } else if (word[0] == 'i') {
            Input operation;
            curOp = std::make_shared<Input>(operation);
        } else if (word[0] == 'd') {
            Dup operation;
            curOp = std::make_shared<Dup>(operation);
        } else {
            Nop operation;
            curOp = std::make_shared<Nop>(operation);
        }
        prevOp = std::make_shared<Combine>(Combine(prevOp, curOp));
    }
    return prevOp;
}


std::shared_ptr<Statement> operator|(std::shared_ptr<Statement> lhs, std::shared_ptr<Statement> rhs) {
    return std::make_shared<Combine>(Combine(lhs, rhs));
}