#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace Catch {

    typedef void (*test_fun_ptr_type)();

    class TestCaseInfo {
    public:
        test_fun_ptr_type test_fun_ptr;
        std::string desc, fun_name;
        int line_number;
    public:
        TestCaseInfo(test_fun_ptr_type _test_fun_ptr, std::string _desc,
                     std::string _fun_name, int _line_number):
                test_fun_ptr(_test_fun_ptr), desc(_desc),
                fun_name(_fun_name), line_number(_line_number)
            {}
    };

    class TestCaseRegister {
    public:
            TestCaseRegister(test_fun_ptr_type test_fun_ptr, std::string desc,
                             std::string fun_name, int line_number)
                {
                    get_test_case_infos().push_back( TestCaseInfo{test_fun_ptr, desc, fun_name, line_number} );
                }
            static std::vector<TestCaseInfo>& get_test_case_infos() {
                static std::vector<TestCaseInfo> test_case_infos;
                return test_case_infos;
            }
            static void run() {
                for(TestCaseInfo& tc_info : get_test_case_infos()) {
                    tc_info.test_fun_ptr();
                }
            }
    };

    class Session {
    public:
        static int& get_assertions_total() {
            static int assertions_total = 0;
            return assertions_total;
        }
        static int& get_assertions_passed() {
            static int assertions_passed = 0;
            return assertions_passed;
        }
        static int& get_assertions_failed() {
            static int assertions_failed = 0;
            return assertions_failed;
        }
        static int& get_new_count() {
            static int new_count = 0;
            return new_count;
        }
        static int& get_delete_count() {
            static int delete_count = 0;
            return delete_count;
        }
        static std::string& get_current_section() {
            static std::string current_section;
            return current_section;
        }
        static void run() {
            get_assertions_total() = 0;
            get_assertions_passed() = 0;
            get_assertions_failed() = 0;
            get_new_count() = 0;
            get_delete_count() = 0;
            get_current_section().clear();
            TestCaseRegister::run();
            if (get_assertions_total() == 0) {
                std::cerr << "Keine Tests durchgefuehrt" << std::endl;
            }
            else if (get_assertions_total() == get_assertions_passed()) {
                std::cerr << "Alle Tests erfolgreich ("
                << get_assertions_total() << " REQUIREs in "
                << TestCaseRegister::get_test_case_infos().size()
                << " Test Cases)" << std::endl;
            }
            else {
                std::cerr << "Tests Cases: "
                << TestCaseRegister::get_test_case_infos().size()
                << "\nREQUIREs: "
                << get_assertions_total() << " | "
                << get_assertions_passed() << " erfolgreich | "
                << get_assertions_failed() << " fehlgeschlagen\n";
            }
        }
    };

    class SectionGuard {
        std::string old_section;
    public:
        explicit SectionGuard(const std::string& new_section)
            : old_section(Session::get_current_section()) {
            Session::get_current_section() = new_section;
        }

        ~SectionGuard() {
            Session::get_current_section() = old_section;
        }

        explicit operator bool() const { return true; }
    };

    namespace Detail {
        template<typename T>
        class is_streamable {
            template<typename U>
            static auto test(int) -> decltype(std::declval<std::ostream&>() << std::declval<const U&>(), std::true_type());
            template<typename>
            static auto test(...) -> std::false_type;
        public:
            static constexpr bool value = decltype(test<T>(0))::value;
        };

        inline std::string to_string_value(const std::string& value) {
            return '"' + value + '"';
        }

        inline std::string to_string_value(const char* value) {
            return value ? ('"' + std::string(value) + '"') : std::string("<null>");
        }

        inline std::string to_string_value(char* value) {
            return value ? ('"' + std::string(value) + '"') : std::string("<null>");
        }

        inline std::string to_string_value(bool value) {
            return value ? "true" : "false";
        }

        template<typename T>
        inline typename std::enable_if<is_streamable<T>::value, std::string>::type
        to_string_value(const T& value) {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }

        template<typename T>
        inline typename std::enable_if<!is_streamable<T>::value, std::string>::type
        to_string_value(const T&) {
            return "<nicht-ausgebbarer-Wert>";
        }

        struct AssertionResult {
            bool passed;
            std::string reconstructed;

            explicit operator bool() const {
                return passed;
            }
        };

        template<typename L>
        struct ExprLhs {
            L lhs;

            explicit ExprLhs(L value) : lhs(value) {}

            template<typename R>
            AssertionResult operator==(const R& rhs) const {
                return AssertionResult{ lhs == rhs, to_string_value(lhs) + " == " + to_string_value(rhs) };
            }
            template<typename R>
            AssertionResult operator!=(const R& rhs) const {
                return AssertionResult{ lhs != rhs, to_string_value(lhs) + " != " + to_string_value(rhs) };
            }
            template<typename R>
            AssertionResult operator<(const R& rhs) const {
                return AssertionResult{ lhs < rhs, to_string_value(lhs) + " < " + to_string_value(rhs) };
            }
            template<typename R>
            AssertionResult operator<=(const R& rhs) const {
                return AssertionResult{ lhs <= rhs, to_string_value(lhs) + " <= " + to_string_value(rhs) };
            }
            template<typename R>
            AssertionResult operator>(const R& rhs) const {
                return AssertionResult{ lhs > rhs, to_string_value(lhs) + " > " + to_string_value(rhs) };
            }
            template<typename R>
            AssertionResult operator>=(const R& rhs) const {
                return AssertionResult{ lhs >= rhs, to_string_value(lhs) + " >= " + to_string_value(rhs) };
            }

            operator AssertionResult() const {
                return AssertionResult{ static_cast<bool>(lhs), to_string_value(lhs) };
            }
        };

        struct Decomposer {
            template<typename T>
            ExprLhs<const T&> operator<=(const T& lhs) const {
                return ExprLhs<const T&>(lhs);
            }
        };
    }

    inline void collect_ok(std::string /*expr*/, std::string /*actual*/, std::string /*fun_name*/, std::string /*filename*/, int /*line_number*/) {
        Session::get_assertions_passed() += 1;
        Session::get_assertions_total() += 1;
    }

    inline void collect_fail(std::string expr, std::string actual, std::string fun_name, std::string filename, int line_number) {
        Session::get_assertions_failed() += 1;
        Session::get_assertions_total() += 1;
        for(TestCaseInfo& tc_info : TestCaseRegister::get_test_case_infos()) {
            if (tc_info.fun_name == fun_name) {
                std::cerr
                    << "\n========================================\n"
                    << "Fehler in TestCase: " << tc_info.desc;
                if (!Session::get_current_section().empty()) {
                    std::cerr << "\nSection fehlgeschlagen: " << Session::get_current_section();
                }
                std::cerr << "\nZeile " << line_number
                          << " in Datei " << filename
                          << "\nErwartet: REQUIRE(" << expr << ");"
                          << "\nErhalten: REQUIRE(" << actual << ");"
                          << "\n========================================"
                          << std::endl;
                break;
            }
        }
    }

#if (defined(_MSC_VER) || __cplusplus < 201402L)
#define __func__ __FUNCTION__
#endif

    #define Q(x) #x
    #define QUOTE(x) Q(x)

#ifndef TEST_FILE
    #define UNIQUE_NAME3( name , line ) name##line
    #define UNIQUE_NAME2( name , line ) UNIQUE_NAME3( name ,line )
    #define UNIQUE_NAME( name ) UNIQUE_NAME2( name , __LINE__ )
#else
    #define UNIQUE_NAME3( name , file , line ) name##file##line
    #define UNIQUE_NAME2( name , file, line ) UNIQUE_NAME3( name , file , line )
    #define UNIQUE_NAME( name ) UNIQUE_NAME2( name , TEST_FILE, __LINE__ )
#endif

    #define TEST_CASE( desc, ... ) \
        void UNIQUE_NAME( test_function )(); \
        Catch::TestCaseRegister UNIQUE_NAME( regtc )( & UNIQUE_NAME( test_function ), desc , QUOTE( UNIQUE_NAME( test_function ) ) , __LINE__ ); \
        void UNIQUE_NAME( test_function )()

    #define SECTION( desc ) \
        if (Catch::SectionGuard UNIQUE_NAME(section_guard_){desc})

    #define REQUIRE( expr ) \
        do { \
            Catch::Detail::AssertionResult _catch_result = (Catch::Detail::Decomposer() <= expr); \
            if (_catch_result) { Catch::collect_ok(#expr, _catch_result.reconstructed, __FUNCTION__, __FILE__, __LINE__); } \
            else { Catch::collect_fail(#expr, _catch_result.reconstructed, __FUNCTION__, __FILE__, __LINE__); } \
        } while(false)
}
