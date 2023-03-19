#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include <sstream>

struct json_writer_t;

struct json_ostream
{
    std::ostream &os;
};

struct json_istream
{
    std::istream &is;
};

template <typename T>
concept bool_value = std::is_same_v<T, bool>;

template <typename T>
concept number_value = std::is_arithmetic_v<T>;

template <typename T>
concept text_value = std::is_same_v<T, std::string>;

template <typename T>
concept array_value = requires(T t) {
                          std::begin(t),
                              std::end(t);
                      };

template <typename T>
concept object_value = requires(T t) {
                           t.accept(std::declval<json_writer_t>());
                       };

template <typename T>
concept tuple_value = requires(T t) {
                          std::tuple_size<T>::value;
                      };

struct json_writer_t
{
    json_ostream &jos;

    json_writer_t(json_ostream &jos) : jos(jos) {}

    template <typename Data>
    void visit(std::string const &name, const Data &data)
    {
        if (!name.empty())
            jos.os << "\"" << name << "\": ";
        if constexpr (bool_value<Data>)
        {
            jos.os << (data ? "true" : "false");
        }
        else if constexpr (number_value<Data>)
        {
            jos.os << data;
        }
        else if constexpr (text_value<Data>)
        {
            jos.os << "\"" << data << "\"";
        }
        else if constexpr (array_value<Data>)
        {
            jos.os << "[";
            auto it = std::begin(data);
            if (it != std::end(data))
            {
                jos.os << *it;
                ++it;
            }
            for (; it != std::end(data); ++it)
            {
                jos.os << ", " << *it;
            }
            jos.os << "]";
        }
        else if constexpr (object_value<Data>)
        {
            jos.os << "{ ";
            data.accept(*this);
            jos.os << "}";
        }
        else if constexpr (tuple_value<Data>)
        {
            jos.os << "{";
            visit_tuple(*this, data, std::make_index_sequence<std::tuple_size_v<Data>>{});
            jos.os << "}";
        }
        if (!name.empty())
            jos.os << ", ";
    }
};

template <typename Tuple, std::size_t... Is>
void visit_tuple(json_writer_t &writer, const Tuple &t, std::index_sequence<Is...>)
{
    ((writer.visit(std::to_string(Is + 1), std::get<Is>(t))), ...);
}

template <typename Data>
json_ostream &operator<<(json_ostream &jos, const Data &data)
{
    json_writer_t writer(jos);
    writer.visit("", data);
    return jos;
}

struct json_reader_t
{
    json_istream &jis;

    json_reader_t(json_istream &jis) : jis(jis) {}

    template <typename Data>
    void read(const std::string &key, Data &data)
    {
        char ch;
        while (jis.is >> ch && ch != ':')
        {
            if (ch == '\"')
            {
                std::string current_key;
                std::getline(jis.is, current_key, '\"');
                if (current_key == key)
                {
                    jis.is >> ch; // Skip the ':' character
                    break;
                }
            }
        }

        if constexpr (bool_value<Data>)
        {
            jis.is >> std::boolalpha >> data;
        }
        else if constexpr (number_value<Data>)
        {
            jis.is >> data;
        }
        else if constexpr (text_value<Data>)
        {
            jis.is.ignore(); // Ignore initial quote
            std::getline(jis.is, data, '\"');
        }
        else if constexpr (array_value<Data>)
        {
            char ch;
            jis.is >> ch; // Expect '['
            if (ch != '[')
            {
                throw std::runtime_error("Expected '['");
            }

            if (jis.is.peek() == ']')
            {
                jis.is.ignore(); // Ignore empty array ']'
            }
            else
            {
                size_t index = 0;
                while (true)
                {
                    // if (index >= data.size())
                    // {
                    //     throw std::runtime_error("Array size mismatch");
                    // }

                    read(std::to_string(index), data[index]);
                    ++index;

                    jis.is >> ch; // Expect ',' or ']'
                    if (ch == ']')
                    {
                        break;
                    }
                    // else if (ch != ',')
                    // {
                    //     throw std::runtime_error("Expected ',' or ']'");
                    // }
                }
            }
        }
        else if constexpr (object_value<Data>)
        {
            char ch;
            jis.is >> ch; // Expect '{'
            if (ch != '{')
            {
                throw std::runtime_error("Expected '{'");
            }

            data.accept(*this);

            jis.is >> ch; // Expect '}'
            if (ch != '}')
            {
                throw std::runtime_error("Expected '}'");
            }
        }
    }

    template <typename Data>
    void visit(std::string const &name, Data &data)
    {
        read(name, data);
    }
};

template <typename Data>
json_istream &operator>>(json_istream &jis, Data &data)
{
    json_reader_t reader(jis);
    data.accept(reader);
    return jis;
}

struct Address
{
    std::string street;
    std::string city;
    int zip;

    template <typename Visitor>
    void accept(Visitor &&visitor) const
    {
        visitor.visit("street", street);
        visitor.visit("city", city);
        visitor.visit("zip", zip);
    }

    template <typename Visitor>
    void accept(Visitor &&visitor)
    {
        visitor.visit("street", street);
        visitor.visit("city", city);
        visitor.visit("zip", zip);
    }
};

struct Person
{
    std::string name;
    int age;
    Address address;
    std::vector<std::string> hobbies;

    template <typename Visitor>
    void accept(Visitor &&visitor) const
    {
        visitor.visit("name", name);
        visitor.visit("age", age);
        visitor.visit("address", address);
        visitor.visit("hobbies", hobbies);
    }

    template <typename Visitor>
    void accept(Visitor &&visitor)
    {
        visitor.visit("name", name);
        visitor.visit("age", age);
        visitor.visit("address", address);
        visitor.visit("hobbies", hobbies);
    }
};

int main()
{
    Person person{
        "Christian",
        24,
        {"Gadenavn", "Aalborg", 9000},
        {"reading", "traveling", "programming"},
    };

    // Serialize
    json_ostream jos{std::cout};
    jos << person;
    std::cout << std::endl;

    // Deserialize
    std::string serialized_person = "{\"name\":\"Christian\",\"age\":24,\"address\":{\"street\":\"Gadenavn\",\"city\":\"Aalborg\",\"zip\":9000},\"hobbies\":[\"reading\",\"traveling\",\"programming\"]}";
    std::stringstream ss(serialized_person);
    json_istream jis{ss};
    Person deserialized_person;
    jis >> deserialized_person;

    std::cout << "Deserialized values: " << deserialized_person.name << ", " << deserialized_person.age << ", " << deserialized_person.address.street << ", " << deserialized_person.address.city << ", " << deserialized_person.address.zip << std::endl;

    std::cout << "Hobbies: ";
    for (const auto &hobby : deserialized_person.hobbies)
    {
        std::cout << hobby << ", ";
    }
    std::cout << std::endl;

    return 0;
}
