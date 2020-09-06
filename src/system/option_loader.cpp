#include "option_loader.hpp"

#include <vector>
#include <memory>
#include <algorithm>

#include "msg_logger.hpp"
#include "i_params.hpp"

#include "autotrack_popup.hpp"
#include "dedicate_to_window.hpp"
#include "virtual_cmd_line.hpp"

using namespace std ;

struct OptionLoader::Impl
{
    vector<op::shp_t> vpop ;

    explicit Impl()
    : vpop{
        AutotrackPopup::create(),
        VirtualCmdLine::create(),
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


OptionLoader::OptionLoader()
: pimpl(make_unique<Impl>())
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