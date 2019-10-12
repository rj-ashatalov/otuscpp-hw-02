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
IpPool filterInternal(const IpPool& pool, T first, Types... args)
{
    std::vector<T> filterList{first, args...};
    IpPool result;
    std::copy_if(pool.begin(), pool.end(), std::back_inserter(result), [&](const auto& ip)
    {
        return std::equal(filterList.begin(), filterList.end(), ip.begin());
    });
    return result;
}

template<class T, class... Types>
IpPool filterInternalAny(const IpPool& pool, T first, Types... args)
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
    return result;
}


template<class... Types>
void filter_any(const IpPool& pool, Types... args)
{
    print(filterInternalAny(pool, args...));
}

template<class... Types>
void filter(const IpPool& pool, Types... args)
{
    print(filterInternal(pool, 0, args...));
}

int main(int argc, char const *argv[])
{
    std::istream* input = &std::cin; // input is stdin by default
    if (argc > 1)
    {
        input = new std::ifstream(argv[1]);
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

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        filter(ip_pool, 1);
        // ip = filter(1)

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        filter(ip_pool, 46, 70);
        // ip = filter(46, 70)

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        filter_any(ip_pool, 46);
        // ip = filter_any(46)

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
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
