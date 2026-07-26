#include "activity/player_activity.hpp"

namespace live {

PlayerActivity::~PlayerActivity() {
    brls::Logger::debug("del PlayerActivity");
}

void PlayerActivity::onContentAvailable() {
    titleLabel->setText(room.title);
    streamerLabel->setText(room.nick);
    statusLabel->setText("Loading...");

    loadPlayUrl();
}

void PlayerActivity::setRoom(const LiveRoomItem& room) {
    this->room = room;
}

void PlayerActivity::loadPlayUrl() {
    if (room.platformId == "bilibili") {
        auto& api = LiveAPI::getInstance();

        api.getBiliRoomDetail(room.roomId,
            [this](const LiveRoomDetail& detail) {
                this->detail = detail;
                this->qualities = {
                    {"Original", "original"},
                    {"Super",    "super"},
                    {"High",     "high"},
                    {"Standard", "standard"}
                };

                LiveAPI::getInstance().getBiliPlayUrl(
                    detail.roomId, this->qualities[0].qualityType,
                    [this](const LivePlayUrl& url) {
                        brls::Threading::sync([this, url]() {
                            statusLabel->setText("Playing");
                            startPlayback(url.url);
                        });
                    },
                    [this](const std::string& error, int code) {
                        brls::Threading::sync([this, error]() {
                            statusLabel->setText("Error: " + error);
                        });
                    });
            },
            [this](const std::string& error, int code) {
                brls::Threading::sync([this, error]() {
                    statusLabel->setText("Error: " + error);
                });
            });
    } else {
        statusLabel->setText("Platform not yet supported");
    }
}

void PlayerActivity::startPlayback(const std::string& url) {
    if (url.empty()) {
        statusLabel->setText("No play URL");
        return;
    }

    brls::Logger::info("Playing: {}", url);
}

} // namespace live
