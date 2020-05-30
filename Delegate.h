#pragma once

template<typename>
class Delegate;

template<class ReturnType, class ... ArgsType>
class Delegate<ReturnType(ArgsType ...)>
{
private:
	// базовый класс для вызова функции исполнения
	class BaseInvoker
	{
	public:
		BaseInvoker() {}
		~BaseInvoker() {}
		virtual ReturnType Invoke(ArgsType ... args) = 0;
		virtual BaseInvoker* Clone() = 0;
	};
	// класс для вызова обычных функций и лямбда-выражений
	class FreeInvoker : public BaseInvoker
	{
	private:
		ReturnType(*Function)(ArgsType ...);
	public:
		FreeInvoker(ReturnType(*func)(ArgsType ...)) : Function(func) {}
		~FreeInvoker() {}
		ReturnType Invoke(ArgsType ... args) { return Function(args ...); }
		BaseInvoker* Clone() { return static_cast<BaseInvoker*>(new FreeInvoker(Function)); }
	};
	// класс для вызова методов классов
	template <class ClassType>
	class MembInvoker : public BaseInvoker
	{
	private:
		ReturnType(ClassType::* Function)(ArgsType ...);
		ClassType* Class;
	public:
		MembInvoker(ReturnType(ClassType::* func)(ArgsType ...), ClassType* obj) : Function(func), Class(obj) {}
		~MembInvoker() {}
		ReturnType Invoke(ArgsType ... args) { return (Class->*Function)(args ...); }
		BaseInvoker* Clone() { return static_cast<BaseInvoker*>(new MembInvoker(Function, Class)); }
	};
	// класс для вызова конст. методов класса
	template <class ClassType>
	class CmemInvoker : public BaseInvoker
	{
		ReturnType(ClassType::* Function)(ArgsType ...) const;
		const ClassType* const Class;
	public:
		CmemInvoker(ReturnType(ClassType::* func)(ArgsType ...) const, const ClassType* const obj) : Function(func), Class(obj) {}
		~CmemInvoker() {}
		ReturnType Invoke(ArgsType ... args) { return (Class->*Function)(args ...); }
		BaseInvoker* Clone() { return static_cast<BaseInvoker*>(new CmemInvoker(Function, Class)); }
	};
	// вызов перегруженного оператора класса
	template <class ClassType>
	class ClssInvoker : public BaseInvoker
	{
		ClassType Class;
	public:
		ClssInvoker(const ClassType& obj) : Class(obj) {}
		~ClssInvoker() {}
		ReturnType Invoke(ArgsType ... args) { return Class(args ...); }
		BaseInvoker* Clone() { return static_cast<BaseInvoker*>(new ClssInvoker(Class)); }
	};

	BaseInvoker* pInvoker;
	// очищение делегата
	void Clear() {
		if (pInvoker)
		{
			delete pInvoker;
			pInvoker = nullptr;
		}
	}
public:
	//конструктор по-умолчанию
	Delegate() : pInvoker(nullptr) {}
	// деструктор
	~Delegate() { Clear(); }
	// кнструктор для обычной функции
	Delegate(ReturnType(*func)(ArgsType ...)) : pInvoker(new FreeInvoker(func)) {}
	// конструктор для метода класса
	template <class ClassType>
	Delegate(ReturnType(ClassType::* func)(ArgsType ...), ClassType* obj) : pInvoker(new MembInvoker<ClassType>(func, obj)) {}
	// конструктор для конст. метода класса
	template <class ClassType>
	Delegate(ReturnType(ClassType::* func)(ArgsType ...) const, const ClassType* const obj) : pInvoker(new CmemInvoker<ClassType>(func, obj)) {}
	// конструктор для класса с перегруженным оператором ()
	template <class ClassType>
	Delegate(const ClassType& obj) : pInvoker(new ClssInvoker<ClassType>(obj)) {}
	// конструктор копирования
	Delegate(const Delegate& other) : pInvoker(other.pInvoker->Clone()) {}
	// перегруженный оператор вызова функции с аргументами args
	ReturnType operator () (ArgsType ... args) { return pInvoker->Invoke(args ...); }
	// оператор присваивания
	void operator = (const Delegate& other) { pInvoker = other.pInvoker->Clone(); }
	// присваивание нового указателя на функцию
	void operator = (ReturnType(*func)(ArgsType ...)) {
		Clear(); pInvoker = new FreeInvoker(func);
	}
	// присваивание классас перегруженным оператором ()
	template <class ClassType>
	void operator = (const ClassType& obj) {
		Clear(); pInvoker = new ClssInvoker<ClassType>(obj);
	}
};