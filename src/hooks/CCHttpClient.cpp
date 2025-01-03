#include <Geode/modify/CCHttpClient.hpp>
#include "../utils/GDPSHub.hpp"

using namespace geode::prelude;

// some servers respond -1 if account data is left in

class $modify(CCHttpClient) {
	void send(CCHttpRequest* req)
    {
		if (GDPSHub::get()->isPreviewing()) {
            log::info("{}", req->getRequestData());

            auto reqData = req->getRequestData();
            std::vector<std::string> data;
            std::istringstream f(reqData);
            std::string s;
            while (getline(f, s, '&')) {
                data.push_back(s);
            }

            std::string newReqData = "";
            for (auto &d : data) {
                if (d.find("accountID=") != std::string::npos) {
                    continue;
                }
                if (d.find("gjp2=") != std::string::npos) {
                    continue;
                }
                
                if (d.find("uuid=") != std::string::npos) {
                    newReqData += "uuid=0&";
                    continue;
                }
                newReqData += d + "&";
            }
            newReqData.pop_back();
            req->setRequestData(newReqData.c_str(), newReqData.size());
        }
        CCHttpClient::send(req);
    }
};