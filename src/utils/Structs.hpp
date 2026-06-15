#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct Server{
    int id;
    std::string title;
    std::string owner;
    std::string description;
    std::string fullDesc;
    std::string url;
    std::string pfp;
    std::string banner;
    std::string dcUrl;
    std::string toolsUrl;
    int views;
    int rating;
    int likes;
    int dislikes;
    std::string created_at;
    std::string version;
};

struct ServerEntry {
  std::string name;
  std::string url;
};


// taken from 
// https://github.com/cdc-sys/level-thumbs-mod/blob/4e72bb7d78eed712ff0fff532d4f4fcec1f2ded4/src/layers/ThumbnailPopup.cpp#L424-L449
// thanks prevter

// adapted from
// https://github.com/geode-sdk/geode/blob/2f390747385b2c7fcf15b606df10f87d671f3929/loader/src/server/Server.cpp#L262
static Result<uint64_t> parseISOTimestamp(std::string str) {
#ifdef GEODE_IS_WINDOWS
    std::stringstream ss(str);
    std::chrono::system_clock::time_point seconds;
    if (ss >> std::chrono::parse("%Y-%m-%dT%H:%M:%S", seconds)) {
        return Ok(std::chrono::duration_cast<std::chrono::seconds>(seconds.time_since_epoch()).count());
    }
    return Err("Invalid date time format '{}'", str);
#else
    auto dotPos = str.find('.');
    if (dotPos != std::string::npos) {
        str.resize(dotPos);
    }

    tm t;
    auto ptr = strptime(str.c_str(), "%Y-%m-%dT%H:%M:%S", &t);
    if (ptr == nullptr || (*ptr != '\0' && *ptr != 'Z')) {
        return Err("Invalid date time format '{}'", str);
    }

    return Ok(static_cast<uint64_t>(timegm(&t)));
#endif
}


template <>
struct matjson::Serialize<Server>
{
    static Result<Server> fromJson(matjson::Value const &value)
    {
        Server server = Server();
        server.id = value["id"].asInt().unwrapOrDefault();
        server.title = value["title"].asString().unwrapOr("Unknown");
        server.owner = value["owner"].asString().unwrapOr("Unknown");
        server.description = value["description"].asString().unwrapOr("No description provided.");
        server.fullDesc = value["full_description"].asString().unwrapOr("No description provided.");
        server.url = value["gdpsdb"].asString().unwrapOr("");
        server.pfp = value["pfp"].asString().unwrapOr("");
        server.banner = value["banner"].asString().unwrapOr("");
        server.dcUrl = value["discord_url"].asString().unwrapOr("");
        server.toolsUrl = value["toolpage"].asString().unwrapOr("");
        server.views = value["views"].asInt().unwrapOrDefault();
        server.rating = value["rating"].asInt().unwrapOrDefault();
        server.likes = value["likes"].asInt().unwrapOrDefault();
        server.dislikes = value["dislikes"].asInt().unwrapOrDefault();
        auto ca = value["created_at"].asString().unwrapOr("Unknown");
        auto caRes = parseISOTimestamp(ca);
        if (!caRes) {
            server.created_at = ca;
            log::error("Failed to parse timestamp: {}", caRes.unwrapErr());
        } else {
            auto tm = geode::localtime(caRes.unwrap());
            server.created_at = fmt::format("{:%Y-%m-%d at %H:%M}", tm);
        }
        server.version = value["version"].asString().unwrapOr("No version provided.");
        return Ok(server);
    }

    static matjson::Value toJson(Server const &value)
    {
        auto obj = matjson::Value();
        obj["id"] = value.id;
        obj["title"] = value.title;
        obj["owner"] = value.owner;
        obj["description"] = value.description;
        obj["full_description"] = value.fullDesc;
        obj["gdpsdb"] = value.url;
        obj["pfp"] = value.pfp;
        obj["discord_url"] = value.dcUrl;
        obj["toolpage"] = value.toolsUrl;
        obj["views"] = value.views;
        obj["rating"] = value.rating;
        obj["likes"] = value.likes;
        obj["dislikes"] = value.dislikes;
        obj["created_at"] = value.created_at;
        obj["version"] = value.version;
        return obj;
    }
};

template <>
struct matjson::Serialize<ServerEntry>
{
    static Result<ServerEntry> fromJson(matjson::Value const &value)
    {
        return Ok(ServerEntry{
            .name = value["name"].asString().unwrapOr("Failed to load name."),
            .url = value["url"].asString().unwrapOr("Failed to load url.")
        });
    }

    static matjson::Value toJson(ServerEntry const &value)
    {
        auto obj = matjson::makeObject({{"name", value.name}, {"url", value.url}});
        return obj;
    }
};