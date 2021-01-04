#ifndef CSVPARSER_TUPLE_IT_H
#define CSVPARSER_TUPLE_IT_H
#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <regex>
#include <sstream>
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
inline std::basic_ostream<_CharT, _Traits>& operator<<(std::basic_ostream<_CharT, _Traits> &stream, std::tuple<Args...> &t) {
    forEach(t, callback(), stream);
    return stream;
}

namespace parser {
        template <typename T> class ToType {
        public:
            T value;
            ToType(std::string &stream) {
                std::stringstream buf;
                buf << stream;
                buf >> value;
            }
        };
        template <int Index, typename Callback, typename ... Args>
        struct iterator {
            static void next(std::tuple<Args...> &t, Callback callback, std::vector<std::string> &stream) {
                iterator<Index-1, Callback, Args...>::next(t, callback, stream);
                callback(Index, std::get<Index>(t), stream);
            }
        };
        template <typename Callback, typename ... Args>
        struct iterator<0, Callback, Args...> {
            static void next(std::tuple<Args...> &t, Callback callback, std::vector<std::string> &stream) {
                callback(0, std::get<0>(t), stream);
            }
        };
        struct callback {
            template<typename T> void operator()(int index, T &t, std::vector<std::string> &stream)
            {
                ToType<T> a(stream[index]);
                t = a.value;
            }
        };
        template<typename Callback, typename... Args>
        void forEach(std::tuple<Args...> &t, Callback callback, std::vector<std::string> &stream)
        {
            int const t_size = std::tuple_size<std::tuple<Args...>>::value;
            iterator<t_size - 1, Callback, Args...>::next(t, callback, stream);
        }
        template<typename... Args>
        inline std::tuple<Args...> parse(std::vector<std::string> &stream) {
            std::tuple<Args...> newTuple;
            forEach(newTuple, callback(), stream);
            return newTuple;
        }
    }

#endif //CSVPARSER_TUPLE_IT_H
