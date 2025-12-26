#include <cassert>
#include <map>
#include <ranges>
#include <string>
#include <string_view>

namespace ufix {

    constexpr inline char SEP   = '=';
    constexpr inline char DELIM = '|';

    struct FieldIterator {
        using difference_type = std::ptrdiff_t;
        using value_type      = std::string_view;

      private:
        std::size_t      cursor{0};
        std::string_view view;

      public:
        FieldIterator() = default;
        FieldIterator(std::size_t pos, std::string_view msg) : cursor(pos), view(msg) {}

        FieldIterator& operator++(int) { return ++*this; }
        FieldIterator& operator++() {
            cursor = view.find(DELIM, cursor);
            if (cursor != std::string::npos) {
                cursor += 1;
            }
            return *this;
        }

        std::string_view operator*() const {
            auto next = view.find(ufix::DELIM, cursor);
            if (next == std::string::npos) {
                return std::string_view(view.begin() + cursor, view.end());
            }
            return std::string_view(view.begin() + cursor, view.begin() + next);
        }

        bool operator==(const FieldIterator& other) const { return other.view.begin() == view.begin() && other.cursor == cursor; }
    };

    struct FieldView {
        std::string_view tag{};
        std::string_view value{};

        FieldView(std::string_view tag, std::string_view value) : tag(tag), value(value) {}
        FieldView(std::string& tag, std::string& value) : tag(tag), value(value) {}
        FieldView(FieldIterator& it) {
            auto view  = *it;
            auto parts = view | std::views::split(ufix::SEP);
            auto pit   = parts.begin();
            if (pit == parts.end())
                return;
            tag = std::string_view{*pit};
            if (++pit == parts.end())
                return;
            value = std::string_view{*pit};
        }
    };

    struct FieldMap {
      private:
        std::map<std::string_view, std::string_view> tv_map;

      public:
        FieldMap(FieldIterator begin, FieldIterator end) {
            for (auto it = begin; it != end; ++it) {
                auto fv = FieldView{it};
                tv_map.insert({fv.tag, fv.value});
            }
        }

        auto kvs() { return tv_map; }
    };

    struct Message {
      private:
        std::string_view raw_message;

      public:
        Message(std::string& str) : raw_message(str) {
            if (raw_message.ends_with(ufix::DELIM))
                raw_message.remove_suffix(1);
        }

        FieldIterator begin() const { return FieldIterator{0, raw_message}; }
        FieldIterator end() const { return FieldIterator{std::string::npos, raw_message}; }
        FieldMap      fields() const { return FieldMap{begin(), end()}; }
    };

} // namespace ufix
