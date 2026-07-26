#pragma once

#include <borealis.hpp>
#include <borealis/core/bind.hpp>

#include "api/live_api.hpp"

namespace live {

class LiveListActivity : public brls::Activity {
public:
    CONTENT_FROM_XML_RES("fragment/live_list.xml");

    void onContentAvailable() override;
    ~LiveListActivity() override;

    void setPlatform(const std::string& platform);
    void setCategoryId(const std::string& id, const std::string& name);

private:
    void loadRooms();

    BRLS_BIND(brls::Label, titleLabel, "live/title");
    BRLS_BIND(brls::Label, statusLabel, "live/status");

    std::string platform;
    std::string categoryId;
    std::string categoryName;
};

} // namespace live
