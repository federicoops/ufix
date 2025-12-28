#include "ufix.h"
#include <print>
#include <ranges>

int main() {
    std::string             buf{"8=FIXT1.1|35=A|60=1921042801|10=234"};
    ufix::view::MessageView msg{buf};

    std::println("range_for -------------------------------");
    for (auto fv : msg) {
        std::println("{}", fv);
    }

    std::println("\nas_map ----------------------------------");
    auto fm = msg.as_map();
    for (auto [k, v] : fm.kvs()) {
        std::println("{}={}", k, v);
    }

    std::println("\nas_generator ----------------------------");
    for (auto fv : msg.as_generator()) {
        std::println("{}", fv);
    }

    std::println("\npop ----------------------------");
    std::println("pop from msg:  {}", msg);
    std::string_view pop;
    while (pop = msg.pop(), pop != "") {
        std::println("pop: {}", ufix::view::FieldView(pop));
    }
    std::println("after pop msg: {}", msg);
}