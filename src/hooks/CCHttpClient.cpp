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

	    int shortenBy = 0;
            std::string newReqData = "";
            for (auto &d : data) {
                if (d.find("accountID=") != std::string::npos) {
            	    shortenBy += d.size();
                    continue;
                }
                if (d.find("gjp2=") != std::string::npos) {
            	    shortenBy += d.size();
                    continue;
                }
                
                if (d.find("uuid=") != std::string::npos) {
            	    shortenBy += d.size() - 7;
                    newReqData += "uuid=0&";
                    continue;
                }
                newReqData += d + "&";
            }
            newReqData.pop_back();
            req->setRequestData(newReqData.c_str(), newReqData.size() - shortenBy);
        }
        CCHttpClient::send(req);
    }
};
