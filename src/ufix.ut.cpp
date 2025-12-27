#include "ufix.h"
#include <print>

int main() {
    std::string             buf{"8=FIXT1.1|35=A|60=1921042801|10=234"};
    ufix::view::MessageView msg{buf};

    auto fm = msg.as_map();
    for (auto [k, v] : fm.kvs()) {
        std::println("{}={}", k, v);
    }
}