#ifndef BIND_H

#define BIND_H

#include <tuple>
#include <functional>
#include <iostream>


template <typename F, typename ...Args>
struct my_bind_t {

public:

    my_bind_t(F&& f, Args&&... args) :
            func(std::forward<F>(f)), args(std::forward<Args>(args)...)
    {}

    template <typename ... New_args>
    auto operator() (New_args&& ...new_args) {
        return call_f(typename seq_generator<std::tuple_size<std::tuple<Args...>>::value>::type(), std::forward<New_args>(new_args)...);
    }

private:

    std::tuple<typename std::decay<Args>::type...> args;
    typename std::decay<F>::type func;

    template <int... sequence>
    struct seq_generator_t {};

    template <int first, int ...other>
    struct seq_generator {
        typedef typename seq_generator<first - 1, first - 1, other...>::type type;
    };

    template <int ...other>
    struct seq_generator<0, other...> {
        typedef seq_generator_t<other...> type;
    };

    template <typename That_arg, typename ...New_args>
    auto&& arg_get(That_arg& that_arg, New_args& ...new_args) {
        return that_arg;
    }

    template <int N, typename ...New_args>
    auto&& arg_get(std::_Placeholder<N> plh, New_args& ...new_args) {
        return std::get<N - 1>(std::forward_as_tuple(new_args...));
    }

    template <typename Other_func, typename ...Other_args, typename ...New_args>
    auto arg_get(my_bind_t<Other_func, Other_args...> & other_bind, New_args& ...new_args) {
        return other_bind(new_args...);
    }

    template <int ...N, typename ...New_args>
    auto call_f(const seq_generator_t<N...>& sequence, New_args&& ...new_args) {
        return func(arg_get(std::get<N>(args), new_args...)...);
    }

};

template <typename F, typename ...Args>
my_bind_t<F, Args...> my_bind(F&& f, Args&&... args) {
    return my_bind_t<F, Args...>(std::forward<F>(f), std::forward<Args>(args)...);
}

#endif //BIND_HWW