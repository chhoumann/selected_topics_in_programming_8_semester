template <typename T>
class Vector
{
private:
    T *elem;
    int sz;

public:
    Vector(int s);
    ~Vector() { delete[] elem; }

    T &operator[](int i);
    const T &operator[](int i) const;
    int size() const { return sz; }
};

template<typename Container, typename Value>
Value sum(const Container& c, Value v) {
    for (auto x : c)
        v += x;

    return v;
}