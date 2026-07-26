#pragma once

#include <borealis/core/activity.hpp>
#include <borealis/core/bind.hpp>
#include <borealis/views/recycling_grid.hpp>
#include "api/live_api.hpp"

namespace live {

class LiveCard : public RecyclingGridItem {
public:
    LiveCard();
    ~LiveCard() override;

    void setCard(const LiveRoomItem& room);
    void prepareForReuse() override;
    static LiveCard* create();

private:
    brls::Image* picture = nullptr;
    brls::Label* titleLabel = nullptr;
    brls::Label* nameLabel = nullptr;
    brls::Label* viewersLabel = nullptr;
};

class LiveListDataSource : public RecyclingGridDataSource {
public:
    LiveListDataSource() = default;

    void setData(const std::vector<LiveRoomItem>& rooms);
    RecyclingGridItem* cellForRow(RecyclingGrid* recycler, size_t index) override;
    size_t getItemCount() override;
    void onItemSelected(RecyclingGrid* recycler, size_t index) override;
    void clearData() override;

    std::function<void(const LiveRoomItem&)> onRoomSelected;

private:
    std::vector<LiveRoomItem> rooms_;
};

class LiveListFragment : public brls::Activity {
public:
    CONTENT_FROM_XML_RES("fragment/live_list.xml");

    void onContentAvailable() override;
    ~LiveListFragment() override;

    void setPlatform(const std::string& platform);
    void setCategoryId(const std::string& id, const std::string& name);

private:
    void loadRooms();

    brls::RecyclingGrid* grid = nullptr;
    brls::Label* titleLabel = nullptr;
    brls::Label* statusLabel = nullptr;
    LiveListDataSource* dataSource = nullptr;

    std::string platform_;
    std::string categoryId_;
    std::string categoryName_;
};

} // namespace live
