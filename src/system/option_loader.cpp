#include "option_loader.hpp"

#include <vector>
#include <memory>
#include <algorithm>

#include "msg_logger.hpp"
#include "i_params.hpp"

#include "autotrack_popup.hpp"
#include "display_cmd.hpp"
#include "dedicate_to_window.hpp"

using namespace std ;

struct OptionLoader::Impl
{
    vector<op::shp_t> vpop ;

    explicit Impl(const KeyBinder* const pkb)
    : vpop{
        AutotrackPopup::create(),
        DisplayCmd::create(pkb),
        Dedicate2Window::create()
    }{}

    ~Impl() {
        vpop.clear() ;
    }

    Impl(Impl&&) = delete ;
    Impl& operator=(Impl&&) = delete ;
    Impl(const Impl&) = delete ;
    Impl& operator=(const Impl&) = delete ;
} ;


OptionLoader::OptionLoader(const KeyBinder* const pkb)
: pimpl(make_unique<Impl>(pkb))
{}

OptionLoader::~OptionLoader() = default ;

void OptionLoader::load_config() noexcept
{
    //load options ------------------------------------------------
    //initialzie
    for_each(pimpl->vpop.cbegin(), pimpl->vpop.cend(), [](auto& op){op->disable() ;}) ;

    for(const auto& op : pimpl->vpop) {
        if(iParams::get_b(op->name())) op->enable() ;
    }
}

void OptionLoader::update() noexcept
{
    for(const auto& op : pimpl->vpop) {
        op->process() ;
    }
}