#include <vector>
#include <list>
#include <string>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace calculator
{
    /** Forward declarations: */
    struct term_t;
    class const_t;
    class var_t;
    class unary_t;
    class binary_t;
    class assign_t;

    struct Visitor
    {
        virtual ~Visitor() = default;
        virtual void visit(const term_t &) = 0;
        virtual void visit(const const_t &) = 0;
        virtual void visit(const var_t &) = 0;
        virtual void visit(const unary_t &) = 0;
        virtual void visit(const binary_t &) = 0;
        virtual void visit(const assign_t &) = 0;
    };
    using state_t = std::vector<double>;

    /** Operations: */
    enum class op_t
    {
        plus,
        minus,
        mul,
        div,
        assign
    };

    /** Interface to various terms */
    struct term_t : std::enable_shared_from_this<term_t>
    {
        term_t() = default;
        term_t(const term_t &) = delete;
        term_t &operator=(const term_t &) = delete;
        term_t(term_t &&) = delete;
        term_t &operator=(term_t &&) = delete;
        virtual ~term_t() noexcept = default;

        virtual void accept(Visitor &) const = 0;
    };

    /** Special term for a variable expression */
    class var_t : public term_t
    {
        size_t id;

    protected:
        explicit var_t(size_t id) : id{id} {}

    public:
        var_t(const var_t &other) : id{other.id} {}
        var_t &operator=(const var_t &other)
        {
            id = other.id;
            return *this;
        }
        size_t get_id() const { return id; }
        /** returns the value of the variable stored in a state */
        friend class symbol_table_t;

        void accept(Visitor &v) const override { v.visit(*this); }
    };

    class symbol_table_t
    {
        std::vector<std::string> names;
        state_t initial;

    public:
        [[nodiscard]] auto var(std::string name, double init = 0)
        {
            auto id = names.size();
            names.push_back(std::move(name));
            initial.push_back(init);
            return std::shared_ptr<term_t>(new var_t{id});
        }
        const std::string &get_name(size_t id) const { return names[id]; }
        [[nodiscard]] state_t state() const { return initial; }
    };

    class const_t : public term_t
    {
        double value;

    public:
        explicit const_t(double value) : value{value} {}
        double get_value() const { return value; }

        void accept(Visitor &v) const override { v.visit(*this); }
    };

    /** Specialization for unary operations: */
    class unary_t : public term_t
    {
        std::shared_ptr<term_t> term;
        op_t op;

    public:
        explicit unary_t(std::shared_ptr<term_t> term, op_t op = op_t::plus) : term{std::move(term)}, op{op} {}
        const term_t &get_term() const { return *term; }
        op_t get_op() const { return op; }

        void accept(Visitor &v) const override { v.visit(*this); }
    };

    class binary_t : public term_t
    {
        std::shared_ptr<term_t> t1;
        std::shared_ptr<term_t> t2;
        op_t op;

    public:
        binary_t(std::shared_ptr<term_t> t1, std::shared_ptr<term_t> t2, op_t op) : t1{std::move(t1)}, t2{std::move(t2)}, op{op} {}
        const term_t &get_term1() const { return *t1; }
        const term_t &get_term2() const { return *t2; }
        op_t get_op() const { return op; }

        void accept(Visitor &v) const override { v.visit(*this); }
    };

    class assign_t : public term_t
    {
        std::shared_ptr<var_t> var;
        std::shared_ptr<term_t> term;
        op_t op;

    public:
        assign_t(std::shared_ptr<var_t> var, std::shared_ptr<term_t> term, op_t op = op_t::assign) : var{std::move(var)}, term{std::move(term)}, op{op} {}
        const var_t &get_var() const { return *var; }
        const term_t &get_term() const { return *term; }
        op_t get_op() const { return op; }

        void accept(Visitor &v) const override { v.visit(*this); }
    };

    struct Evaluator : public Visitor
    {
        double result;
        state_t state;

        Evaluator(const state_t &s) : result(0), state{s} {}

        void visit(const class term_t &term) override
        {
            term.accept(*this);
        }

        void visit(const class var_t &var) override
        {
            auto val = state[var.get_id()];
            result = val;
        };

        void visit(const class const_t &c) override
        {
            result = c.get_value();
        };

        void visit(const class unary_t &u) override
        {
            u.get_term().accept(*this);
            auto val = result;

            switch (u.get_op())
            {
            case op_t::plus:
                result = val;
                break;
            case op_t::minus:
                result = -val;
                break;
            default:
                throw std::logic_error{"unsupported unary operator"};
            }
        };

        void visit(const class binary_t &b) override
        {
            b.get_term1().accept(*this);
            auto val1 = result;
            b.get_term2().accept(*this);
            auto val2 = result;

            switch (b.get_op())
            {
            case op_t::plus:
                result = val1 + val2;
                break;
            case op_t::minus:
                result = val1 - val2;
                break;
            case op_t::mul:
                result = val1 * val2;
                break;
            case op_t::div:
                if (val2 == 0)
                    throw std::logic_error{"division by zero"};
                result = val1 / val2;
                break;
            default:
                throw std::logic_error{"unsupported binary operator"};
            }
        };

        void visit(const class assign_t &a) override
        {
            a.get_term().accept(*this);
            auto val = result;
            auto id = a.get_var().get_id();
            switch (a.get_op())
            {
            case op_t::assign:
                state[id] = val;
                break;
            case op_t::plus:
                state[id] += val;
                break;
            case op_t::minus:
                state[id] -= val;
                break;
            case op_t::mul:
                state[id] *= val;
                break;
            case op_t::div:
                if (val == 0)
                    throw std::logic_error{"division by zero"};
                state[id] /= val;
                break;
            default:
                throw std::logic_error{"unsupported assignment operator"};
            }
        };
    };

    struct Printer : Visitor
    {
        state_t state;

        Printer(const state_t &s) : state{s} {}

        void visit(const class term_t &term) override
        {
            term.accept(*this);
        }

        void visit(const class var_t &var) override
        {
            std::cout << state[var.get_id()];
        };

        void visit(const class const_t &c) override
        {
            std::cout << c.get_value();
        };

        void visit(const class unary_t &u) override
        {
            std::cout << "(";
            switch (u.get_op())
            {
            case op_t::plus:
                std::cout << "+";
                break;
            case op_t::minus:
                std::cout << "-";
                break;
            default:
                throw std::logic_error{"unsupported unary operator"};
            }
            u.get_term().accept(*this);
            std::cout << ")";
        };

        void visit(const class binary_t &b) override
        {
            std::cout << "(";
            b.get_term1().accept(*this);
            switch (b.get_op())
            {
            case op_t::plus:
                std::cout << "+";
                break;
            case op_t::minus:
                std::cout << "-";
                break;
            case op_t::mul:
                std::cout << "*";
                break;
            case op_t::div:
                std::cout << "/";
                break;
            default:
                throw std::logic_error{"unsupported binary operator"};
            }
            b.get_term2().accept(*this);
            std::cout << ")";
        };

        void visit(const class assign_t &a) override
        {
            std::cout << "(";
            a.get_var().accept(*this);
            switch (a.get_op())
            {
            case op_t::assign:
                std::cout << "=";
                break;
            case op_t::plus:
                std::cout << "+=";
                break;
            case op_t::minus:
                std::cout << "-=";
                break;
            case op_t::mul:
                std::cout << "*=";
                break;
            case op_t::div:
                std::cout << "/=";
                break;
            default:
                throw std::logic_error{"unsupported assignment operator"};
            }
            a.get_term().accept(*this);
            std::cout << ")";
        };

        void operator<<(const term_t &term)
        {
            term.accept(*this);
            std::cout << std::endl;
        }
    };
}

int main()
{
    auto sys = calculator::symbol_table_t{};
    auto a = sys.var("a", 2);
    auto b = sys.var("b", 3);
    auto c = sys.var("c");
    auto state = sys.state();

    std::shared_ptr<calculator::term_t> t = std::make_shared<calculator::binary_t>(a, b, calculator::op_t::plus);
    // calculator::Evaluator e(state);
    calculator::Printer p(state);
    p << *t;
    // std::cout << e.result << std::endl;
    return 0;
}

/**
 * Add your test cases as you go.
 * TODO: introduce visitor interface.
 * TODO: replace operator() with evaluator visitor.
 * TODO: implement printer visitor which prints the terms into output stream.
 * TODO: provide operator<< to your printer implementation to print terms.
 */

using namespace calculator;