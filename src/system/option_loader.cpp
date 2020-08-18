#include "option_loader.hpp"

#include <vector>
#include <memory>
#include <algorithm>

#include "ini_parser.hpp"
#include "msg_logger.hpp"

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

void OptionLoader::load_config(const string& filename) noexcept
{
    //load options ------------------------------------------------
    //initialzie
    for_each(pimpl->vpop.cbegin(), pimpl->vpop.cend(), [](auto& op){op->disable() ;}) ;
    const auto pt = INIParser::load_config(filename) ;

    for(const auto& op : pimpl->vpop) {
        if(pt.get_optional<int>("Options." + op->name()).get() == 0) {
            continue ;
        }

        op->enable() ;
    }

    MESSAGE_STREAM << "Loaded " << filename << endl ;
}

void OptionLoader::update() noexcept
{
    for(const auto& op : pimpl->vpop) {
        op->process() ;
    }
}