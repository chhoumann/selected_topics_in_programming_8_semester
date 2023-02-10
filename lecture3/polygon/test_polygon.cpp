#include "polygon.hpp"
#include <iostream>
#include <vector>

/**
 * Code to test the understanding of special member functions and references.
 * @author Marius Mikucionis <marius@cs.aau.dk>
 */

polygon polygon_factory(size_t count){ return polygon(count); }

void draw_value(polygon) { std::cout << "passed-by-value\n"; }
void draw(const polygon&) { std::cout << "passed-by-const-reference\n"; }
void draw(polygon&) { std::cout << "passed-by-lvalue-reference\n"; }
void draw(polygon&&) { std::cout << "passed-by-rvalue-reference\n"; }

int main()
{
    std::string ruler(40, '-');
    // TODO: Replace "???" with your predictions: what gets printed after each line?
    //       Note that some statements may print more than one line.
    // TODO: After all "???" are gone, step through the program in debugger and check your answers against output.
    // TODO: Answer the additional questions below.
    {   // for example:
        std::cout << ruler << std::endl;
        polygon poly;
        // default-constructor
        // destructor
    }
    {
        std::cout << ruler << std::endl;
        polygon poly(1);
        // custom-constructor(1)
        // destruct
    }
    {
        std::cout << ruler << std::endl;
        //polygon poly = 1;
        // custom-constructor(1)
        // destruct
        // not possible due to explicit now
    }
    {
        std::cout << ruler << std::endl;
        const polygon poly0;
        // default-constructor
        // destruct

        std::cout << ruler << std::endl;
        polygon poly1 = poly0;
        // copy constructor

        std::cout << ruler << std::endl;
        polygon poly2 = polygon(2);
        // custom-constructor(2) -> copy constructor | was custom-constructor(2) only

        std::cout << ruler << std::endl;
        polygon poly3{{1,2},{3,4},{5,6},{7,8}};
        // creates a bunch of points inside, so custom-constructor{point{1,2},point{3,4},point{5,6},point{7,8}}

        std::cout << ruler << std::endl;
        poly2 = poly1;
        // copy assignment because poly1 isn't rvalue

        std::cout << ruler << std::endl;
        poly2 = polygon_factory(3);
        // move assignment of custom-constructor(3) because return val is rvalue, so tmp moved

        std::cout << ruler << std::endl;
        draw_value(poly0);
        // pass by value | actually seems to do copy-constructor first

        std::cout << ruler << std::endl;
        draw(poly0);
        // pass by const reference

        std::cout << ruler << std::endl;
        draw(poly1);
        // pass by lvalue reference

        std::cout << ruler << std::endl;
        draw(polygon_factory(4));
        // pass by rvalue reference

        std::cout << ruler << std::endl;
        std::vector<polygon> polies(4, poly3);
        // didn't know. answer is 4x copy constructor.

        std::cout << ruler << std::endl;
        polies.push_back(poly3);
        // first copy poly3, then move every individual point to reallocate space (increasing capacity *2).
        // many ops because capacity is 4, but has to increase to 8, so elements are moved

        std::cout << ruler << std::endl;
        polies.push_back(poly3);
        // capacity supports it, so just copied. no moves (capacity = 8)

        std::cout << ruler << std::endl;
        for (const polygon &p : polies)
            std::cout << p << " at " << &p << '\n';
        // made more efficient by using references, not copying each point with `polygon p : ...`
    }
    {
        std::cout << ruler << std::endl;
        // draw(1);
        // answer was to mark the `polygon(size_t count);` constructor as explicit, to disallow implicit conversions
        // What is going on here? Why "1" is treated as polygon? This is unlikely to be intentional.
        // Can we forbid the last statement without breaking the previous code?
        // Fix it in polygon.hpp so that this line does not compile.
    }
    {
        std::cout << ruler << std::endl;
        polygon poly = polygon(2);
        std::cout << poly << " at " << &poly << std::endl;
        draw_value(std::move(poly));
        // should be pass by value
        std::cout << poly << " at " << &poly << std::endl;
        // What was inside poly before draw_value call?
        // What is inside poly after draw_value call? Why?
        // because we move it, we delete the points & set to nullptr, so will be empty
    }
    {
        std::cout << ruler << std::endl;
        polygon poly = polygon(2);
        std::cout << poly << " at " << &poly << std::endl;
        draw(std::move(poly));
        // pass by rvalue reference, it's literally move
        std::cout << poly << " at " << &poly << std::endl;
        // What was inside poly before draw call?
        // What is inside poly after draw call? Why?
        // it's the same: two points. can't explain why, though.
    }
    std::cout << ruler << ruler << std::endl;
}
