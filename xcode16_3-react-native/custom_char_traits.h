#ifndef custom_char_traits_h
#define custom_char_traits_h

#ifdef __cplusplus
#include <string>
#include <ios>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <type_traits>
#include <iterator>

// Forward declare our traits to help with specialization
namespace yourreactnativeapp_traits {
    template<typename CharT> struct char_traits;
}

namespace std {
    // Specialization for containers using our traits
    template<typename InputIt>
    bool all_of(InputIt first, InputIt last, int (*pred)(int),
        typename enable_if<
            is_same<
                typename iterator_traits<InputIt>::value_type::algorithm_category,
                true_type
            >::value
        >::type* = nullptr) 
    {
        while (first != last) {
            if (!pred(static_cast<int>(*first))) {
                return false;
            }
            ++first;
        }
        return true;
    }

    // Specialization for unsigned char sequences
    template<typename InputIt>
    typename enable_if<
        is_same<typename iterator_traits<InputIt>::value_type, unsigned char>::value,
        bool
    >::type
    all_of(InputIt first, InputIt last, int (*pred)(int)) {
        while (first != last) {
            if (!pred(static_cast<int>(*first))) {
                return false;
            }
            ++first;
        }
        return true;
    }
}

// Define our traits in a completely isolated namespace
namespace yourreactnativeapp_traits {
    // Algorithm specialization for unsigned char sequences
    template<typename InputIt>
    bool all_of(InputIt first, InputIt last, int (*pred)(int)) {
        while (first != last) {
            if (!pred(static_cast<int>(*first))) {
                return false;
            }
            ++first;
        }
        return true;
    }

    // Common implementation for unsigned char-like types
    template<typename CharT>
    struct char_traits {
        using char_type = CharT;
        using int_type = unsigned int;
        using off_type = std::streamoff;
        using pos_type = std::streampos;
        using state_type = std::mbstate_t;

        // Add a type alias to help with algorithm specialization
        using algorithm_category = std::true_type;

        static void assign(char_type& c1, const char_type& c2) noexcept {
            c1 = c2;
        }

        static bool eq(char_type c1, char_type c2) noexcept {
            return c1 == c2;
        }

        static bool lt(char_type c1, char_type c2) noexcept {
            return static_cast<unsigned char>(c1) < static_cast<unsigned char>(c2);
        }

        static int compare(const char_type* s1, const char_type* s2, size_t n) noexcept {
            return std::memcmp(s1, s2, n);
        }

        static size_t length(const char_type* s) noexcept {
            const char_type* end = s;
            while (*end) ++end;
            return end - s;
        }

        static const char_type* find(const char_type* s, size_t n, const char_type& a) noexcept {
            for (size_t i = 0; i < n; ++i) {
                if (eq(s[i], a)) return s + i;
            }
            return nullptr;
        }

        static char_type* move(char_type* s1, const char_type* s2, size_t n) noexcept {
            return static_cast<char_type*>(std::memmove(s1, s2, n));
        }

        static char_type* copy(char_type* s1, const char_type* s2, size_t n) noexcept {
            return static_cast<char_type*>(std::memcpy(s1, s2, n));
        }

        static char_type* assign(char_type* s, size_t n, char_type a) noexcept {
            for (size_t i = 0; i < n; ++i) s[i] = a;
            return s;
        }

        static int_type not_eof(int_type c) noexcept {
            return c != eof() ? c : 0;
        }

        static char_type to_char_type(int_type c) noexcept {
            return static_cast<char_type>(c);
        }

        static int_type to_int_type(char_type c) noexcept {
            return static_cast<int_type>(static_cast<unsigned char>(c));
        }

        static bool eq_int_type(int_type c1, int_type c2) noexcept {
            return c1 == c2;
        }

        static int_type eof() noexcept {
            return static_cast<int_type>(-1);
        }
    };
}

// Specialization for unsigned char
template<>
struct std::char_traits<unsigned char> : yourreactnativeapp_traits::char_traits<unsigned char> {};

#endif /* __cplusplus */
#endif /* custom_char_traits_h */ 
