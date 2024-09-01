#ifndef _XENON_ECS_CONTAINERTUPLE_
#define _XENON_ECS_CONTAINERTUPLE_

#include <cstddef>
#include <type_traits>
#include <utility>
#include <algorithm>

template<template<typename Contained> typename Container, typename... Types>
struct ContainerTuple;

template<template<typename Contained> typename Container>
struct ContainerTuple<Container> {
	ContainerTuple() = default;
	explicit ContainerTuple([[maybe_unused]] auto arg) {}

	template<typename U>
	Container<U>& get() {
		static_assert(sizeof(U) == 0, "Type not found in tuple.");
	}

	template<typename U>
	const Container<U>& get() const {
		static_assert(sizeof(U) == 0, "Type not found in tuple.");
	}

	static constexpr std::size_t size = 0;
};

template<template<typename Contained> typename Container, typename T, typename... Rest>
struct ContainerTuple<Container, T, Rest...> {
	Container<T> element;
	ContainerTuple<Container, Rest...> rest;

	ContainerTuple() = default;
	explicit ContainerTuple(auto arg) : element(arg), rest(arg) {}
	explicit ContainerTuple(auto arg, auto... args) : element(arg), rest(args...) {}

	template<std::size_t N>
	auto& get() {
		if constexpr (N == 0)
			return element;
		else 
			return rest.template get<N - 1>();
	}

	template<std::size_t N>
	const auto& get() const {
		if constexpr (N == 0)
			return element;
		else
			return rest.template get<N - 1>();
	}

	template<typename U>
	Container<U>& get() {
		if constexpr (std::is_same_v<T, U>)
			return element;
		else
			return rest.template get<U>();
	}

	template<typename U>
	const Container<U>& get() const {
		if constexpr (std::is_same_v<T, U>)
			return element;
		else
			return rest.template get<U>();
	}

	static constexpr std::size_t size = 1 + ContainerTuple<Container, Rest...>::size;
};

template <typename Func, std::size_t N = 0, typename Tuple, typename... Tuples>
void for_each_indexed(Func&& func, [[maybe_unused]] Tuples&... tuples) {
	constexpr std::size_t min = std::min({Tuples::size...});
    if constexpr (N < min) {
        func(N, tuples.template get<N>()...);
        for_each_indexed<Func, N + 1>(std::forward<Func>(func), std::forward<Tuples>(tuples)...);
    }
}

template <std::size_t N = 0, typename Func, typename... Tuples>
void for_each(Func&& func, [[maybe_unused]] Tuples&... tuples) {
	constexpr std::size_t min = std::min({Tuples::size...});
    if constexpr (N < min) {
        func(tuples.template get<N>()...);
        for_each<N + 1>(std::forward<Func>(func), tuples...);
    }
}

#endif // !_XENON_ECS_CONTAINERTUPLE_
