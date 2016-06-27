#pragma once

#include <sstream>
#include <cassert>
#include <iostream>
#include <string>

#include <memory>
#include <type_traits>
#include <list>
#include <utility>
#include <iomanip>
#include <glm.hpp>
#include <cassert>
#include <unordered_set>

using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;


inline bool eeq(float a, float b) {
	static const float EPSILON = 0.002;
	return fabs(a - b) < EPSILON;
}


template<typename T>
class owning_ptr {
	shared_ptr<T> shared;

	explicit owning_ptr(const shared_ptr<T> & shared_, int) :
		shared(shared_) { }


	template <typename T2, typename... Args>
	friend owning_ptr<T2> make_owning(Args&&... args);

	template<typename T2>
	friend class owning_ptr;

public:
	using pointer = T *;

	owning_ptr() { }

	owning_ptr(T * that) : shared(that) { }

	owning_ptr(const owning_ptr<T> & that) = delete;

	void operator=(const owning_ptr<T> & that) = delete;


	template<
		class _Up,
		typename = typename std::enable_if<
			!std::is_array<_Up>::value &&
			std::is_convertible<typename owning_ptr<_Up>::pointer, pointer>::value,
			owning_ptr&
		>::type
	>
	owning_ptr(owning_ptr<_Up> && that) {
		*this = std::move(that);
	}

	~owning_ptr() {
		if (shared) {
			assert(shared.use_count() == 1);
			shared = nullptr;
		}
	}

	template<
		class _Up,
		typename = typename std::enable_if<
			!std::is_array<_Up>::value &&
			std::is_convertible<typename owning_ptr<_Up>::pointer, pointer>::value,
			owning_ptr&
		>::type
	>
	void operator=(owning_ptr<_Up> && that) {
		shared = that.shared;
		that.shared = nullptr;
	}

	shared_ptr<T> release() {
		shared_ptr<T> result = shared;
		shared = nullptr;
		return result;
	}

	T * get() { return shared.get(); }
	T * operator->() { return get(); }

	operator shared_ptr<T>() {
		return shared;
	}
};

template <typename T, typename... Args>
owning_ptr<T> make_owning(Args&&... args) {
   return owning_ptr<T>(std::make_shared<T>(std::forward<Args>(args)...), 0);
}







template<typename... T>
void concatInner(std::stringstream &stream, T&&... args) {
	assert(false);
}

template<typename First, typename... Rest>
void concatInner(std::stringstream &stream, First&& first, Rest&&... rest) {
	stream << first;
	concatInner(stream, std::forward<Rest>(rest)...);
}

template<typename First>
void concatInner(std::stringstream &stream, First&& first) {
	stream << first;
}

template<typename... T>
std::string concat(T&&... args) {
	std::stringstream stream;
	concatInner(stream, std::forward<T>(args)...);
	return stream.str();
}


class VAssertFailedException : public std::runtime_error {
public:
	VAssertFailedException(std::string message) : std::runtime_error(message) { }
};

inline bool vassertImpl(const char * file, int line, const char * cond, const std::string & msg) {
	std::string message = concat(file, ":", line, " ", cond, " was false! ", msg);
	std::cerr << message << std::endl;
	throw VAssertFailedException(message);
}

#define vassert(cond, msgs...) ((cond) || vassertImpl(__FILE__, __LINE__, #cond, concat(msgs)))
#define vfail(msgs...) (throw VAssertFailedException(concat(msgs)))
#define curiosity() (std::cerr << "Curiosity happened " << __FILE__ << ":" << __LINE__ << std::endl)
#define flare(msgs...) (std::cerr << "Flare " << __FILE__ << ":" << __LINE__ << " " << concat(msgs) << std::endl)




class VException : public std::exception {
	std::string message;
public:
	VException(const std::string &e) : message(e) { }

	virtual const char * what() const _NOEXCEPT {
		return message.c_str();
	}
};

std::string readFileAsString(const char *filename);

glm::vec4 randomColor();






// Begin ecatmur's code
template<typename T> struct remove_class { };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...)> { using type = R(A...); };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) const> { using type = R(A...); };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) volatile> { using type = R(A...); };
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) const volatile> { using type = R(A...); };

template<typename T>
struct get_signature_impl { using type = typename remove_class<
    decltype(&std::remove_reference<T>::type::operator())>::type; };
template<typename R, typename... A>
struct get_signature_impl<R(A...)> { using type = R(A...); };
template<typename R, typename... A>
struct get_signature_impl<R(&)(A...)> { using type = R(A...); };
template<typename R, typename... A>
struct get_signature_impl<R(*)(A...)> { using type = R(A...); };
template<typename T> using get_signature = typename get_signature_impl<T>::type;
// End ecatmur's code

// Begin typecase code
template<typename Base, typename FirstSubclass, typename... RestOfSubclasses>
void typecase(
        Base *base,
        FirstSubclass &&first,
        RestOfSubclasses &&... rest) {

    using Signature = get_signature<FirstSubclass>;
    using Function = std::function<Signature>;

    if (typecaseHelper(base, (Function)first)) {
        return;
    }
    else {
        typecase(base, rest...);
    }
}
template<typename Base>
void typecase(Base *) {
    assert(false);
}
template<typename Base, typename T>
bool typecaseHelper(Base *base, std::function<void(T *)> func) {
    if (T *first = dynamic_cast<T *>(base)) {
        func(first);
        return true;
    }
    else {
        return false;
    }
}
// End typecase code


// Begin typecase code
template<typename Base, typename... Subclasses>
void share_typecase(
        shared_ptr<Base> base,
        Subclasses &&... rest);

template<typename Base, typename T>
inline bool share_typecaseHelper(shared_ptr<Base> base, std::function<void(shared_ptr<T>)> func) {
    if (shared_ptr<T> first = std::dynamic_pointer_cast<T>(base)) {
        func(first);
        return true;
    }
    else {
        return false;
    }
}
template<typename Base, typename FirstSubclass, typename... RestOfSubclasses>
inline void share_typecase(
        shared_ptr<Base> base,
        FirstSubclass &&first,
        RestOfSubclasses &&... rest) {

    using Signature = get_signature<FirstSubclass>;
    using Function = std::function<Signature>;

    if (share_typecaseHelper(base, (Function)first)) {
        return;
    }
    else {
        share_typecase(base, rest...);
    }
}
template<typename Base>
inline void share_typecase(shared_ptr<Base>) {
    assert(false);
}
template<typename Base, typename... Subclasses>
void share_typecase(
        shared_ptr<Base> base,
        Subclasses &&... rest) {
   assert(false);
}
// End typecase code


template<typename Base, typename... Subclasses>
inline void share_typecase(
        owning_ptr<Base> base,
        Subclasses &&... rest) {
   shared_ptr<Base> shared = base;
   share_typecase(shared, rest...);
}
