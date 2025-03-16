#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct Server{
    int id;
    std::string title;
    std::string owner;
    std::string description;
    std::string url;
    std::string pfp;
    std::string banner;
    std::string dcUrl;
    std::string toolsUrl;
    int views;
    int rating;
    int likes;
    int dislikes;
    int created_at;
    std::string version;
};

struct ServerEntry {
  std::string name;
  std::string url;
};



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
        server.url = value["gdpsdb"].asString().unwrapOr("");
        server.pfp = value["pfp"].asString().unwrapOr("");
        server.banner = value["banner"].asString().unwrapOr("");
        server.dcUrl = value["discord_url"].asString().unwrapOr("");
        server.toolsUrl = value["toolpage"].asString().unwrapOr("");
        server.views = value["views"].asInt().unwrapOrDefault();
        server.rating = value["rating"].asInt().unwrapOrDefault();
        server.likes = value["likes"].asInt().unwrapOrDefault();
        server.dislikes = value["dislikes"].asInt().unwrapOrDefault();
        server.created_at = std::stoi(value["created_at"].asString().unwrapOrDefault());
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
