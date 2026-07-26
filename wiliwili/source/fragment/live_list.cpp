#include "fragment/live_list.hpp"
#include "activity/player_activity.hpp"
#include <borealis/core/application.hpp>
#include <borealis/core/thread.hpp>

namespace live {

// LiveCard
LiveCard::LiveCard() {
    this->inflateFromXMLString(R"xml(
<brls:Box
    width="300"
    height="200"
    focusable="true"
    flexDirection="column"
    padding="10"
    cornerRadius="8">
    <brls:Image
        id="live/picture"
        width="280"
        height="140"
        scalingType="fill"
        cornerRadius="6"/>
    <brls:Label
        id="live/title"
        fontSize="16"
        width="280"
        height="auto"
        marginTop="6"
        singleLine="true"/>
    <brls:Label
        id="live/name"
        fontSize="13"
        width="280"
        height="auto"
        marginTop="2"
        singleLine="true"/>
    <brls:Label
        id="live/viewers"
        fontSize="12"
        width="280"
        height="auto"
        marginTop="2"/>
</brls:Box>
)xml");
}

LiveCard::~LiveCard() = default;

void LiveCard::setCard(const LiveRoomItem& room) {
    if (picture && !room.cover_url.empty()) {
        picture->setImageFromURL(room.cover_url);
    }
    if (titleLabel) titleLabel->setText(room.title);
    if (nameLabel) nameLabel->setText(room.nick);
    if (viewersLabel) viewersLabel->setText(std::to_string(room.online) + " viewers");
}

void LiveCard::prepareForReuse() {
    if (picture) picture->setImageFromRes("pictures/video-card-bg.png");
    if (titleLabel) titleLabel->setText("");
    if (nameLabel) nameLabel->setText("");
    if (viewersLabel) viewersLabel->setText("");
}

LiveCard* LiveCard::create() {
    auto* card = new LiveCard();
    card->picture     = card->getViewById<brls::Image>("live/picture");
    card->titleLabel  = card->getViewById<brls::Label>("live/title");
    card->nameLabel   = card->getViewById<brls::Label>("live/name");
    card->viewersLabel = card->getViewById<brls::Label>("live/viewers");
    return card;
}

// LiveListDataSource
void LiveListDataSource::setData(const std::vector<LiveRoomItem>& rooms) {
    rooms_ = rooms;
}

RecyclingGridItem* LiveListDataSource::cellForRow(RecyclingGrid* recycler, size_t index) {
    LiveCard* item = (LiveCard*)recycler->dequeueReusableCell("LiveCard");
    item->setCard(rooms_[index]);
    return item;
}

size_t LiveListDataSource::getItemCount() {
    return rooms_.size();
}

void LiveListDataSource::onItemSelected(RecyclingGrid* recycler, size_t index) {
    if (onRoomSelected) onRoomSelected(rooms_[index]);
}

void LiveListDataSource::clearData() {
    rooms_.clear();
}

// LiveListFragment
LiveListFragment::~LiveListFragment() {
    brls::Logger::debug("del LiveListFragment");
}

void LiveListFragment::onContentAvailable() {
    grid       = this->getViewById<brls::RecyclingGrid>("live/grid");
    titleLabel = this->getViewById<brls::Label>("live/title");
    statusLabel = this->getViewById<brls::Label>("live/status");

    if (titleLabel) titleLabel->setText(categoryName_);

    if (grid) {
        grid->registerCell("LiveCard", []() { return LiveCard::create(); });
        dataSource = new LiveListDataSource();
        dataSource->onRoomSelected = [this](const LiveRoomItem& room) {
            auto* activity = new PlayerActivity();
            activity->setRoom(room);
            brls::Application::pushActivity(activity);
        };
        grid->setDataSource(dataSource);
    }

    if (statusLabel) statusLabel->setText("Loading rooms...");

    loadRooms();
}

void LiveListFragment::setPlatform(const std::string& platform) {
    platform_ = platform;
}

void LiveListFragment::setCategoryId(const std::string& id, const std::string& name) {
    categoryId_ = id;
    categoryName_ = name;
}

void LiveListFragment::loadRooms() {
    auto& api = LiveAPI::getInstance();

    auto onSuccess = [this](const LiveCategoryResult& result) {
        brls::Threading::sync([this, result]() {
            if (dataSource) {
                dataSource->setData(result.items);
                grid->notifyDataChanged();
            }
            if (statusLabel) {
                statusLabel->setText(std::to_string(result.items.size()) + " rooms");
            }
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
