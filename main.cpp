#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <fstream>

using Ip = std::tuple<int, int, int, int>;
using IpPool = std::vector<Ip>;

std::vector<std::string> split(const std::string& str, char d)
{
    std::vector<std::string> r;
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }
    r.push_back(str.substr(start));
    return r;
}

int main()
{
    try
    {
        IpPool ip_pool;

        std::ifstream file("../ip_filter.tsv");
        if (!file.is_open()) {
            return 1;
        }

        for (std::string line; std::getline(file, line);)
        {
            auto v = split(line, '\t');
            auto ip_parts = split(v.at(0), '.');
            assert(ip_parts.size() == 4);

            ip_pool.emplace_back(
                std::stoi(ip_parts[0]),
                std::stoi(ip_parts[1]),
                std::stoi(ip_parts[2]),
                std::stoi(ip_parts[3])
            );
        }

        std::sort(ip_pool.begin(), ip_pool.end(), std::greater<>{});

        auto print_ip = [](const Ip& ip)
            {
                std::cout << std::get<0>(ip) << '.'
                    << std::get<1>(ip) << '.'
                    << std::get<2>(ip) << '.'
                    << std::get<3>(ip) << '\n';
            };

        auto filter_and_print = [&](auto predicate)
            {
                for (const auto& ip : ip_pool)
                {
                    if (predicate(ip))
                    {
                        print_ip(ip);
                    }
                }
            };

        filter_and_print([](const auto&) { return true; });

        filter_and_print([](const auto& ip) { return std::get<0>(ip) == 1; });

        filter_and_print([](const auto& ip)
            {
                return std::get<0>(ip) == 46 && std::get<1>(ip) == 70;
            });

        filter_and_print([](const auto& ip)
            {
                return std::get<0>(ip) == 46 ||
                    std::get<1>(ip) == 46 ||
                    std::get<2>(ip) == 46 ||
                    std::get<3>(ip) == 46;
            });
    }
    catch (const std::exception& e)
    {
        return 1;
    }

    return 0;
}