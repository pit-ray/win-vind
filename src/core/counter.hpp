#ifndef _COUNTER_HPP
#define _COUNTER_HPP


namespace vind
{
    namespace core
    {
        class Counter {
        private:

        public:
            explicit Counter() ;
            virtual ~Counter() noexcept ;

            Counter operator+(Counter) ;
            Counter operator+=(Counter) ;
        } ;
    }
}

#endif
