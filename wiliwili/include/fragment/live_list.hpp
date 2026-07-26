#pragma once

#include <borealis.hpp>
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

    brls::Label* titleLabel = nullptr;
    brls::Label* statusLabel = nullptr;

    std::string platform_;
    std::string categoryId_;
    std::string categoryName_;
};

} // namespace live
