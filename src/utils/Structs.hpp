using namespace geode::prelude;

struct Server
    {
    int id;
    std::string title;
    std::string description;
    std::string url;
    std::string pfp;
    std::string dcUrl;
    int views;
    int created_at;
};

template <>
struct matjson::Serialize<Server>
{
    static Server from_json(matjson::Value const &value)
    {
        return Server{
            .id = value["id"].as_int(),
            .title = value["title"].as_string(),
            .description = value["description"].as_string(),
            .url = value["url"].as_string(),
            .pfp = value["pfp"].as_string(),
            .dcUrl = value["dcUrl"].as_string(),
            .views = value["views"].as_int(),
            .created_at = value["created_at"].as_int()
        };
    }

    static matjson::Value to_json(Server const &value)
    {
        auto obj = matjson::Object();
        obj["id"] = value.id;
        obj["title"] = value.title;
        obj["description"] = value.description;
        obj["url"] = value.url;
        obj["pfp"] = value.pfp;
        obj["dcUrl"] = value.dcUrl;
        obj["views"] = value.views;
        obj["created_at"] = value.created_at;
        return obj;
    }
};