#include "polygon.hpp" // include your headers first to test that they are self-contained
#include <iostream>

/**
 * Implementation of the polygon class methods.
 * They are mostly printing, but other content (copy and move constructor and assignment)
 * are representative of good practice. There are still some minor details left to fix.
 * @author Marius Mikucionis <marius@cs.aau.dk>
 */

/**
 * 1) Very important: fix the memory leak. Address sanitizer may provide a quick help.
 * 2) Change the initializations to uniform initializations with curly braces.
 *          Be careful, not all make sense.
 * 3) move-constructor and move-assignment operator have identical code,
 *          which is difficult to maintain. How can we apply the DRY principle? Fix it!
 *          DRY = Don't Repeat Yourself, i.e. get rid of the code duplicates by reusing the code.
 * 4) copy-constructor and copy-assignment operator have almost identical code,
 *          which is difficult to maintain. How can we apply the DRY principle? Fix it!
 * 5) Nearly all constructors have identical initializations, DRY them by using constructor delegation.
 * 6) Use std::transform instead of for() loop when copying array data.
 * 7) Compare the performance of copy-assignment vs move-assignment (mentally).
 * 8) Install SonarLink plugin for Clion (Settings -> Plugins -> search for SonarLint)
 *          and check if there are more issues in this file and polygon.hpp.
 */

std::ostream& operator<<(std::ostream& os, const point& p)
{
    return os << "point{" << p.x << ',' << p.y << "}";
}

polygon::~polygon() noexcept
{
    delete[] points;

    std::cout << "destructor\n";
}

polygon::polygon():
    polygon{0} // initialize
{
    std::cout << "default-constructor\n";
}

polygon::polygon(size_t count):
    count(count), points(count ? new point[count]() : nullptr)
{
    std::cout << "custom-constructor(" << count << ")\n";
}

polygon::polygon(const std::initializer_list<point>& ps):
    polygon{ps.size()}
{
    std::cout << "custom-constructor{";
    for (auto i=0u; i<count; ++i)
        points[i] = std::data(ps)[i];

    if (count) {
        std::cout << points[0];
        for (auto i=1u; i<count; ++i)
            std::cout << ',' << points[i];
    }
    std::cout << "}\n";
}

polygon& polygon::operator=(const polygon& other)
{
    if (this != &other) {
        std::cout << "copy-assignment\n";
        // First, release our resources (fragments the dynamic memory, not nice):
        delete [] points;
        // Second, allocate memory:
        //count = other.count; // copy
        //points = new point[count]; // allocate -- expensive
        // Third, copy the data:
        // for (auto i {0u}; i < count; ++i) // many assignments -- expensive
        //     points[i] = other[i]; // copy-assignment -- potentially expensive

        copy(other);
    }
    return *this; // remember to return, otherwise wicked memory issues
}

polygon::polygon(const polygon& other):
{
    std::cout << "copy-constructor\n";
    //for (auto i{0u}; i<count; ++i)
    //    points[i] = other[i]; // assignment operation: potentially expensive
    copy(other);
}

void polygon::copy(const polygon& other) noexcept
{
    count = other.count;
    points = new point[count];

    std::transform(other.begin(), other.end(), points, [](auto&& point){return point;});
}

polygon& polygon::operator=(polygon&& other) noexcept
{
    std::cout << "move-assignment\n";
    // The other is about to be destroyed, so let's just put our garbage there.
    // Overall, this boils down to a simple resource exchange:
    move(other);
    // no allocation, no element copying => very efficient, no chance for exceptions
    return *this; // remember to return, otherwise all kinds of weird memory issues
}

polygon::polygon(polygon&& other) noexcept: count(0), points(nullptr)
{
    std::cout << "move-constructor\n";
    move(other);
}

void polygon::move(polygon& other) noexcept {
    std::swap(count, other.count);   // cheap
    std::swap(points, other.points); // super cheap!
}

std::ostream& operator<<(std::ostream& os, const polygon& poly)
{
    if (poly.empty()) {
        os << "(empty polygon)";
    } else {
        for (const auto &point : poly) // is this really efficient? | DONE! from `auto point` to `const auto &point`
            os << point << " ";
    }
    return os; // remember to return, otherwise wicked memory issues
}
