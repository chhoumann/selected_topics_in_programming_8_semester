
#include "calculator.hpp"

namespace calculator
{
    double expr_t::operator()(state_t &s)
    {
        return term->operator()(s);
    }

    double const_t::operator()(state_t &s)
    {
        return value;
    }

    double unary_t::operator()(state_t &s)
    {
        auto value = term->operator()(s);

        switch (op)
        {
        case op_t::plus:
            return +value;
        case op_t::minus:
            return -value;
        default:
            throw std::runtime_error("unary operator not implemented");
        }
    }

    double binary_t::operator()(state_t &s)
    {
        auto left_value = left->operator()(s);
        auto right_value = right->operator()(s);

        switch (op)
        {
        case op_t::plus:
            return left_value + right_value;
        case op_t::minus:
            return left_value - right_value;
        case op_t::multiply:
            return left_value * right_value;
        case op_t::divide:
            return left_value / right_value;
        default:
            throw std::runtime_error("binary operator not implemented");
        }
    }

    double assign_t::operator()(state_t &s)
    {
        auto value = term->operator()(s);

        switch (op)
        {
        case op_t::assign:
            var = std::make_shared<var_t>(value);
            return value;
        default:
            throw std::runtime_error("assignment operator not implemented");
        }
    }
}