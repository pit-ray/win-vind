#ifndef _SETTABLE_HPP
#define _SETTABLE_HPP

#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <variant>
#include <vector>


namespace vind
{
    namespace core
    {
        class Param ;

        class SetTable {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            explicit SetTable() ;
            virtual ~SetTable() noexcept ;

            static std::vector<Param> default_params ;

        public:
            static SetTable& get_instance() ;

            void clear() ;

            void set(const std::string& name, bool val) ;
            void set(const std::string& name, long val) ;
            void set(const std::string& name, float val) ;
            void set(const std::string& name, const std::string& val) ;
            void set(const std::string& name, std::string&& val) ;

            template <typename T>
            inline void set(const std::string& name, T val) {
                set(name, static_cast<long>(val)) ;
            }
            inline void set(const std::string& name, double val) {
                set(name, static_cast<float>(val)) ;
            }

            inline void set(const std::string& name, const char* val) {
                set(name, std::string(val)) ;
            }

            void set(const Param& param) ;
            void set(Param&& param) ;

            const Param& get(const std::string& name) const ;
            bool has(const std::string& name) const ;

            void remove(const std::string& name) ;
        } ;


        class Param {
            std::string name_ ;
            std::variant<
                bool,
                int,
                long,
                float,
                std::string> val_ ;

            template <typename ReturnType>
            class Visitor {
            private:
                const std::string& name_ ;

            public:
                Visitor(const std::string& name)
                : name_(name)
                {}

                ReturnType operator()(bool v) {
                    return static_cast<ReturnType>(v) ;
                }
                ReturnType operator()(int v) {
                    return static_cast<ReturnType>(v) ;
                }
                ReturnType operator()(long v) {
                    return static_cast<ReturnType>(v) ;
                }
                ReturnType operator()(float v) {
                    return static_cast<ReturnType>(v) ;
                }
                ReturnType operator()(const std::string&) {
                    throw std::invalid_argument(
                        "Not supported conversion from std::string to typeid(" +
                        std::string(typeid(ReturnType).name()) +
                        ") in " + name_ + ".") ;
                    return 0 ;
                }
            } ;

        public:
            Param()
            : name_(),
              val_()
            {}

            template <typename ParamName>
            Param(ParamName&& name, const char* value)
            : name_(std::forward<ParamName>(name)),
              val_(std::string(value))
            {}

            template <typename ParamName, typename ValueType>
            Param(ParamName&& name, ValueType&& value)
            : name_(std::forward<ParamName>(name)),
              val_(std::forward<ValueType>(value))
            {}

            const std::string& name() const noexcept {
                return name_ ;
            }

            bool is_bool() const noexcept {
                return std::holds_alternative<bool>(val_) ;
            }

            bool is_string() const noexcept {
                return std::holds_alternative<std::string>(val_) ;
            }

            bool is_number() const noexcept {
                return !is_bool() && !is_string() ;
            }

            template <typename T>
            T get() const {
                return std::visit(Visitor<T>{name_}, val_) ;
            }
        } ;

        // std::string -> bool
        template <>
        bool Param::Visitor<bool>::operator()(const std::string& v) {
            return !v.empty() ;
        }

        // std::string -> char
        template <>
        char Param::Visitor<char>::operator()(const std::string& v) {
            return static_cast<unsigned char>(std::stoi(v)) ;
        }

        // std::string -> unsigned char
        template <>
        unsigned char Param::Visitor<unsigned char>::operator()(const std::string& v) {
            return static_cast<unsigned char>(std::stoi(v)) ;
        }

        // std::string -> int
        template <>
        int Param::Visitor<int>::operator()(const std::string& v) {
            return std::stoi(v) ;
        }

        // std::string -> unsigned int
        template <>
        unsigned int Param::Visitor<unsigned int>::operator()(const std::string& v) {
            return static_cast<unsigned int>(std::stoll(v)) ;
        }

        // std::string -> short
        template <>
        short Param::Visitor<short>::operator()(const std::string& v) {
            return static_cast<short>(std::stoi(v)) ;
        }

        // std::string -> unsigned short
        template <>
        unsigned short Param::Visitor<unsigned short>::operator()(const std::string& v) {
            return static_cast<unsigned short>(std::stoi(v)) ;
        }

        // std::string -> long
        template <>
        long Param::Visitor<long>::operator()(const std::string& v) {
            return std::stol(v) ;
        }

        // std::string -> long long
        template <>
        long long Param::Visitor<long long>::operator()(const std::string& v) {
            return std::stoll(v) ;
        }

        // std::string -> unsigned long
        template <>
        unsigned long Param::Visitor<unsigned long>::operator()(const std::string& v) {
            return std::stoul(v) ;
        }

        // std::string -> unsigned long long
        template <>
        unsigned long long Param::Visitor<unsigned long long>::operator()(const std::string& v) {
            return std::stoull(v) ;
        }

        // std::string -> float
        template <>
        float Param::Visitor<float>::operator()(const std::string& v) {
            return std::stof(v) ;
        }

        // std::string -> double
        template <>
        double Param::Visitor<double>::operator()(const std::string& v) {
            return std::stod(v) ;
        }

        // std::string -> long double
        template <>
        long double Param::Visitor<long double>::operator()(const std::string& v) {
            return std::stold(v) ;
        }


        // bool -> std::string
        template <>
        std::string Param::Visitor<std::string>::operator()(bool v) {
            return v ? "1" : "0" ;
        }

        // int -> std::string
        template <>
        std::string Param::Visitor<std::string>::operator()(int v) {
            return std::to_string(v) ;
        }

        // long -> std::string
        template <>
        std::string Param::Visitor<std::string>::operator()(long v) {
            return std::to_string(v) ;
        }

        // float -> std::string
        template <>
        std::string Param::Visitor<std::string>::operator()(float v) {
            return std::to_string(v) ;
        }

        // std::string -> std::string
        template <>
        std::string Param::Visitor<std::string>::operator()(const std::string& v) {
            return v ;
        }
    }
}

#endif
