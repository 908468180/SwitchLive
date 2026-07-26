#include "activity/player_activity.hpp"
#include <borealis/core/application.hpp>
#include <borealis/core/thread.hpp>

namespace live {

PlayerActivity::~PlayerActivity() {
    brls::Logger::debug("del PlayerActivity");
}

void PlayerActivity::onContentAvailable() {
    titleLabel     = this->getViewById<brls::Label>("player/title");
    streamerLabel  = this->getViewById<brls::Label>("player/streamer");
    statusLabel    = this->getViewById<brls::Label>("player/status");

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
            [this](const LiveRoomDetail& detail) {
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

#ifdef __SWITCH__
    // TODO: Initialize mpv and play
#endif
}

} // namespace live
