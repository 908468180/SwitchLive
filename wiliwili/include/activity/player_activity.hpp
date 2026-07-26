#pragma once

#include <borealis.hpp>
#include <borealis/core/bind.hpp>

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

    LiveRoomItem room;
    LiveRoomDetail detail;
    std::vector<LivePlayQuality> qualities;

    BRLS_BIND(brls::Label, titleLabel, "player/title");
    BRLS_BIND(brls::Label, streamerLabel, "player/streamer");
    BRLS_BIND(brls::Label, statusLabel, "player/status");
};

} // namespace live
