#ifndef INCLUDE_ALGEBRA_HPP
#define INCLUDE_ALGEBRA_HPP

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <stdexcept>

namespace calculator
{
    /** Type to capture the state of entire calculator (one number per variable): */
    using state_t = std::vector<double>;

    /** Forward declarations to get around circular dependencies: */
    struct expr_t;

    /** Class representing a variable */
    class var_t
    {
        size_t id;
        /** only friends are allowed to construct variable instances */
        explicit var_t(size_t id): id{id} {}
    public:
        var_t(const var_t&) = default;
        var_t& operator=(const var_t&) = default;
        /** returns the value of the variable stored in a state */
        double operator()(const state_t& s) const { return s[id]; }
        /** evaluates an assignment to a given expression and returns the resulting value */
        double operator()(state_t&, const expr_t&) const;
        friend class symbol_table_t;
    };

    class symbol_table_t
    {
        std::vector<std::string> names;
        std::vector<double> initial;
    public:
        [[nodiscard]] var_t var(std::string name, double init = 0) {
            auto res = names.size();
            names.push_back(std::move(name));
            initial.push_back(init);
            return var_t{res};
        }
        [[nodiscard]] state_t state() const { return {initial}; }
    };


    struct expr_t
    {
        std::unique_ptr<var_t> var;
        std::vector<std::unique_ptr<expr_t>> operands;
        enum op_t { plus, minus, add, sub, assign, multiply, divide } op;
        expr_t(const expr_t& other) { *this = other; }
        expr_t& operator=(const expr_t& other) {
            op = other.op;
            var = other.var ? std::make_unique<var_t>(*other.var) : nullptr;
            operands.resize(other.operands.size());
            std::transform(other.operands.begin(), other.operands.end(), operands.begin(),
                           [](const auto& ptr) { return std::make_unique<expr_t>(*ptr); } );
            return *this;
        }
        expr_t(expr_t&& other) noexcept = default;
        expr_t& operator=(expr_t&& other) noexcept = default;
        ~expr_t() noexcept = default;

        expr_t(const var_t& v, op_t op = plus): var{std::make_unique<var_t>(v)}, op{op} {}
        expr_t(const var_t& v, const expr_t& e): var{std::make_unique<var_t>(v)}, operands(1), op{assign} {
            operands[0] = std::make_unique<expr_t>(e);
        }
        expr_t(const expr_t& e, op_t op): operands(1), op{op} {
            operands[0] = std::make_unique<expr_t>(e);
        }
        expr_t(const expr_t& e1, const expr_t& e2, op_t op): operands(2), op{op} {
            operands[0] = std::make_unique<expr_t>(e1);
            operands[1] = std::make_unique<expr_t>(e2);
        }

        double operator()(state_t& s) const {
            if (var) {
                auto& v = *var;
                switch(op) {
                    case expr_t::plus: return v(s);
                    case expr_t::minus: return -v(s);
                    case expr_t::assign:
                        if (operands.empty())
                            throw std::logic_error{"missing expression to evaluate"};
                        return v(s, *operands[0]);
                    default:
                        throw std::logic_error{"unsupported operation over a variable"};
                }
            } else {
                auto& e1 = *operands[0];
                switch(op) {
                    case expr_t::plus:
                        if (operands.empty())
                            throw std::logic_error{"bug: missing an operand for unary +"};
                        return e1(s);
                    case expr_t::minus:
                        if (operands.empty())
                            throw std::logic_error{"bug: missing an operand for unary -"};
                        return -e1(s);
                    case expr_t::add: {
                        if (operands.size() != 2)
                            throw std::logic_error{"bug: expecting two operands for binary +"};
                        auto &e2 = *operands[1];
                        return e1(s) + e2(s);
                    }
                    case expr_t::sub: {
                        if (operands.size() != 2)
                            throw std::logic_error{"bug: expecting two operands for binary -"};
                        auto &e2 = *operands[1];
                        return e1(s) - e2(s);
                    }
                    case expr_t::multiply: {
                        if (operands.size() != 2)
                            throw std::logic_error{"bug: expecting two operands for binary *"};
                        auto &e2 = *operands[1];
                        return e1(s) * e2(s);
                    }
                    case expr_t::divide: {
                        if (operands.size() != 2)
                            throw std::logic_error{"bug: expecting two operands for binary /"};
                        auto &e2 = *operands[1];
                        if (e2(s) == 0)
                            throw std::logic_error{"division by zero"};
                        return e1(s) / e2(s);
                    }
                    default:
                        throw std::logic_error{"not implemented"};
                }
            }
        }
    };

    /** assignment operation */
    inline double var_t::operator()(state_t& s, const expr_t& e) const { return s[id] = e(s); }

    /** unary operators: */
    inline expr_t operator+(const expr_t& e) { return expr_t{e, expr_t::plus}; }
    inline expr_t operator-(const expr_t& e) { return expr_t{e, expr_t::minus}; }

    /** binary operators: */
    inline expr_t operator+(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, expr_t::add}; }
    inline expr_t operator-(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, expr_t::sub}; }
    inline expr_t operator<<=(const var_t& v, const expr_t& e) { return expr_t{v, e}; }
    inline expr_t operator*(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, expr_t::multiply}; }
    inline expr_t operator/(const expr_t& e1, const expr_t& e2) { return expr_t{e1, e2, expr_t::divide}; }
}

#endif // INCLUDE_ALGEBRA_HPP

