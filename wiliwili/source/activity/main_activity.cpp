#include "activity/main_activity.hpp"
#include "fragment/live_list.hpp"
#include <borealis/core/application.hpp>
#include <borealis/core/thread.hpp>

namespace live {

// CategoryCard
CategoryCard::CategoryCard() {
    this->inflateFromXMLString(R"xml(
<brls:Box
    width="200"
    height="80"
    focusable="true"
    paddingLeft="10"
    paddingRight="10"
    paddingTop="8"
    paddingBottom="8"
    backgroundColor="@theme/brls/list/selection_box"
    cornerRadius="8">
    <brls:Label
        id="cat/name"
        width="match_parent"
        height="match_parent"
        verticalAlign="center"
        horizontalAlign="center"
        fontSize="18"
        singleLine="true"/>
</brls:Box>
)xml");
}

CategoryCard::~CategoryCard() = default;

void CategoryCard::setData(const LiveCategory& cat) {
    if (nameLabel) {
        nameLabel->setText(cat.name);
    }
}

void CategoryCard::prepareForReuse() {
    if (nameLabel) {
        nameLabel->setText("");
    }
}

CategoryCard* CategoryCard::create() {
    auto* card = new CategoryCard();
    card->nameLabel = card->getViewById<brls::Label>("cat/name");
    return card;
}

// CategoryDataSource
void CategoryDataSource::setData(const std::vector<LiveCategory>& cats, const std::string& platform) {
    categories_ = cats;
    platform_ = platform;
}

RecyclingGridItem* CategoryDataSource::cellForRow(RecyclingGrid* recycler, size_t index) {
    CategoryCard* item = (CategoryCard*)recycler->dequeueReusableCell("CategoryCard");
    item->setData(categories_[index]);
    return item;
}

size_t CategoryDataSource::getItemCount() {
    return categories_.size();
}

void CategoryDataSource::onItemSelected(RecyclingGrid* recycler, size_t index) {
    if (onCategorySelected) {
        onCategorySelected(categories_[index]);
    }
}

void CategoryDataSource::clearData() {
    categories_.clear();
}

// MainActivity
MainActivity::~MainActivity() {
    brls::Logger::debug("del MainActivity");
}

void MainActivity::onContentAvailable() {
    grid = this->getViewById<brls::RecyclingGrid>("main/grid");
    statusLabel = this->getViewById<brls::Label>("main/status");

    if (grid) {
        grid->registerCell("CategoryCard", []() { return CategoryCard::create(); });
        dataSource = new CategoryDataSource();
        dataSource->onCategorySelected = [this](const LiveCategory& cat) {
            onCategoryClick(cat);
        };
        grid->setDataSource(dataSource);
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
            if (dataSource) {
                dataSource->setData(categories, current_platform_);
                grid->notifyDataChanged();
            }
            if (statusLabel) {
                statusLabel->setText(std::to_string(categories.size()) + " categories found");
            }
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

void MainActivity::onCategoryClick(const LiveCategory& cat) {
    brls::Logger::debug("Selected category: {} ({})", cat.name, cat.id);

    auto* fragment = new LiveListFragment();
    fragment->setPlatform(current_platform_);
    fragment->setCategoryId(cat.id, cat.name);
    brls::Application::pushActivity(fragment);
}

} // namespace live
