#pragma once

#include <borealis/core/activity.hpp>
#include <borealis/core/bind.hpp>
#include <borealis/views/recycling_grid.hpp>
#include "api/live_api.hpp"

namespace live {

class CategoryCard : public RecyclingGridItem {
public:
    CategoryCard();
    ~CategoryCard() override;

    void setData(const LiveCategory& cat);
    void prepareForReuse() override;
    static CategoryCard* create();

private:
    brls::Label* nameLabel = nullptr;
};

class CategoryDataSource : public RecyclingGridDataSource {
public:
    CategoryDataSource() = default;

    void setData(const std::vector<LiveCategory>& cats, const std::string& platform);
    RecyclingGridItem* cellForRow(RecyclingGrid* recycler, size_t index) override;
    size_t getItemCount() override;
    void onItemSelected(RecyclingGrid* recycler, size_t index) override;
    void clearData() override;

    std::function<void(const LiveCategory&)> onCategorySelected;

private:
    std::vector<LiveCategory> categories_;
    std::string platform_;
};

class MainActivity : public brls::Activity {
public:
    CONTENT_FROM_XML_RES("activity/main.xml");

    void onContentAvailable() override;
    ~MainActivity() override;

private:
    void loadCategories(const std::string& platform);
    void onCategoryClick(const LiveCategory& cat);

    brls::RecyclingGrid* grid = nullptr;
    brls::Label* statusLabel = nullptr;
    CategoryDataSource* dataSource = nullptr;

    std::string current_platform_ = "bilibili";
};

} // namespace live
