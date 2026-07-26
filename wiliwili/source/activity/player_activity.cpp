#include "activity/player_activity.hpp"

namespace live {

PlayerActivity::~PlayerActivity() {
    brls::Logger::debug("del PlayerActivity");
}

void PlayerActivity::onContentAvailable() {
    titleLabel    = dynamic_cast<brls::Label*>(this->getView("player/title"));
    streamerLabel = dynamic_cast<brls::Label*>(this->getView("player/streamer"));
    statusLabel   = dynamic_cast<brls::Label*>(this->getView("player/status"));

    if (titleLabel)    titleLabel->setText(room_.title);
    if (streamerLabel) streamerLabel->setText(room_.nick);
    if (statusLabel)   statusLabel->setText("Loading...");

    loadPlayUrl();
}

void PlayerActivity::setRoom(const LiveRoomItem& room) {
    room_ = room;
}

void PlayerActivity::loadPlayUrl() {
    auto& api = LiveAPI::getInstance();

    if (room_.platform_id == "bilibili") {
        api.getBiliRoomDetail(room_.room_id,
            [&api, this](const LiveRoomDetail& detail) {
                detail_ = detail;
                qualities_ = {
                    {"Original", "original"},
                    {"Super",    "super"},
                    {"High",     "high"},
                    {"Standard", "standard"}
                };

                api.getBiliPlayUrl(detail.room_id, qualities_[0].quality_type,
                    [this](const LivePlayUrl& url) {
                        brls::Threading::sync([this, url]() {
                            if (statusLabel) statusLabel->setText("Playing");
                            startPlayback(url.url);
                        });
                    },
                    [this](const std::string& error, int code) {
                        brls::Threading::sync([this, error]() {
                            if (statusLabel) statusLabel->setText("Error: " + error);
                        });
                    }
                );
            },
            [this](const std::string& error, int code) {
                brls::Threading::sync([this, error]() {
                    if (statusLabel) statusLabel->setText("Error: " + error);
                });
            }
        );
    } else {
        if (statusLabel) statusLabel->setText("Platform not yet supported");
    }
}

void PlayerActivity::startPlayback(const std::string& url) {
    if (url.empty()) {
        if (statusLabel) statusLabel->setText("No play URL");
        return;
    }

    brls::Logger::info("Playing: {}", url);
}

} // namespace live
