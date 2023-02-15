#include <iostream>
#include "polyline.h"

std::vector<point>::iterator Polyline::begin() const {
    return points->begin();
}

std::vector<point>::iterator Polyline::end() const {
    return points->end();
}

void Polyline::clear() const {
    std::cout << "Cleared points." << std::endl;
    points->clear();
}

std::vector<point>* Polyline::get() const {
    return points.get();
}

Polyline::operator bool() const {
    return !points->empty();
}

void Polyline::operator+=(const point &rhs) {
    // Check if reference count of points > 1
    if (points.use_count() > 1) {
        // If this is the case, we create a new instance of std::vector<point> and copy the content of points into it.
        // Then, we reset points to the new instance.
        std::cout << "Performed copy-on-write" << std::endl;
        points = std::make_shared<std::vector<point>>(*points);
    }

    points->push_back(rhs);
}

Polyline& Polyline::operator=(const Polyline& other) {
    if (this == &other) {
        return *this;
    }

    std::cout << "Copy assignment operator." << std::endl;
    points = other.points;

    return *this;
}