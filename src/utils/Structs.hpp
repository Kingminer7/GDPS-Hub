#pragma once
#include "matjson.hpp"

using namespace geode::prelude;

struct Server{
    int id;
    std::string title;
    std::string owner;
    std::string description;
    std::string url;
    std::string pfp;
    std::string dcUrl;
    std::string toolsUrl;
    int views;
    int rating;
    int likes;
    int dislikes;
    int created_at;
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
        server.title = value["title"].asString().unwrapOr("No title provided");
        server.owner = value["owner"].asString().unwrapOr("No owner provided");
        server.description = value["description"].asString().unwrapOr("No description provided.");
        server.url = value["gdpsdb"].asString().unwrapOr("No URL provided.");
        server.pfp = value["pfp"].asString().unwrapOr("No PFP provided");
        server.dcUrl = value["discord_url"].asString().unwrapOr("No discord provided.");
        server.toolsUrl = value["toolpage"].asString().unwrapOr("No toolpage provided.");
        server.views = value["views"].asInt().unwrapOrDefault();
        server.rating = value["rating"].asInt().unwrapOrDefault();
        server.likes = value["likes"].asInt().unwrapOrDefault();
        server.dislikes = value["dislikes"].asInt().unwrapOrDefault();
        server.created_at = std::stoi(value["created_at"].asString().unwrapOrDefault());
        return Ok(server);
    }
    /*
    {"id":2392,"title":"SurgeryGDPS","description":"The invite should hopefully work now","gdpsdb":"https://surgery.ps.fhgdps.com/dashboard/","pfp":null,"discord_url":"https://discord.com/Rb3zAVuQNN","views":1,"created_at":"1725482475"}
    */

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