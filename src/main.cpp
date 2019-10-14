#include "ip_filter.h"

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

        print(filter(ip_pool, 1));
        print(filter(ip_pool, 46, 70));
        print(filter_any(ip_pool, 46));
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