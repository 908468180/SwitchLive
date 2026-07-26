#pragma once

#include <string>
#include <vector>
#include <functional>
#include <nlohmann/json.hpp>

namespace live {

struct LiveCategory {
    std::string id;
    std::string name;
};

struct LiveRoomItem {
    std::string room_id;
    std::string title;
    std::string cover_url;
    std::string avatar_url;
    std::string nick;
    int online = 0;
    std::string platform_id;
};

struct LivePlayQuality {
    std::string quality;
    std::string quality_type;
};

struct LivePlayUrl {
    std::string url;
    std::string backup_url;
};

struct LiveRoomDetail {
    std::string room_id;
    std::string title;
    std::string cover_url;
    std::string avatar_url;
    std::string nick;
    int live_status = 0;
    int online = 0;
    std::string platform_id;
};

struct LiveCategoryResult {
    std::vector<LiveRoomItem> items;
    int total = 0;
};

struct LiveSearchResult {
    std::vector<LiveRoomItem> items;
    int total = 0;
};

using ErrorCallback = std::function<void(const std::string&, int code)>;

class LiveAPI {
public:
    static LiveAPI& getInstance();

    void init();
    void setCookie(const std::string& cookie) { cookie_ = cookie; }

    // Bilibili Live API
    void getBiliCategories(std::function<void(const std::vector<LiveCategory>&)> callback,
                           ErrorCallback error = nullptr);
    
    void getBiliRoomList(const std::string& categoryId, int page,
                         std::function<void(const LiveCategoryResult&)> callback,
                         ErrorCallback error = nullptr);
    
    void getBiliRecommendRooms(int page,
                               std::function<void(const LiveCategoryResult&)> callback,
                               ErrorCallback error = nullptr);
    
    void getBiliRoomDetail(const std::string& roomId,
                           std::function<void(const LiveRoomDetail&)> callback,
                           ErrorCallback error = nullptr);
    
    void getBiliPlayUrl(const std::string& roomId, const std::string& quality,
                        std::function<void(const LivePlayUrl&)> callback,
                        ErrorCallback error = nullptr);
    
    void searchBiliRooms(const std::string& keyword, int page,
                         std::function<void(const LiveSearchResult&)> callback,
                         ErrorCallback error = nullptr);

    // Douyu Live API
    void getDouyuCategories(std::function<void(const std::vector<LiveCategory>&)> callback,
                            ErrorCallback error = nullptr);
    
    void getDouyuRoomList(const std::string& categoryId, int page,
                          std::function<void(const LiveCategoryResult&)> callback,
                          ErrorCallback error = nullptr);

    // Huya Live API
    void getHuyaCategories(std::function<void(const std::vector<LiveCategory>&)> callback,
                           ErrorCallback error = nullptr);
    
    void getHuyaRoomList(const std::string& categoryId, int page,
                         std::function<void(const LiveCategoryResult&)> callback,
                         ErrorCallback error = nullptr);

    // Douyin Live API
    void getDouyinCategories(std::function<void(const std::vector<LiveCategory>&)> callback,
                             ErrorCallback error = nullptr);
    
    void getDouyinRoomList(const std::string& categoryId, int page,
                           std::function<void(const LiveCategoryResult&)> callback,
                           ErrorCallback error = nullptr);

private:
    LiveAPI() = default;
    std::string cookie_;
};

} // namespace live
