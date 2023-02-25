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

    enum op_t
    {
        plus,
        minus,
        multiply,
        divide,
        assign
    };

    struct term_t
    {
        term_t() = default;
        virtual ~term_t() noexcept = default;
        virtual double operator()(state_t &s) const = 0;
    };

    /** Class representing a variable */
    struct var_t : public term_t
    {
        size_t id;
        /** only friends are allowed to construct variable instances */
        explicit var_t(size_t id) : id{id} {}

    public:
        var_t(const var_t &) = default;
        var_t &operator=(const var_t &) = default;

        /** returns the value of the variable stored in a state */
        double operator()(state_t &s) const override { return s[id]; }

        /** evaluates an assignment to a given expression and returns the resulting value */
        double operator()(state_t &, const expr_t &) const;

        friend class symbol_table_t;

        operator expr_t() const;

        var_t(expr_t expr);
    };

    class symbol_table_t
    {
        std::vector<std::string> names;
        std::vector<double> initial;

    public:
        [[nodiscard]] var_t var(std::string name, double init = 0)
        {
            auto res = names.size();

            names.push_back(std::move(name));
            initial.push_back(init);

            return var_t{res};
        }

        [[nodiscard]] state_t state() const { return {initial}; }
    };

    struct const_t : public term_t
    {
        double value;
        double operator()(state_t &) const override { return value; }
    };

    struct assign_t : public term_t
    {
        std::shared_ptr<var_t> var;
        std::shared_ptr<term_t> term;
        op_t op;

        assign_t(std::shared_ptr<var_t> var, std::shared_ptr<term_t> term, op_t op)
            : var{std::move(var)}, term{std::move(term)}, op{op} {}

        double operator()(state_t &s) const override {
            double value = (*term)(s);
            double *var_value = &s[var->id];

            switch (op) {
                case assign:
                    *var_value = value;
                    break;
                case plus:
                    *var_value += value;
                    break;
                case minus:
                    *var_value -= value;
                    break;
                case multiply:
                    *var_value *= value;
                    break;
                case divide:
                    *var_value /= value;
                    break;
                default:
                    throw std::runtime_error("invalid assignment operator");
            }

            return *var_value;
        }
    };

    struct unary_t : public term_t
    {
        std::shared_ptr<term_t> term;
        op_t op;

        unary_t(std::shared_ptr<term_t> term, op_t op)
            : term{std::move(term)}, op{op} {}

        double operator()(state_t &s) const override {
            switch (op) {
                case plus:
                    return +(*term)(s);
                case minus:
                    return -(*term)(s);
                default:
                    throw std::runtime_error("invalid unary operator");
            }
        }
    };

    struct binary_t : public term_t
    {
        std::shared_ptr<term_t> left;
        std::shared_ptr<term_t> right;
        op_t op;

        binary_t(std::shared_ptr<term_t> left, std::shared_ptr<term_t> right, op_t op)
            : left{std::move(left)}, right{std::move(right)}, op{op} {}

        double operator()(state_t &s) const override {
            switch (op) {
                case plus:
                    return (*left)(s) + (*right)(s);
                case minus:
                    return (*left)(s) - (*right)(s);
                case multiply:
                    return (*left)(s) * (*right)(s);
                case divide:
                    return (*left)(s) / (*right)(s);
                default:
                    throw std::runtime_error("invalid binary operator");
            }
        }
    };

    struct expr_t
    {
        std::shared_ptr<term_t> term;

        // Var constructor
        explicit expr_t(const var_t &var) : term{std::make_shared<var_t>(var)} {}

        // Binary constructor
        expr_t(const expr_t &e1, const expr_t &e2, op_t op)
            : term{std::make_shared<binary_t>(e1.term, e2.term, op)} {}

        // Unary constructor
        expr_t(const expr_t &e, op_t op)
            : term{std::make_shared<unary_t>(e.term, op)} {}

        // Const constructor
        expr_t(double value) : term{std::make_shared<const_t>(value)} {}

        // Assignment constructor
        expr_t(const var_t &var, const expr_t &e, op_t op)
            : term{std::make_shared<assign_t>(var, e.term, op)} {}

        double operator()(state_t &s) const { return (*term)(s); }
    };

    var_t::operator expr_t() const { return expr_t{*this}; }

    var_t::var_t(expr_t expr) {
        throw std::logic_error("assignment destination must be a variable expression");
    }

    /** assignment operation */
    inline double var_t::operator()(state_t &s, const expr_t &e) const { return s[id] = e(s); }

    /** unary operators: */
    inline expr_t operator+(const expr_t &e) { return expr_t{e, op_t::plus}; }
    inline expr_t operator-(const expr_t &e) { return expr_t{e, op_t::minus}; }

    /** binary operators: */
    inline expr_t operator+(const expr_t &e1, const expr_t &e2) { return expr_t{e1, e2, op_t::plus}; }
    inline expr_t operator-(const expr_t &e1, const expr_t &e2) { return expr_t{e1, e2, op_t::minus}; }
    inline expr_t operator<<=(const var_t &v, const expr_t &e) { return expr_t{v, e, op_t::assign}; }
    inline expr_t operator*(const expr_t &e1, const expr_t &e2) { return expr_t{e1, e2, op_t::multiply}; }
    inline expr_t operator/(const expr_t &e1, const expr_t &e2) { return expr_t{e1, e2, op_t::divide}; }
    inline expr_t operator+=(const var_t &v, const expr_t &e) { return expr_t{v, e, op_t::plus}; }
    inline expr_t operator-=(const var_t &v, const expr_t &e) { return expr_t{v, e, op_t::minus}; }
}

#endif // INCLUDE_ALGEBRA_HPP
