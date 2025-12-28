#include "ufix.h"
#include <print>
#include <ranges>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("test pop") {
    std::string             buf{"8=FIXT1.1|35=A|60=1921042801|10=234"};
    ufix::view::MessageView msg{buf};
    CHECK_EQ(msg.pop(), "8=FIXT1.1");
    CHECK_EQ(msg.pop(), "35=A");
    CHECK_EQ(msg.pop(), "60=1921042801");
    CHECK_EQ(msg.pop(), "10=234");
    CHECK_EQ(msg.pop(), "");
}

TEST_CASE("test range_for") {
    std::string             buf{"8=FIXT1.1|35=A|60=1921042801|10=234"};
    ufix::view::MessageView msg{buf};

    for (auto fv : msg) {
        CHECK_NE(fv, "");
    }
}

TEST_CASE("test as_map") {
    std::string             buf{"8=FIXT1.1|35=A|60=1921042801|10=234"};
    ufix::view::MessageView msg{buf};

    for (auto v : msg.as_map().kvs() | std::views::values) {
        CHECK_NE(v, "");
    }
}

TEST_CASE("test as_generator") {
    std::string             buf{"8=FIXT1.1|35=A|60=1921042801|10=234"};
    ufix::view::MessageView msg{buf};

    for (auto fv : msg.as_generator()) {
        CHECK_NE(fv.tag, "");
        CHECK_NE(fv.value, "");
    }
}
