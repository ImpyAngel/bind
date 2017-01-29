#ifndef BIND_H
#define BIND_H

#include <tuple>
#include <functional>

template <class F, class... First_arg>
class bind_t {

template <class FF, class... Args>
friend bind_t bind(FF&& f, Args&&... args);

public:
	template < class... New_args>
	auto operator()(New_args&&... new_args) {
		return call(typename make_seq<std::tuple_size<std::tuple<First_arg...>>::value>::type(), std::forward<New_args>(new_args)...);
	}
private:
	F f;
	std::tuple<First_arg...> tuple;

	bind_t(F&& f, std::tuple<First_arg...>&& tuple) : f(std::forward<F>(f)),
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

	template<size_t N, class... New_args>
	decltype(auto) get(std::_Placeholder<N>, New_args&& ... new_args) const {
		return std::get<N - 1>(std::forward_as_tuple(new_args...));
	}

	template<size_t... Indexes, class ... New_args>
	auto call(seq<Indexes...>, New_args&& ... new_args) {
		return f(arg_get(std::get<Indexes>(tuple), new_args...)...);
	}
};
template <class R, class F, class... Args>
bind_t<F, Args...> bind(F&& f, Args&&... args ) {
	return bind_t<F, Args...>(std::forward<F>(f), 
		std::tuple<Args...>(std::forward<Args...>(args)...));
} 
#endif