#ifndef BIND_H
#define BIND_H

#include <tuple>
#include <functional>
#include <iostream>

template <class F, class... First_arg>
struct my_bind_t {

template <class FF, class... Args>
friend my_bind_t<FF, Args ...> my_bind(FF&& f, Args&&... args);

public:
	template < class... New_args>
	auto operator()(New_args&&... new_args) {
		return call(typename make_seq<std::tuple_size<std::tuple<First_arg...>>::value>::type(), std::forward<New_args>(new_args)...);
	}
private:
	F f;
	std::tuple<First_arg...> tuple;

	my_bind_t(F&& f, std::tuple<First_arg...>&& tuple) : f(std::forward<F>(f)),
	tuple(std::forward<std::tuple<First_arg...>>(tuple)) {}

	template<size_t... Hs>
    struct seq {};

    template<size_t H, size_t... Hs>
    struct make_seq {
        typedef typename make_seq<H - 1, H - 1, Hs...>::type type;
    };

    template<size_t... Hs>
    struct make_seq<0, Hs...> {
        typedef seq<Hs...> type;
	};
	
	template<class Old_arg, class... New_args>
	Old_arg&& get(Old_arg&& old_arg, New_args&& ...) const {
		return old_arg;
	}

	template<int N, class... New_args>
	auto&& get(std::_Placeholder<N> const&, New_args&& ... new_args) const {
		return std::get<N - 1>(std::forward_as_tuple(new_args...));
	}

	template<int N, class... New_args, class H, class... Hs>
	auto get(my_bind_t<H, Hs...>& rhs, New_args&& ...  new_args) const {
		return rhs(std::forward<New_args>(new_args)...);
	}

	template<size_t... Indexes, class ... New_args>
	auto call(seq<Indexes...>, New_args&& ... new_args) {
		return f(get(std::get<Indexes>(tuple), new_args...)...);
	}
};


template <class F, class... Args>
my_bind_t<F, Args...> my_bind(F&& f, Args&&... args){
	return my_bind_t<F, Args...>(std::forward<F>(f), 
		std::tuple<Args...>(std::forward<Args>(args)...));

	//std::cout << get(std::placeholders::_1, 1) << std::endl;
} 
#endif