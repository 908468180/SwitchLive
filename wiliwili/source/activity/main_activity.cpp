#include "activity/main_activity.hpp"
#include "fragment/live_list.hpp"
#include <borealis/core/application.hpp>
#include <borealis/core/thread.hpp>
#include <borealis/views/label.hpp>
#include <borealis/views/box.hpp>

namespace live {

MainActivity::~MainActivity() {
    brls::Logger::debug("del MainActivity");
}

void MainActivity::onContentAvailable() {
    statusLabel = dynamic_cast<brls::Label*>(this->getView("main/status"));
    titleLabel = dynamic_cast<brls::Label*>(this->getView("main/title"));
    categoryBox = dynamic_cast<brls::Box*>(this->getView("main/categories"));

    if (titleLabel) {
        titleLabel->setText("SwitchLive");
    }

    if (statusLabel) {
        statusLabel->setText("Loading categories...");
    }

    loadCategories(current_platform_);
}

void MainActivity::loadCategories(const std::string& platform) {
    current_platform_ = platform;

    if (statusLabel) {
        statusLabel->setText("Loading " + platform + " categories...");
    }

    auto& api = LiveAPI::getInstance();

    auto onSuccess = [this](const std::vector<LiveCategory>& categories) {
        brls::Threading::sync([this, categories]() {
            if (statusLabel) {
                statusLabel->setText(std::to_string(categories.size()) + " categories loaded");
            }
            brls::Logger::info("Loaded {} categories", categories.size());
        });
    };

    auto onError = [this](const std::string& error, int code) {
        brls::Threading::sync([this, error]() {
            if (statusLabel) {
                statusLabel->setText("Error: " + error);
            }
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
