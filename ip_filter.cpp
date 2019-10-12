#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>

using Ip = std::vector<int>;
using IpPool = std::vector<Ip>;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        auto&& result = str.substr(start, stop - start);
        r.push_back(result);

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

void print(const IpPool& pool)
{
    for(const auto& ip : pool)
    {
        std::stringstream output;
        std::copy(ip.begin(), std::prev(ip.end()),std::ostream_iterator<int>(output, "."));
        output << ip.back();
        std::cout << output.str() << std::endl;
    }
}

template<class T, class... Types>
void filter(const IpPool& pool, T first, Types... args)
{
    std::vector<T> filterList{first, args...};
    IpPool result;
    std::copy_if(pool.begin(), pool.end(), std::back_inserter(result), [&](const auto& ip)
    {
        return std::equal(filterList.begin(), filterList.end(), ip.begin());
    });
    print(result);
}

template<class T, class... Types>
void filter_any(const IpPool& pool, T first, Types... args)
{
    std::vector<T> filterList{first, args...};
    IpPool result;
    std::copy_if(pool.begin(), pool.end(), std::back_inserter(result), [&](const auto& ip)
    {
        return std::any_of(filterList.begin(), filterList.end(), [&](const auto& filterKey)
        {
            return std::find(ip.begin(), ip.end(), filterKey) != ip.end();
        });
    });
    print(result);
}

int main(int argc, char const *argv[])
{
    std::istream* input = &std::cin;
    if (argc > 1)
    {
        auto inputDataPath = argv[1];
        input = new std::ifstream(inputDataPath);
    }

    try
    {
        IpPool ip_pool;
        for(std::string line; std::getline(*input, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            Ip ip;
            for(auto rawIpPart : split(v.at(0), '.'))
            {
                ip.push_back(std::stoi(rawIpPart));
            }
            ip_pool.push_back(ip);
        }

        std::sort(ip_pool.begin(), ip_pool.end(), std::greater<Ip>());

        print(ip_pool);

        filter(ip_pool, 1);
        filter(ip_pool, 46, 70);
        filter_any(ip_pool, 46);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    if (argc > 1)
    {
        delete input;
    }
    return 0;
}
