#ifndef BIND_H

#define BIND_H

#include <tuple>
#include <functional>

template<class F, class... WAS>
class my_bind_type {
    template<class F_, class... Old_Args_>
    friend my_bind_type<F_, Old_Args_...> my_bind(F_&&, Old_Args_&& ...);

public:
    template<class... New_Args>
    auto operator()(New_Args&& ... new_args) {
        return call(typename tuple_seq_generator<std::tuple_size<tuple_t>::value>::type(),
                    std::forward<New_Args>(new_args)...);
    }

private:
    typedef F func_t;
    typedef std::tuple<WAS...> tuple_t;

    func_t f;
    tuple_t tuple;

    my_bind_type(F&& f, std::tuple<WAS...>&& tuple) : f(std::forward<F>(f)),
                                                           tuple(std::forward<tuple_t>(tuple))
    {}

    template<size_t... Indexes>
    struct tuple_seq_t {};


    template<size_t Head, size_t... Indexes>
    struct tuple_seq_generator {
        typedef typename tuple_seq_generator<Head - 1, Head - 1, Indexes...>::type type;
    };


    template<size_t... Indexes>
    struct tuple_seq_generator<0, Indexes...> {
        typedef tuple_seq_t<Indexes...> type;
    };


    template<class Old_Arg, class... New_Args>
    Old_Arg&& arg_get(Old_Arg&& old_arg, New_Args&& ...) const {
        return old_arg;
    }

    template<int N, class... New_Args>
    auto&& arg_get(std::_Placeholder<N>, New_Args&& ... new_args) const {
        return std::get<N - 1>(std::forward_as_tuple(new_args...));
    }

    template<class New_Function, class... Another_Args, class... New_Args>
    auto arg_get(my_bind_type<New_Function, Another_Args...>& another_bind, New_Args&& ... new_args) const {
        return another_bind(std::forward<New_Args>(new_args)...);
    }


    template<size_t... Indexes, class... New_Args>
    auto call(tuple_seq_t<Indexes...>, New_Args&& ... new_args) {
        return f(arg_get(std::get<Indexes>(tuple),
                         new_args...)...);
    }

};


template<class F, class... WAS>
my_bind_type<F, WAS...> my_bind(F&& f, WAS&& ... args) {
    return my_bind_type<F, WAS...>(std::forward<F>(f),
                                        std::tuple<WAS...>(std::forward<WAS>(args)...));
}

#endif //BIND_HWW