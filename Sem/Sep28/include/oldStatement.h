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
        return r->apply(std::move(l->apply(std::move(in))));
    }

    friend std::vector<std::shared_ptr<Statement>> unwrap(std::shared_ptr<Statement> stmt);

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

    friend void printOps(std::vector<std::shared_ptr<Statement>> operations);    

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
        int a = abs(in.back());
        in.pop_back();
        in.push_back(a);
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
    if (std::dynamic_pointer_cast<Nop>(lhs)) {
        return rhs;
    }
    if (std::dynamic_pointer_cast<Nop>(rhs)) {
        return lhs;
    }
    return std::make_shared<Combine>(Combine(lhs, rhs));
}

std::vector<std::shared_ptr<Statement>> unwrap(std::shared_ptr<Statement> stmt) {
    std::shared_ptr<Combine> comb = std::dynamic_pointer_cast<Combine>(stmt);
    bool notComb = false;
    if (comb == nullptr) {
        notComb = true;
    }
    std::vector<std::shared_ptr<Statement>> left;
    std::vector<std::shared_ptr<Statement>> right;
    if (notComb) {
        if (std::dynamic_pointer_cast<Nop>(stmt) == nullptr) {
            left.push_back(stmt);
        }
    } else {
        left = unwrap(comb->l);
        right = unwrap(comb->r);
    }
    for (auto& i : right) {
        left.push_back(i);
    }
    return left;
}


void printOps(std::vector<std::shared_ptr<Statement>> operations) {
    for (auto i : operations) {
        if (std::shared_ptr<ConstOp> tmp = std::dynamic_pointer_cast<ConstOp>(i)) {
            std::cout << tmp->v << ' ';
        } else if (std::shared_ptr<BinOpDiv> tmp = std::dynamic_pointer_cast<BinOpDiv>(i)) {
            std::cout << "/ ";
        } else if (std::shared_ptr<BinOpMinus> tmp = std::dynamic_pointer_cast<BinOpMinus>(i)) {
            std::cout << "- ";
        } else if (std::shared_ptr<BinOpMod> tmp = std::dynamic_pointer_cast<BinOpMod>(i)) {
            std::cout << "% ";
        } else if (std::shared_ptr<BinOpMult> tmp = std::dynamic_pointer_cast<BinOpMult>(i)) {
            std::cout << "* ";
        } else if (std::shared_ptr<BinOpPlus> tmp = std::dynamic_pointer_cast<BinOpPlus>(i)) {
            std::cout << "+ ";
        } else if (std::shared_ptr<Dup> tmp = std::dynamic_pointer_cast<Dup>(i)) {
            std::cout << "dup ";
        } else if (std::shared_ptr<UnOpAbs> tmp = std::dynamic_pointer_cast<UnOpAbs>(i)) {
            std::cout << "abs ";
        } else if (std::shared_ptr<Input> tmp = std::dynamic_pointer_cast<Input>(i)) {
            std::cout << "input ";
        }
    }
}

void printOp(std::shared_ptr<Statement> operation) {
    std::vector<std::shared_ptr<Statement>> ops = {operation};
    printOps(ops);
}

std::shared_ptr<Statement> optimize(std::shared_ptr<Statement> stmt) {
    std::vector<std::shared_ptr<Statement>> operations = unwrap(stmt);
    std::shared_ptr<Statement> answer = compile("");
    int const_ops = 0;
    for (int i = 0; i < operations.size(); i++) {
        if (std::dynamic_pointer_cast<ConstOp>(operations[i])) {
            const_ops++;
            #ifdef DEBUG_PRINT
            std::cout << i << ": is constOp ";
            printOp(operations[i]);
            std::cout << std::endl;
            #endif
        } else if (!(operations[i]->is_pure())) {
            const_ops = 0;
            #ifdef DEBUG_PRINT
            std::cout << i << ": is not pure" << std::endl;
            #endif 
        } else { // optimization is possible if some conditions met
            int required_args = operations[i]->get_arguments_count();
            int given_results = operations[i]->get_results_count();
            #ifdef DEBUG_PRINT
            std::cout << i << ": is operation with params (" << required_args << ' ' << given_results << ")" << std::endl;
            #endif
            if (required_args <= const_ops) {
                #ifdef DEBUG_PRINT
                std::cout << "Optimizing." << std::endl;
                #endif
                const_ops -= required_args;
                std::vector<int> stack;
                for (int j = i - required_args; j < i; j++) {
                    stack = operations[j]->apply(stack);
                }
                #ifdef DEBUG_PRINT
                std::cout << "Operating with: ";
                for (int j : stack) {
                    std::cout << j << ' ';
                }
                std::cout << std::endl;
                #endif
                stack = operations[i]->apply(stack);
                operations.erase(operations.begin() + i - required_args, operations.begin() + i + 1);
                #ifdef DEBUG_PRINT
                std::cout << "Result: ";
                #endif
                for (auto j : stack) {
                    operations.insert(operations.begin() + i - required_args, std::make_shared<ConstOp>(j));
                    #ifdef DEBUG_PRINT
                    std::cout << j << ' ';
                    #endif
                }
                #ifdef DEBUG_PRINT
                std::cout << std::endl;
                #endif
                i -= required_args + 1;
                i += given_results;
                const_ops += given_results;
                #ifdef DEBUG_PRINT
                std::cout << "Now operations look like this:" << std::endl;
                printOps(operations);
                std::cout << std::endl;
                #endif
            } else {
                #ifdef DEBUG_PRINT
                std::cout << "Unable to optimize ";
                printOp(operations[i]);
                std::cout << ", not enough constants" << std::endl;
                #endif
                const_ops = 0;
            }
            #ifdef DEBUG_PRINT
            std::cout << std::endl;
            #endif
        }
    }
    for (auto op : operations) {
        answer = answer | op;
    }
    return answer;
}