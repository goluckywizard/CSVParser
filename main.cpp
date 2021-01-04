#include "tuple_it.h"
template <typename...Args> class CSVIterator {
public:
    std::vector<std::tuple<Args...> > &CSVData;
    int curTuple;
    CSVIterator(std::vector<std::tuple<Args...> > &a, int& b) : CSVData(a), curTuple(b) {}
    void operator++() {
        curTuple++;
    }
    bool operator==(CSVIterator operand) {
        return (curTuple == operand.curTuple);
    }
    bool operator!=(CSVIterator operand) {
        return (curTuple != operand.curTuple);
    }
    operator std::tuple<Args...> () {
        return CSVData[curTuple];
    }
    std::tuple<Args...> operator *() {
        return CSVData[curTuple];
    }
};

template <typename... Args> class CSVParser {
    std::vector<std::tuple<Args...> > CSVData;
    int curTuple = 0;
public:
    template <typename Char_T, typename Traits>
    CSVParser(std::basic_istream<Char_T, Traits> &input_file, int lineCount) {
        std::string buf;
        for (int i = 0; i < lineCount; ++i) {
            std::getline(input_file, buf);
        }
        const std::regex extractor("[^,]+");
        while (getline(input_file, buf))
        {
            std::vector<std::string> for_tuple;
            std::string new_word;
            auto newstr_begin = std::sregex_iterator(buf.begin(), buf.end(), extractor);
            for (auto i = newstr_begin; i != std::sregex_iterator(); ++i)
            {
                new_word = i -> str();
                for_tuple.push_back(new_word);
            }
            CSVData.push_back(parser::parse<Args...>(for_tuple));
            //std::cout << CSVData.pop_back();
        }
        //std::cout << CSVData.size();
        //std::cout << CSVData[1];
    }
    CSVIterator<Args...> begin() {
        CSVIterator<Args...> a(CSVData, curTuple);
        return a;
    }
    CSVIterator<Args...> end() {
        int count = CSVData.size();
        CSVIterator<Args...> a(CSVData, count);
        return a;
    }
    /*std::tuple<Args...> &operator++() {
        return CSVData[curTuple++];
    }*/
};


int main() {
    std::tuple<int, double, char, int> t = std::make_tuple(5, 2.4, 'a', 21);
    //std::cout << t;
    std::ifstream file("in.txt");
    CSVParser<int, double> parser(file, 0);
    for (std::tuple<int, double> rs : parser)
    {
        std::cout << rs << "\n";
    }
}