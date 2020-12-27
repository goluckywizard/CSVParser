#include <iostream>
#include <fstream>
#include <tuple>
//using namespace std;
//namespace tup {
    /*template <typename... Args> struct tuple;
template <typename Head, typename... Tail> struct tuple <Head, Tail...> : tuple<Tail...> {
    tuple(Head h, Tail... tail): tuple<Tail...>(tail...), head_(h){};
    typedef tuple<Tail...> base_type;
    typedef Head value_type;
    base_type& base = static_cast<base_type&>(*this);
    Head head_;
};
template <> struct tuple<>{};
*/
    /*template<int I, typename Head, typename... Args>
    struct getter {
        typedef typename getter<I - 1, Args...>::return_type return_type;
        static return_type get(std::tuple<Head, Args...> t) {
            return getter<I - 1, Args...>::get(t);
        }
    };

    template<typename Head, typename ...Args>
    struct getter <0, Head, Args...> {
        typedef typename Head return_type;

        static return_type get(std::tuple<Head, Args...> t) {
            return std::get<0>(t);
        }
    };

    template<int I, typename Head, typename... Args>
    typename getter<I, Head, Args...>::return_type
    get(std::tuple<Head, Args...> t) {
        return getter<I, Head, Args...>::get(t);
    }*/
    template <int Index, typename Callback, typename ... Args>
    struct iterator {
        static void next(std::tuple<Args...> &t, Callback callback, std::ostream &stream) {
            iterator<Index-1, Callback, Args...>::next(t, callback, stream);
            callback(Index, std::get<Index>(t), stream);
        }
    };
template <typename Callback, typename ... Args>
    struct iterator<0, Callback, Args...> {
        static void next(std::tuple<Args...> &t, Callback callback, std::ostream &stream) {
            callback(0, std::get<0>(t), stream);
        }
    };
struct callback {
    template<typename T> void operator()(int index, T &&t, std::ostream &stream)
    {
        stream << t << " ";
    }
};

template<typename Callback, typename... Args>
void forEach(std::tuple<Args...> &t, Callback callback, std::ostream &stream)
{
    int const t_size = std::tuple_size<std::tuple<Args...>>::value;
    iterator<t_size - 1, Callback, Args...>::next(t, callback, stream);
}

template<typename _CharT, typename _Traits, typename... Args>
    inline std::basic_ostream<_CharT, _Traits> &
    operator<<(std::basic_ostream<_CharT, _Traits> &stream, std::tuple<Args...> &t) {
        forEach(t, callback(), stream);
        return stream;
    }

int main() {
    std::tuple<int, double, char> t = std::make_tuple(5, 2.4, 'a');
    /*std::cout << get<0>(t) << get<1>(t) << get<2>(t);*/
    std::cout << t;
    /*ifstream file("test.csv");
    CSVParser<int, string> parser(file, 0 );
    for (tuple<int, string> rs : parser) {
        cout << rs << endl;
    }*/
}