#pragma once

#include <borealis.hpp>
#include <borealis/core/bind.hpp>

#include "api/live_api.hpp"

namespace live {

class MainActivity : public brls::Activity {
public:
    CONTENT_FROM_XML_RES("activity/main.xml");

    void onContentAvailable() override;
    ~MainActivity() override;

private:
    void loadCategories(const std::string& platform);

    BRLS_BIND(brls::Label, statusLabel, "main/status");
    BRLS_BIND(brls::Label, titleLabel, "main/title");

    std::string currentPlatform = "bilibili";
};

} // namespace live
