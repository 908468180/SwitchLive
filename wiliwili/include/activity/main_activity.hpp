#pragma once

#include <borealis/core/activity.hpp>
#include <borealis/core/box.hpp>
#include <borealis/views/label.hpp>
#include "api/live_api.hpp"

namespace live {

class MainActivity : public brls::Activity {
public:
    CONTENT_FROM_XML_RES("activity/main.xml");

    void onContentAvailable() override;
    ~MainActivity() override;

private:
    void loadCategories(const std::string& platform);

    brls::Label* statusLabel = nullptr;
    brls::Label* titleLabel = nullptr;

    std::string current_platform_ = "bilibili";
};

} // namespace live
