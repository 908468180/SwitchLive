#include "activity/main_activity.hpp"

namespace live {

MainActivity::~MainActivity() {
    brls::Logger::debug("del MainActivity");
}

void MainActivity::onContentAvailable() {
    titleLabel->setText("SwitchLive");
    statusLabel->setText("Loading categories...");

    loadCategories(currentPlatform);
}

void MainActivity::loadCategories(const std::string& platform) {
    currentPlatform = platform;
    statusLabel->setText("Loading " + platform + " categories...");

    auto& api = LiveAPI::getInstance();

    auto onSuccess = [this](const std::vector<LiveCategory>& categories) {
        brls::Threading::sync([this, categories]() {
            statusLabel->setText(std::to_string(categories.size()) + " categories loaded");
            brls::Logger::info("Loaded {} categories", categories.size());
        });
    };

    auto onError = [this](const std::string& error, int code) {
        brls::Threading::sync([this, error]() {
            statusLabel->setText("Error: " + error);
        });
    };

    if (platform == "bilibili") {
        api.getBiliCategories(onSuccess, onError);
    } else if (platform == "douyu") {
        api.getDouyuCategories(onSuccess, onError);
    } else if (platform == "huya") {
        api.getHuyaCategories(onSuccess, onError);
    } else if (platform == "douyin") {
        api.getDouyinCategories(onSuccess, onError);
    }
}

} // namespace live
