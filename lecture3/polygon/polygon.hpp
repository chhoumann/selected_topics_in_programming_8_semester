#ifndef INCLUDE_POLYGON_HPP
#define INCLUDE_POLYGON_HPP

#include <iosfwd> // I/O stream declarations only (no definitions -- faster to compile)
#include <initializer_list>

/**
 * Exercise to demonstrate the following:
 * 1) basic class structure
 * 2) special constructors and assignment operators
 * 3) other customization points:
 *    a) custom constructors
 *    b) printing to output stream
 *    c) subscripting with []
 *    d) iteration with ranged-for-loop
 * Disclaimer: the polygon class design is NOT a good practice! Not good for the exam!
 * It is artificially inflated just to demonstrate all kinds of constructors.
 * In practice one would use smart-pointers and the rule of zero instead (next lecture).
 * This shows a gist of how standard libraries are implemented from scratch.
 * @author Marius Mikucionis <marius@cs.aau.dk>
 */

/** Simple struct to store 2-dimensional point data */
struct point
{   // struct means class with "public:" section by default
    /** Coordinate x */
    double x = 0;
    /** Coordinate y */
    double y = 0;
    /** Output operator to print into output stream: */
    friend std::ostream& operator<<(std::ostream&, const point&); // output operator
}; // point

/** Aggregate/complex class managing a sequence of points */
class polygon
{
public: // interface:
    // Rule of three: https://en.cppreference.com/w/cpp/language/rule_of_three
    ~polygon() noexcept; // destructor (noexcept is important for std::vector)
    polygon(const polygon&); // copy-constructor
    polygon& operator=(const polygon&); // copy-assignment

    // ... rule of five:
    polygon(polygon&&) noexcept; // move-constructor (noexcept is important for std::vector)
    polygon& operator=(polygon&&) noexcept; // move-assignment (noexcept is important for std::vector)

    // ... rule of six:
    polygon(); // default-constructor

    /** Custom constructors: */
    explicit polygon(size_t count); // custom-constructor(int)
    polygon(const std::initializer_list<point>&); // custom-constructor{...}

    /** Basic field access, note "const"! */
    size_t size() const { return count; }
    bool empty() const { return count == 0; }

    /** Subscripting for mutable access: my_poly[i] */
    point& operator[](size_t i) { return points[i]; }
    /** Subscripting for read-only access: */
    const point& operator[](size_t i) const { return points[i]; }

    /** Iteration and ranged-for-loop support with mutable access: */
    point* begin() { return points; }
    point* end() { return points+count; }

    /** Iteration and ranged-for-loop support with read-only access: */
    const point* begin() const { return points; }
    const point* end() const { return points+count; }

    /** Output operator for printing into output stream: */
    friend std::ostream& operator<<(std::ostream&, const polygon&);

private: // hide the private data:
    size_t count;  // store the number of points
    point* points; // points to data in a contiguous piece of memory ("array" of unknown size)

    void copy(const polygon &other) noexcept;
    void move(polygon &other) noexcept;
}; // polygon

#endif /* INCLUDE_POLYGON_HPP */
