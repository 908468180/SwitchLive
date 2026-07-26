#include "fragment/live_list.hpp"

namespace live {

LiveListActivity::~LiveListActivity() {
    brls::Logger::debug("del LiveListActivity");
}

void LiveListActivity::onContentAvailable() {
    titleLabel->setText(categoryName);
    statusLabel->setText("Loading rooms...");
    loadRooms();
}

void LiveListActivity::setPlatform(const std::string& p) {
    platform = p;
}

void LiveListActivity::setCategoryId(const std::string& id, const std::string& name) {
    categoryId = id;
    categoryName = name;
}

void LiveListActivity::loadRooms() {
    auto& api = LiveAPI::getInstance();

    auto onSuccess = [this](const LiveCategoryResult& result) {
        brls::Threading::sync([this, result]() {
            statusLabel->setText(std::to_string(result.items.size()) + " rooms loaded");
            brls::Logger::info("Loaded {} rooms", result.items.size());
        });
    };

    auto onError = [this](const std::string& error, int code) {
        brls::Threading::sync([this, error]() {
            statusLabel->setText("Error: " + error);
        });
    };

    if (platform == "bilibili") {
        api.getBiliRoomList(categoryId, 1, onSuccess, onError);
    } else if (platform == "douyu") {
        api.getDouyuRoomList(categoryId, 1, onSuccess, onError);
    } else if (platform == "huya") {
        api.getHuyaRoomList(categoryId, 1, onSuccess, onError);
    } else if (platform == "douyin") {
        api.getDouyinRoomList(categoryId, 1, onSuccess, onError);
    }
}

} // namespace live
