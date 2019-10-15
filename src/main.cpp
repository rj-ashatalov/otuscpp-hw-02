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
        IpPool ipPool;
        for(std::string line; std::getline(*input, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            Ip ip;
            for(auto rawIpPart : split(v.at(0), '.'))
            {
                ip.push_back(std::stoi(rawIpPart));
            }
            ipPool.push_back(ip);
        }

        std::sort(ipPool.begin(), ipPool.end(), std::greater<Ip>());

        print(ipPool);
        print(filter(ipPool, 1));
        print(filter(ipPool, 46, 70));
        print(filter_any(ipPool, 46));
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