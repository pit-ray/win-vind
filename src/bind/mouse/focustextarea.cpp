#include "focustextarea.hpp"

#include "core/errlogger.hpp"
#include "core/settable.hpp"
#include "opt/uiacachebuild.hpp"
#include "util/box2d.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"
#include "util/uia.hpp"
#include "util/uiwalker.hpp"
#include "util/winwrap.hpp"

#include <limits>

#undef max


namespace
{
    using namespace vind ;

    struct TextAreaScanner : public util::UIWalker {
        explicit TextAreaScanner()
        : UIWalker{
            UIA_IsTextPatternAvailablePropertyId,
            UIA_IsValuePatternAvailablePropertyId,
            UIA_HasKeyboardFocusPropertyId,
            UIA_ValueIsReadOnlyPropertyId
        }
        {
            UIWalker::enable_fullcontrol() ;
        }

        virtual bool pinpoint_element(const util::SmartElement& elem) override {
            BOOL flag ;
            if(util::is_failed(elem->get_CachedHasKeyboardFocus(&flag))) {
                throw RUNTIME_EXCEPT("Could not get a cached property value: HasKeyboardFocus.") ;
            }
            return flag == TRUE ;
        }

        virtual bool filter_element(const util::SmartElement& elem) override {
            VARIANT val ;

            if(util::is_failed(elem->GetCachedPropertyValue(
                            UIA_ValueIsReadOnlyPropertyId, &val))) {
                throw RUNTIME_EXCEPT("Could not get a chached property value: IsReadOnly.") ;
            }
            if(val.vt != VT_BOOL) {
                return false ;
            }
            if(val.boolVal == VARIANT_TRUE) {
                return false ;
            }

            if(util::is_failed(elem->GetCachedPropertyValue(
                            UIA_IsTextPatternAvailablePropertyId, &val))) {
                throw RUNTIME_EXCEPT("Could not get a chached property value: IsTextPatternAvailable.") ;
            }
            if(val.vt != VT_BOOL) {
                return false ;
            }
            if(val.boolVal == VARIANT_FALSE) {
                return false ;
            }

            if(util::is_failed(elem->GetCachedPropertyValue(
                            UIA_IsValuePatternAvailablePropertyId, &val))) {
                throw RUNTIME_EXCEPT("Could not get a chached property value: IsValuePatternAvailable.") ;
            }
            if(val.vt != VT_BOOL) {
                return false ;
            }
            if(val.boolVal == VARIANT_FALSE) {
                return false ;
            }

            return true ;
        }
    } ;
}


namespace vind
{
    namespace bind
    {
        struct FocusTextArea::Impl {
            TextAreaScanner scanner_{} ;

            void focus_nearest_textarea(
                    HWND hwnd,
                    const util::Point2D& point) {

                std::vector<util::SmartElement> editables{} ;

                auto& settable = core::SetTable::get_instance() ;
                if(settable.get("uiacachebuild").get<bool>()) {
                    auto root_elem = opt::AsyncUIACacheBuilder::get_root_element(hwnd) ;
                    scanner_.scan(root_elem, editables) ;
                }
                else {
                    scanner_.scan(hwnd, editables) ;
                }

                if(editables.empty()) {
                    return ;
                }

                if(editables.size() == 1) {
                    if(util::is_failed(editables.front()->SetFocus())) {
                        throw RUNTIME_EXCEPT("SetFocus Failed.") ;
                    }
                    return ;
                }

                util::SmartElement nearest ;
                util::Box2D nearest_box ;
                auto min_distance = std::numeric_limits<double>::max() ;

                for(auto& elem : editables) {
                    // scan GUI objects only at leaves in tree.
                    util::Box2D box ;
                    if(util::is_failed(elem->get_CachedBoundingRectangle(&box.data()))) {
                        throw RUNTIME_EXCEPT("Could not get the a rectangle of a element.") ;
                    }

                    auto distance = util::l2_distance_nosq(
                            point.x(), point.y(),
                            box.center_x(), box.center_y()) / 100 ;

                    if(min_distance > distance) {
                        nearest = elem ;
                        min_distance = distance ;
                        nearest_box = box ;
                    }
                }

                if(util::is_failed(nearest->SetFocus())) {
                    throw RUNTIME_EXCEPT("SetFocus Failed.") ;
                }

                // Move the cursor into the text area.
                util::set_cursor_pos(nearest_box.center_x(), nearest_box.center_y()) ;
            }
        } ;

        FocusTextArea::FocusTextArea()
        : BindedFuncVoid("focus_textarea"),
          pimpl(std::make_unique<Impl>())
        {
            opt::AsyncUIACacheBuilder::register_properties(
                    pimpl->scanner_.get_properties()) ;
        }

        FocusTextArea::~FocusTextArea() noexcept = default ;
        FocusTextArea::FocusTextArea(FocusTextArea&&) = default ;
        FocusTextArea& FocusTextArea::operator=(FocusTextArea&&) = default ;

        void FocusTextArea::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            if(auto hwnd = util::get_foreground_window()) {
                auto pos = util::get_cursor_pos() ;
                pimpl->focus_nearest_textarea(hwnd, pos) ;
            }
        }
    }
}
