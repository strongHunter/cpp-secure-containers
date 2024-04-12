# cpp-secure-containers

Простая библиотека для C++, предоставляющая безопасные контейнеры с автоматическим освобождением памяти.
Включает в себя реализацию `vector_secure` и `basic_string_secure` - аналоги `std::vector` и `std::basic_string`.

Объекты `vector_secure` и `basic_string_secure` наследуют весь набор функций и операторов из `std`. Но в качестве защитной меры запрешено явное копирование объектов через конструкторы копирования. Вместо этого предоставлены статические методы `copy`.

## Особенности

* Безопасные аналоги стандартных `std` контейнеров.
* Запрет явного копирования через конструктор копирования и оператор присваивания
  * Возможность копирования через вызов статических методов `copy`
* Пользовательский аллокатор `SanitizingAllocator`, который обеспечивает очистку памяти.
* Поддержка различных типов, включая `vector_secure<T>`, `string_secure`, `wstring_secure`, `u16string_secure` и другие.
* Легкая интеграция в код, благодаря совместимости со стандартными контейнерами и привычному синтаксису.


## Примеры использования
### vector_secure
#### Основное использование vector_secure
```c++
#include <iostream>
#include "SecureContainers.h"

int main()
{
    int* ptr;
    size_t size;

    {
        vector_secure<int> v = { 8, 4, 5, 9 };

        ptr = v.data();
        size = v.size();

        for (size_t i = 0; i < size; ++i)
            std::cout << ptr[i] << ' ';
        std::cout << std::endl;
    }

    for (size_t i = 0; i < size; ++i)
        std::cout << ptr[i] << ' ';
    std::cout << std::endl;
}
```
---
#### Передача vector_secure в функцию
```c++
#include <iostream>
#include "SecureContainers.h"

void func_object(vector_secure<int> v) {}
void func_lvalue(vector_secure<int>& v) {}
void func_rvalue(vector_secure<int>&& v) {}

int main()
{
    vector_secure<int> v1 = { 8, 4, 5, 9 };
    vector_secure<int> v2 = { 8, 4, 5, 9 };
    vector_secure<int> v3 = { 8, 4, 5, 9 };

    //    func_object(v1); // not compiled
    func_object(std::move(v1));
    std::cout << "v1.empty() - "
              << (v1.empty() ? "true" : "false") << std::endl; // true

    func_lvalue(v2);
    std::cout << "v2.empty() - "
              << (v2.empty() ? "true" : "false") << std::endl; // false

    func_rvalue(std::move(v3));
    std::cout << "v3.empty() - "
              << (v3.empty() ? "true" : "false") << std::endl; // false
}
```
---
#### Копирование vector_secure
```c++
#include "SecureContainers.h"

void func_object(vector_secure<int> v) {}

int main()
{
    vector_secure<int> v = { 8, 4, 5, 9 };

    auto v_copy1 = vector_secure<int>::copy(v);
    auto v_copy2 = vector_secure<int>::copy(v.begin(), v.end());

    func_object(vector_secure<int>::copy(v));
}
```
---
### string_secure
#### Основное использование string_secure
```c++
#include <iostream>
#include <iomanip>
#include "SecureContainers.h"

void print(const char* ptr, size_t size)
{
    for (size_t i = 0; i < size; ++i)
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)ptr[i] << ' ';
    std::cout << std::endl;
}

int main()
{
    const char* ptr;
    size_t size;

    {
        string_secure str = "Hello, world!";

        ptr = str.c_str();
        size = str.size();

        print(ptr, size);
    }

    print(ptr, size);
}
```
---
#### Передача string_secure в функцию
```c++
#include <iostream>
#include "SecureContainers.h"

void func_object(string_secure s) {}
void func_lvalue(string_secure& s) {}
void func_rvalue(string_secure&& s) {}

int main()
{
    string_secure s1 = "Hello, world!";
    string_secure s2 = "Hello, world!";
    string_secure s3 = "Hello, world!";

    //    func_object(s1); // not compiled
    func_object(std::move(s1));
    std::cout << "s1.empty() - "
              << (s1.empty() ? "true" : "false") << std::endl; // true

    func_lvalue(s2);
    std::cout << "s2.empty() - "
              << (s2.empty() ? "true" : "false") << std::endl; // false

    func_rvalue(std::move(s3));
    std::cout << "s3.empty() - "
              << (s3.empty() ? "true" : "false") << std::endl; // false
}
```
#### Копирование string_secure
```c++
#include "SecureContainers.h"

void func_object(string_secure s) {}

int main()
{
    string_secure s = "Hello, world!";

    auto v_copy1 = string_secure::copy(s);
    auto v_copy2 = string_secure::copy(s.begin(), s.end());

    func_object(string_secure::copy(s));
}
```
