#pragma once

#include <borealis/core/activity.hpp>
#include <borealis/views/label.hpp>
#include "api/live_api.hpp"

namespace live {

class PlayerActivity : public brls::Activity {
public:
    CONTENT_FROM_XML_RES("activity/player.xml");

    void onContentAvailable() override;
    ~PlayerActivity() override;

    void setRoom(const LiveRoomItem& room);

private:
    void loadPlayUrl();
    void startPlayback(const std::string& url);

    LiveRoomItem room_;
    LiveRoomDetail detail_;
    std::vector<LivePlayQuality> qualities_;

    brls::Label* titleLabel = nullptr;
    brls::Label* streamerLabel = nullptr;
    brls::Label* statusLabel = nullptr;
};

} // namespace live
