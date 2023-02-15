#include <vector>
#include <memory>
#include "point.h"

class Polyline
{
private:
    std::shared_ptr<std::vector<point>> points = std::make_shared<std::vector<point>>();

public:
    Polyline(){
        std::cout << "Polyline default constructor." << std::endl;
    };

    explicit Polyline(const std::vector<point> &points) 
        : points{std::make_shared<std::vector<point>>(points)}
    {
        std::cout << "Polyline custom constructor." << std::endl;
    }

    Polyline(const Polyline &other) 
        : points{other.points}
    {
        std::cout << "Polyline copy constructor." << std::endl;
    }

    [[nodiscard]] std::vector<point>::iterator begin() const;
    [[nodiscard]] std::vector<point>::iterator end() const;

    void clear() const;
    [[nodiscard]] std::vector<point> *get() const;
    explicit operator bool() const;

    void operator+=(const point &rhs);
    Polyline &operator=(const Polyline &other);
};