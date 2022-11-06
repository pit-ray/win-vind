#ifndef _MOTIONIDS_HPP
#define _MOTIONIDS_HPP

#include <unordered_set>

namespace vind
{
    namespace bind
    {
        class MotionIds {
        private:
            std::unordered_set<std::size_t> ids_ ;

            explicit MotionIds()
            : ids_{}
            {}
            virtual ~MotionIds() noexcept = default ;

        public:
            static MotionIds& get_instance() {
                static MotionIds instance ;
                return instance ;
            }

            void register_id(std::size_t id) {
                ids_.insert(id) ;
            }

            bool is_motion(std::size_t id) const {
                return ids_.find(id) != ids_.end() ;
            }
        } ;
    }
}

#endif
