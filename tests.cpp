#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <string>

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

void filter_and_print(const IpPool& ip_pool, std::function<bool(const Ip&)> predicate, std::ostream& output)
{
    for (const auto& ip : ip_pool)
    {
        if (predicate(ip))
        {
            output << std::get<0>(ip) << '.'
                   << std::get<1>(ip) << '.'
                   << std::get<2>(ip) << '.'
                   << std::get<3>(ip) << '\n';
        }
    }
}

TEST(FilterTest, FirstByte1)
{
    IpPool pool = {
        {1, 29, 168, 152},
        {1, 1, 1, 1},
        {46, 70, 225, 39},
        {222, 173, 235, 246}
    };
    std::sort(pool.begin(), pool.end(), std::greater<>{});

    std::ostringstream out;
    
    filter_and_print(pool, [](const auto& ip) { return std::get<0>(ip) == 1; }, out);

    std::string expected =
        "1.29.168.152\n"
        "1.1.1.1\n";

    EXPECT_EQ(out.str(), expected);
}

TEST(FilterTest, Filter46_70)
{
    IpPool pool = {
        {46, 70, 225, 39},
        {46, 70, 147, 26},
        {46, 70, 29, 76},
        {1, 1, 1, 1}
    };
    std::sort(pool.begin(), pool.end(), std::greater<>{});

    std::ostringstream out;

    filter_and_print(pool, [](const auto& ip) {
        return std::get<0>(ip) == 46 && std::get<1>(ip) == 70;
        }, out);

    std::string expected =
        "46.70.225.39\n"
        "46.70.147.26\n"
        "46.70.29.76\n";

    EXPECT_EQ(out.str(), expected);
}

TEST(FilterTest, AnyByte46)
{
    IpPool pool = {
        {186, 204, 34, 46},
        {46, 70, 225, 39},
        {1, 1, 46, 1},
        {100, 100, 100, 100}
    };
    std::sort(pool.begin(), pool.end(), std::greater<>{});

    std::ostringstream out;

    filter_and_print(pool, [](const auto& ip) {
        return std::get<0>(ip) == 46 || std::get<1>(ip) == 46 ||
            std::get<2>(ip) == 46 || std::get<3>(ip) == 46;
        }, out);

    std::string expected =
        "186.204.34.46\n"
        "46.70.225.39\n"
        "1.1.46.1\n";

    EXPECT_EQ(out.str(), expected);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}