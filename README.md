# Delegate_C - проект, демонстрирующий один из вариантов пользовательского аналога std::function.
Класс Delegate может хранить указатель на функцию, либо указаетль на класс и его метод (константный или нет), либо класс, у которого надлежащим образом перегружен оператор ().
Класс имеет перегруженный оператор (), посредством которого выполняется код функции.
Класс имеет несколько вариантов конструктора: для функции, для метода класса и для класса с перегруженным оператором ().
Клас имеет конструктор копирования и оператор присваивания, который можно использовать по отношению к самому и к указателям на функции.
