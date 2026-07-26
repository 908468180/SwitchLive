#include "api/live_api.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace live {

LiveAPI& LiveAPI::getInstance() {
    static LiveAPI instance;
    return instance;
}

void LiveAPI::init() {
    // Initialize HTTP client
}

// Bilibili Live API
void LiveAPI::getBiliCategories(std::function<void(const std::vector<LiveCategory>&)> callback,
                                ErrorCallback error) {
    cpr::Response r = cpr::Get(
        cpr::Url{"https://api.live.bilibili.com/room/v1/Area/getList"},
        cpr::Header{{"User-Agent", "Mozilla/5.0"}}
    );

    if (r.status_code != 200) {
        if (error) error("HTTP request failed", r.status_code);
        return;
    }

    try {
        auto j = json::parse(r.text);
        std::vector<LiveCategory> categories;
        
        if (j.contains("data") && j["data"].is_array()) {
            for (auto& item : j["data"]) {
                LiveCategory cat;
                cat.id = std::to_string(item["id"].get<int>());
                cat.name = item["name"].get<std::string>();
                categories.push_back(cat);
            }
        }
        
        callback(categories);
    } catch (const std::exception& e) {
        if (error) error(e.what(), -1);
    }
}

void LiveAPI::getBiliRoomList(const std::string& categoryId, int page,
                              std::function<void(const LiveCategoryResult&)> callback,
                              ErrorCallback error) {
    cpr::Response r = cpr::Get(
        cpr::Url{"https://api.live.bilibili.com/room/v1/area/getRoomList"},
        cpr::Parameters{
            {"area_id", categoryId},
            {"page", std::to_string(page)}
        },
        cpr::Header{{"User-Agent", "Mozilla/5.0"}}
    );

    if (r.status_code != 200) {
        if (error) error("HTTP request failed", r.status_code);
        return;
    }

    try {
        auto j = json::parse(r.text);
        LiveCategoryResult result;
        
        if (j.contains("data") && j["data"].contains("list")) {
            for (auto& item : j["data"]["list"]) {
                LiveRoomItem room;
                room.room_id = std::to_string(item["roomid"].get<int>());
                room.title = item["title"].get<std::string>();
                room.cover_url = item["cover"].get<std::string>();
                room.nick = item["uname"].get<std::string>();
                room.online = item["online"].get<int>();
                room.platform_id = "bilibili";
                result.items.push_back(room);
            }
        }
        
        callback(result);
    } catch (const std::exception& e) {
        if (error) error(e.what(), -1);
    }
}

void LiveAPI::getBiliRecommendRooms(int page,
                                    std::function<void(const LiveCategoryResult&)> callback,
                                    ErrorCallback error) {
    cpr::Response r = cpr::Get(
        cpr::Url{"https://api.live.bilibili.com/room/v1/area/getRoomList"},
        cpr::Parameters{
            {"area_id", "0"},
            {"page", std::to_string(page)}
        },
        cpr::Header{{"User-Agent", "Mozilla/5.0"}}
    );

    if (r.status_code != 200) {
        if (error) error("HTTP request failed", r.status_code);
        return;
    }

    try {
        auto j = json::parse(r.text);
        LiveCategoryResult result;
        
        if (j.contains("data") && j["data"].contains("list")) {
            for (auto& item : j["data"]["list"]) {
                LiveRoomItem room;
                room.room_id = std::to_string(item["roomid"].get<int>());
                room.title = item["title"].get<std::string>();
                room.cover_url = item["cover"].get<std::string>();
                room.nick = item["uname"].get<std::string>();
                room.online = item["online"].get<int>();
                room.platform_id = "bilibili";
                result.items.push_back(room);
            }
        }
        
        callback(result);
    } catch (const std::exception& e) {
        if (error) error(e.what(), -1);
    }
}

void LiveAPI::getBiliRoomDetail(const std::string& roomId,
                                std::function<void(const LiveRoomDetail&)> callback,
                                ErrorCallback error) {
    cpr::Response r = cpr::Get(
        cpr::Url{"https://api.live.bilibili.com/room/v1/Room/get_info"},
        cpr::Parameters{{"room_id", roomId}},
        cpr::Header{{"User-Agent", "Mozilla/5.0"}}
    );

    if (r.status_code != 200) {
        if (error) error("HTTP request failed", r.status_code);
        return;
    }

    try {
        auto j = json::parse(r.text);
        LiveRoomDetail detail;
        
        if (j.contains("data")) {
            auto& data = j["data"];
            detail.room_id = std::to_string(data["room_id"].get<int>());
            detail.title = data["title"].get<std::string>();
            detail.cover_url = data["cover"].get<std::string>();
            detail.avatar_url = data["avatar"].get<std::string>();
            detail.nick = data["uname"].get<std::string>();
            detail.live_status = data["live_status"].get<int>();
            detail.online = data["online"].get<int>();
            detail.platform_id = "bilibili";
        }
        
        callback(detail);
    } catch (const std::exception& e) {
        if (error) error(e.what(), -1);
    }
}

void LiveAPI::getBiliPlayUrl(const std::string& roomId, const std::string& quality,
                             std::function<void(const LivePlayUrl&)> callback,
                             ErrorCallback error) {
    cpr::Response r = cpr::Get(
        cpr::Url{"https://api.live.bilibili.com/room/v1/Room/playUrl"},
        cpr::Parameters{
            {"room_id", roomId},
            {"quality", quality},
            {"platform", "h5"}
        },
        cpr::Header{{"User-Agent", "Mozilla/5.0"}}
    );

    if (r.status_code != 200) {
        if (error) error("HTTP request failed", r.status_code);
        return;
    }

    try {
        auto j = json::parse(r.text);
        LivePlayUrl url;
        
        if (j.contains("data") && j["data"].contains("durl")) {
            auto& durls = j["data"]["durl"];
            if (durls.is_array() && !durls.empty()) {
                url.url = durls[0]["url"].get<std::string>();
                if (durls[0].contains("backup_url") && durls[0]["backup_url"].is_array()) {
                    if (!durls[0]["backup_url"].empty()) {
                        url.backup_url = durls[0]["backup_url"][0].get<std::string>();
                    }
                }
            }
        }
        
        callback(url);
    } catch (const std::exception& e) {
        if (error) error(e.what(), -1);
    }
}

void LiveAPI::searchBiliRooms(const std::string& keyword, int page,
                              std::function<void(const LiveSearchResult&)> callback,
                              ErrorCallback error) {
    cpr::Response r = cpr::Get(
        cpr::Url{"https://api.live.bilibili.com/search/api/type"},
        cpr::Parameters{
            {"keyword", keyword},
            {"page", std::to_string(page)}
        },
        cpr::Header{{"User-Agent", "Mozilla/5.0"}}
    );

    if (r.status_code != 200) {
        if (error) error("HTTP request failed", r.status_code);
        return;
    }

    try {
        auto j = json::parse(r.text);
        LiveSearchResult result;
        
        if (j.contains("data") && j["data"].contains("result")) {
            for (auto& item : j["data"]["result"]) {
                if (item.contains("live_room")) {
                    LiveRoomItem room;
                    room.room_id = std::to_string(item["live_room"]["roomid"].get<int>());
                    room.title = item["live_room"]["title"].get<std::string>();
                    room.cover_url = item["live_room"]["cover"].get<std::string>();
                    room.nick = item["uname"].get<std::string>();
                    room.platform_id = "bilibili";
                    result.items.push_back(room);
                }
            }
        }
        
        callback(result);
    } catch (const std::exception& e) {
        if (error) error(e.what(), -1);
    }
}

// Douyu Live API
void LiveAPI::getDouyuCategories(std::function<void(const std::vector<LiveCategory>&)> callback,
                                 ErrorCallback error) {
    cpr::Response r = cpr::Get(
        cpr::Url{"https://www.douyu.com/api/v1/live/category"},
        cpr::Header{{"User-Agent", "Mozilla/5.0"}}
    );

    if (r.status_code != 200) {
        if (error) error("HTTP request failed", r.status_code);
        return;
    }

    try {
        auto j = json::parse(r.text);
        std::vector<LiveCategory> categories;
        
        if (j.contains("data") && j["data"].is_array()) {
            for (auto& item : j["data"]) {
                LiveCategory cat;
                cat.id = std::to_string(item["id"].get<int>());
                cat.name = item["name"].get<std::string>();
                categories.push_back(cat);
            }
        }
        
        callback(categories);
    } catch (const std::exception& e) {
        if (error) error(e.what(), -1);
    }
}

void LiveAPI::getDouyuRoomList(const std::string& categoryId, int page,
                               std::function<void(const LiveCategoryResult&)> callback,
                               ErrorCallback error) {
    cpr::Response r = cpr::Get(
        cpr::Url{"https://www.douyu.com/api/v1/live/roomList"},
        cpr::Parameters{
            {"cate_id", categoryId},
            {"page", std::to_string(page)}
        },
        cpr::Header{{"User-Agent", "Mozilla/5.0"}}
    );

    if (r.status_code != 200) {
        if (error) error("HTTP request failed", r.status_code);
        return;
    }

    try {
        auto j = json::parse(r.text);
        LiveCategoryResult result;
        
        if (j.contains("data") && j["data"].contains("list")) {
            for (auto& item : j["data"]["list"]) {
                LiveRoomItem room;
                room.room_id = std::to_string(item["room_id"].get<int>());
                room.title = item["room_name"].get<std::string>();
                room.cover_url = item["room_src"].get<std::string>();
                room.nick = item["nickname"].get<std::string>();
                room.online = item["online"].get<int>();
                room.platform_id = "douyu";
                result.items.push_back(room);
            }
        }
        
        callback(result);
    } catch (const std::exception& e) {
        if (error) error(e.what(), -1);
    }
}

// Huya Live API
void LiveAPI::getHuyaCategories(std::function<void(const std::vector<LiveCategory>&)> callback,
                                ErrorCallback error) {
    cpr::Response r = cpr::Get(
        cpr::Url{"https://live.huya.com/api/base/bigLiveCategories"},
        cpr::Header{{"User-Agent", "Mozilla/5.0"}}
    );

    if (r.status_code != 200) {
        if (error) error("HTTP request failed", r.status_code);
        return;
    }

    try {
        auto j = json::parse(r.text);
        std::vector<LiveCategory> categories;
        
        if (j.contains("data") && j["data"].is_array()) {
            for (auto& item : j["data"]) {
                LiveCategory cat;
                cat.id = std::to_string(item["id"].get<int>());
                cat.name = item["name"].get<std::string>();
                categories.push_back(cat);
            }
        }
        
        callback(categories);
    } catch (const std::exception& e) {
        if (error) error(e.what(), -1);
    }
}

void LiveAPI::getHuyaRoomList(const std::string& categoryId, int page,
                              std::function<void(const LiveCategoryResult&)> callback,
                              ErrorCallback error) {
    cpr::Response r = cpr::Get(
        cpr::Url{"https://live.huya.com/api/base/bigLiveList"},
        cpr::Parameters{
            {"gameId", categoryId},
            {"page", std::to_string(page)}
        },
        cpr::Header{{"User-Agent", "Mozilla/5.0"}}
    );

    if (r.status_code != 200) {
        if (error) error("HTTP request failed", r.status_code);
        return;
    }

    try {
        auto j = json::parse(r.text);
        LiveCategoryResult result;
        
        if (j.contains("data") && j["data"].contains("list")) {
            for (auto& item : j["data"]["list"]) {
                LiveRoomItem room;
                room.room_id = std::to_string(item["profileRoom"].get<int>());
                room.title = item["introduction"].get<std::string>();
                room.cover_url = item["sScreenshot"].get<std::string>();
                room.nick = item["nick"].get<std::string>();
                room.online = item["online"].get<int>();
                room.platform_id = "huya";
                result.items.push_back(room);
            }
        }
        
        callback(result);
    } catch (const std::exception& e) {
        if (error) error(e.what(), -1);
    }
}

// Douyin Live API
void LiveAPI::getDouyinCategories(std::function<void(const std::vector<LiveCategory>&)> callback,
                                  ErrorCallback error) {
    cpr::Response r = cpr::Get(
        cpr::Url{"https://live.douyin.com/webcast/ranklist/hot/"},
        cpr::Header{{"User-Agent", "Mozilla/5.0"}}
    );

    if (r.status_code != 200) {
        if (error) error("HTTP request failed", r.status_code);
        return;
    }

    try {
        auto j = json::parse(r.text);
        std::vector<LiveCategory> categories;
        
        if (j.contains("data") && j["data"].contains("ranks")) {
            for (auto& item : j["data"]["ranks"]) {
                LiveCategory cat;
                cat.id = std::to_string(item["id"].get<int>());
                cat.name = item["name"].get<std::string>();
                categories.push_back(cat);
            }
        }
        
        callback(categories);
    } catch (const std::exception& e) {
        if (error) error(e.what(), -1);
    }
}

void LiveAPI::getDouyinRoomList(const std::string& categoryId, int page,
                                std::function<void(const LiveCategoryResult&)> callback,
                                ErrorCallback error) {
    cpr::Response r = cpr::Get(
        cpr::Url{"https://live.douyin.com/webcast/room/web/enter/"},
        cpr::Parameters{
            {"aid", "6383"},
            {"category_id", categoryId},
            {"cursor", std::to_string(page)}
        },
        cpr::Header{{"User-Agent", "Mozilla/5.0"}}
    );

    if (r.status_code != 200) {
        if (error) error("HTTP request failed", r.status_code);
        return;
    }

    try {
        auto j = json::parse(r.text);
        LiveCategoryResult result;
        
        if (j.contains("data") && j["data"].contains("rooms")) {
            for (auto& item : j["data"]["rooms"]) {
                LiveRoomItem room;
                room.room_id = item["id_str"].get<std::string>();
                room.title = item["title"].get<std::string>();
                room.cover_url = item["cover"]["url_list"][0].get<std::string>();
                room.nick = item["author"]["nickname"].get<std::string>();
                room.online = item["user_count"].get<int>();
                room.platform_id = "douyin";
                result.items.push_back(room);
            }
        }
        
        callback(result);
    } catch (const std::exception& e) {
        if (error) error(e.what(), -1);
    }
}

} // namespace live
