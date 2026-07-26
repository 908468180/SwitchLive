#include "fragment/live_list.hpp"

namespace live {

LiveListActivity::~LiveListActivity() {
    brls::Logger::debug("del LiveListActivity");
}

void LiveListActivity::onContentAvailable() {
    titleLabel  = dynamic_cast<brls::Label*>(this->getView("live/title"));
    statusLabel = dynamic_cast<brls::Label*>(this->getView("live/status"));

    if (titleLabel) titleLabel->setText(categoryName_);

    if (statusLabel) statusLabel->setText("Loading rooms...");

    loadRooms();
}

void LiveListActivity::setPlatform(const std::string& platform) {
    platform_ = platform;
}

void LiveListActivity::setCategoryId(const std::string& id, const std::string& name) {
    categoryId_ = id;
    categoryName_ = name;
}

void LiveListActivity::loadRooms() {
    auto& api = LiveAPI::getInstance();

    auto onSuccess = [this](const LiveCategoryResult& result) {
        brls::Threading::sync([this, result]() {
            if (statusLabel) {
                statusLabel->setText(std::to_string(result.items.size()) + " rooms loaded");
            }
            brls::Logger::info("Loaded {} rooms", result.items.size());
        });
    };

    auto onError = [this](const std::string& error, int code) {
        brls::Threading::sync([this, error]() {
            if (statusLabel) statusLabel->setText("Error: " + error);
        });
    };

    if (platform_ == "bilibili") {
        api.getBiliRoomList(categoryId_, 1, onSuccess, onError);
    } else if (platform_ == "douyu") {
        api.getDouyuRoomList(categoryId_, 1, onSuccess, onError);
    } else if (platform_ == "huya") {
        api.getHuyaRoomList(categoryId_, 1, onSuccess, onError);
    } else if (platform_ == "douyin") {
        api.getDouyinRoomList(categoryId_, 1, onSuccess, onError);
    }
}

} // namespace live
