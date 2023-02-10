#include <iostream>
#include <cstring>

class String
{
public:
    String() = default;
    String(const char *string)
    {
        printf("Created!\n");
        m_Size = strlen(string);
        m_Data = new char[m_Size];
        memcpy(m_Data, string, m_Size);
    }

    String(const String &other)
    {
        printf("Copied!\n");
        m_Size = other.m_Size;
        m_Data = new char[m_Size];
        memcpy(m_Data, other.m_Data, m_Size);
    }

    String(String &&other) noexcept
    {
        printf("Moved!\n");
        m_Size = other.m_Size;
        m_Data = other.m_Data;

        // Need to do this, because when `other` gets destroyed, this obj would otherwise get ruined.
        // In essence, we've just rewired the pointers, such that we're taking over this object.
        other.m_Size = 0;
        other.m_Data = nullptr;
    }

    String &operator=(String &&other) noexcept
    {
        printf("Moved with assignment!\n");

        if (this == &other)
            return *this;

        // prevent memory leaks. But if you assign to the same object, that's bad. So need above check.
        delete[] m_Data; 

        m_Size = other.m_Size;
        m_Data = other.m_Data;

        // Need to do this, because when `other` gets destroyed, this obj would otherwise get ruined.
        // In essence, we've just rewired the pointers, such that we're taking over this object.
        other.m_Size = 0;
        other.m_Data = nullptr;
    }

    ~String()
    {
        printf("Destroyed!\n");
        delete m_Data;
    }

    void Print()
    {
        for (uint32_t i = 0; i < m_Size; i++)
            printf("%c", m_Data[i]);

        printf("\n");
    }

private:
    char *m_Data;
    uint32_t m_Size;
};

class Entity
{
public:
    Entity(const String &name) : m_Name(name){};
    // Need to explicitly cast to temporary to ensure the constructor is used.
    // but actually you'd use std::move, rather than casting to (String&&)name.
    Entity(String &&name) : m_Name(std::move(name)){};

    void PrintName()
    {
        m_Name.Print();
    }

private:
    String m_Name;
};

int main()
{
    Entity entity(String("Christian"));

    String string = "Hello";
    String dest(std::move(string)); // String dest = std::move(string); also works

    String apple = "Apple";
    String dest2;

    // Observe that, from here, we're basically moving "Apple" from `apple` into `dest`.
    std::cout << "Apple: ";
    apple.Print();
    std::cout << "Dest: ";
    dest2.Print();

    dest2 = std::move(apple);

    std::cout << "Apple: ";
    apple.Print();
    std::cout << "Dest: ";
    dest2.Print();

    entity.PrintName();
}