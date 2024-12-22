#pragma once
#include "matjson.hpp"  // Include matjson's header

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
    static Server from_json(matjson::Value const &value)
    {
        Server server = Server();
        if (!value["id"].is_number()) {
            server.id = 0;
        } else {
            server.id = value["id"].as_int();
        }
        
        if (!value["title"].is_string()) {
            server.title = "No title provided";
        } else {
            if (value["title"].as_string() == "") {
                server.title = "No title provided";
            } else {
                server.title = value["title"].as_string();
            }
        }

        if (!value["owner"].is_string()) {
            server.owner = "No owner provided";
        } else {
            if (value["owner"].as_string() == "") {
                server.owner = "No owner provided";
            } else {
                server.owner = value["owner"].as_string();
            }
        }
        
        if (!value["description"].is_string()) {
            server.description =  "No description provided.";
        } else {
            if (value["description"].as_string() == "") {
                server.description = "No description provided.";
            } else {
                server.description = value["description"].as_string();
            }
        }
        
        if (!value["gdpsdb"].is_string()) {
            server.url = "No URL provided.";
        } else {
            if (value["gdpsdb"].as_string() == "") {
                server.url = "No URL provided.";
            } else {
                server.url = value["gdpsdb"].as_string();
            }
        }
        
        if (!value["discord_url"].is_string()) {
            server.dcUrl = "No discord provided.";
        } else {
            if (value["discord_url"].as_string() == "") {
                server.dcUrl = "No discord provided.";
            } else {
                server.dcUrl = value["discord_url"].as_string();
            }
        }

        if (!value["toolpage"].is_string()) {
            server.toolsUrl = "No toolpage provided.";
        } else {
            if (value["toolpage"].as_string() == "") {
                server.toolsUrl = "No toolpage provided.";
            } else {
                server.toolsUrl = value["toolpage"].as_string();
            }
        }

        if (!value["pfp"].is_string()) {
            server.pfp =  "No PFP provided";
        } else {
            if (value["pfp"].as_string() == "") {
                server.pfp = "No PFP provided";
            } else {
                server.pfp = value["pfp"].as_string();
            }
        }
        
        if (!value["views"].is_number()) {
            server.views =  0;
        } else {
            server.views = value["views"].as_int();
        }

        if (!value["likes"].is_number()) {
            server.likes =  0;
        } else {
            server.likes = value["likes"].as_int();
        }

        if (!value["rating"].is_number()) {
            server.rating =  0;
        } else {
            server.rating = value["rating"].as_int();
        }


        if (!value["dislikes"].is_number()) {
            server.dislikes =  0;
        } else {
            server.dislikes = value["dislikes"].as_int();
        }
        
        if (!value["created_at"].is_string()) {
            server.created_at =  0;
        } else {
            if (value["created_at"].as_string() == "") {
                server.created_at = 0;
            } else {
                auto num = utils::numFromString<int>(value["created_at"].as_string());
                server.created_at = num.isOk() ? num.unwrap() : 0;
            }
        }

        return server;
    }
    /*
    {"id":2392,"title":"SurgeryGDPS","description":"The invite should hopefully work now","gdpsdb":"https://surgery.ps.fhgdps.com/dashboard/","pfp":null,"discord_url":"https://discord.com/Rb3zAVuQNN","views":1,"created_at":"1725482475"}
    */

    static matjson::Value to_json(Server const &value)
    {
        auto obj = matjson::Object();
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

template <> struct matjson::Serialize<ServerEntry> {
  static ServerEntry from_json(matjson::Value const &value) {
    return ServerEntry{.name = value["name"].as_string(),
                       .url = value["url"].as_string()};
  }

  static matjson::Value to_json(ServerEntry const &value) {
    auto obj = matjson::Object();
    obj["name"] = value.name;
    obj["url"] = value.url;
    return obj;
  }
};