#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <map>
#include <cctype>
#include <grpc++/grpc++.h>
#include <httplib.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <sstream>
#include "database.h"

#include "polymarket_clob.grpc.pb.h"
#include "database_layer.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class StatusServiceImpl final : public polymarket::clob::status::Service {
public:
    grpc::Status checkStatus(grpc::ServerContext* context, const polymarket::clob::Empty* request, polymarket::clob::ClobApiStatus* response) override {
        try{
            httplib::SSLClient client("data-api.polymarket.com", 443);
            auto res = client.Get("/");

            if(!res) {
                std::cerr << "Error getting status response" << std::endl;
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from data-api.polymarket.com");
            }
            if (res->status != 200) {
                std::cerr << "Error: HTTP status " << res->status << std::endl;
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error from data-api.polymarket.com: " + std::to_string(res->status));
            }

            response->set_status(res->body);
            std::cout << "[checkStatus] Called," << res->body << std::endl;
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in checkStatus: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }
};

class SportsServiceImpl final : public polymarket::clob::sports::Service {
public:
    grpc::Status listTeams(grpc::ServerContext* context, const polymarket::clob::ListTeamsRequest* request, polymarket::clob::ListTeamsResponse* response) override {
        try{
            httplib::SSLClient client("gamma-api.polymarket.com", 443);
            auto res = client.Get("/teams");

            if(!res) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from API");
            }
            if (res->status != 200) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error: " + std::to_string(res->status));
            }

            response->set_json_data(res->body);
            std::cout << "[listTeams] Called," << res->body << std::endl;
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in listTeams: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }

    grpc::Status getSportsMetadata(grpc::ServerContext* context, const polymarket::clob::Empty* request, polymarket::clob::SportsMetadataResponse* response) override {
        try{
            httplib::SSLClient client("gamma-api.polymarket.com", 443);
            auto res = client.Get("/sports");

            if(!res) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from API");
            }
            if (res->status != 200) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error: " + std::to_string(res->status));
            }

            response->set_json_data(res->body);
            std::cout << "[getSportsMetadata] Called," << res->body << std::endl;
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in getSportsMetadata: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }
};

class EventsServiceImpl final : public polymarket::clob::events::Service {
public:
    grpc::Status listEvents(grpc::ServerContext* context, const polymarket::clob::ListEventsRequest* request, polymarket::clob::ListEventsResponse* response) override {
        try{
            httplib::SSLClient client("gamma-api.polymarket.com", 443);
            auto res = client.Get("/events");

            if(!res) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from API");
            }
            if (res->status != 200) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error: " + std::to_string(res->status));
            }

            response->set_json_data(res->body);
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in listEvents: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }

    grpc::Status getEventById(grpc::ServerContext* context, const polymarket::clob::GetEventByIdRequest* request, polymarket::clob::Event* response) override {
        try{
            httplib::SSLClient client("gamma-api.polymarket.com", 443);
            std::string path = "/events/" + std::to_string(request->id());
            auto res = client.Get(path.c_str());

            if(!res) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from API");
            }
            if (res->status != 200) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error: " + std::to_string(res->status));
            }

            response->set_json_data(res->body);
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in getEventById: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }

    grpc::Status getEventBySlug(grpc::ServerContext* context, const polymarket::clob::GetEventBySlugRequest* request, polymarket::clob::Event* response) override {
        try{
            httplib::SSLClient client("gamma-api.polymarket.com", 443);
            std::string path = "/events/slug/" + request->slug();
            auto res = client.Get(path.c_str());

            if(!res) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from API");
            }
            if (res->status != 200) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error: " + std::to_string(res->status));
            }

            response->set_json_data(res->body);
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in getEventBySlug: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }

    grpc::Status getEventTags(grpc::ServerContext* context, const polymarket::clob::GetEventTagsRequest* request, polymarket::clob::GetEventTagsResponse* response) override {
        try{
            httplib::SSLClient client("gamma-api.polymarket.com", 443);
            std::string path = "/events/" + std::to_string(request->id()) + "/tags";
            auto res = client.Get(path.c_str());

            if(!res) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from API");
            }
            if (res->status != 200) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error: " + std::to_string(res->status));
            }

            response->set_json_data(res->body);
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in getEventTags: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }
};

class MarketsServiceImpl final : public polymarket::clob::markets::Service {
public:
    grpc::Status listMarkets(grpc::ServerContext* context, const polymarket::clob::ListMarketsRequest* request, polymarket::clob::ListMarketsResponse* response) override {
        try{
            httplib::SSLClient client("gamma-api.polymarket.com", 443);
            auto res = client.Get("/markets");

            if(!res) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from API");
            }
            if (res->status != 200) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error: " + std::to_string(res->status));
            }

            response->set_json_data(res->body);
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in listMarkets: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }

    grpc::Status getMarketById(grpc::ServerContext* context, const polymarket::clob::GetMarketByIdRequest* request, polymarket::clob::Market* response) override {
        try{
            httplib::SSLClient client("gamma-api.polymarket.com", 443);
            std::string path = "/markets/" + std::to_string(request->id());
            auto res = client.Get(path.c_str());

            if(!res) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from API");
            }
            if (res->status != 200) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error: " + std::to_string(res->status));
            }

            response->set_json_data(res->body);
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in getMarketById: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }

    grpc::Status getMarketBySlug(grpc::ServerContext* context, const polymarket::clob::GetMarketBySlugRequest* request, polymarket::clob::Market* response) override {
        try{
            httplib::SSLClient client("gamma-api.polymarket.com", 443);
            std::string path = "/markets/slug/" + request->slug();
            auto res = client.Get(path.c_str());

            if(!res) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from API");
            }
            if (res->status != 200) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error: " + std::to_string(res->status));
            }

            response->set_json_data(res->body);
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in getMarketBySlug: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }

    grpc::Status getMarketTags(grpc::ServerContext* context, const polymarket::clob::GetMarketTagsRequest* request, polymarket::clob::GetMarketTagsResponse* response) override {
        try{
            httplib::SSLClient client("gamma-api.polymarket.com", 443);
            std::string path = "/markets/" + std::to_string(request->id()) + "/tags";
            auto res = client.Get(path.c_str());

            if(!res) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from API");
            }
            if (res->status != 200) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error: " + std::to_string(res->status));
            }

            response->set_json_data(res->body);
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in getMarketTags: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }
};

class SeriesServiceImpl final : public polymarket::clob::series::Service {
public:
    grpc::Status listSeries(grpc::ServerContext* context, const polymarket::clob::ListSeriesRequest* request, polymarket::clob::ListSeriesResponse* response) override {
        try{
            httplib::SSLClient client("gamma-api.polymarket.com", 443);
            auto res = client.Get("/series");

            if(!res) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from API");
            }
            if (res->status != 200) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error: " + std::to_string(res->status));
            }

            response->set_json_data(res->body);
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in listSeries: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }

    grpc::Status getSeriesById(grpc::ServerContext* context, const polymarket::clob::GetSeriesByIdRequest* request, polymarket::clob::Series* response) override {
        try{
            httplib::SSLClient client("gamma-api.polymarket.com", 443);
            std::string path = "/series/" + std::to_string(request->id());
            auto res = client.Get(path.c_str());

            if(!res) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from API");
            }
            if (res->status != 200) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error: " + std::to_string(res->status));
            }

            response->set_json_data(res->body);
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in getSeriesById: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }
};

class SearchServiceImpl final : public polymarket::clob::search::Service {
public:
    grpc::Status publicSearch(grpc::ServerContext* context, const polymarket::clob::PublicSearchRequest* request, polymarket::clob::PublicSearchResponse* response) override {
        try{
            httplib::SSLClient client("gamma-api.polymarket.com", 443);
            std::string path = "/public-search?q=" + request->q();
            auto res = client.Get(path.c_str());

            if(!res) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "No response from API");
            }
            if (res->status != 200) {
                return grpc::Status(grpc::StatusCode::UNAVAILABLE, "HTTP error: " + std::to_string(res->status));
            }

            response->set_json_data(res->body);
            return grpc::Status::OK;

        } catch (const std::exception& e) {
            std::cerr << "Exception in publicSearch: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }
};

class AuthServiceImpl final : public databaselayer::auth::auth::Service {
private:
    DatabaseService databaseService;

public:
    AuthServiceImpl() : databaseService("localhost", "admin", "PolyTerminal", "PolyTerminal") {}
        grpc::Status signup(grpc::ServerContext* context, const databaselayer::auth::Credentials* request, databaselayer::auth::SignupStatus* response) override {
            try {
                if (databaseService.SignUp(request->email(), request->password())) {
                    response->set_statuscode(200);
                    response->set_statusmsg("Signup successful");
                    std::cout << "[signup] Signup successful" << std::endl;
                    return grpc::Status::OK;
                } else {
                    response->set_statuscode(500);
                    response->set_statusmsg("Signup failed");
                    std::cout << "[signup] Signup failed" << std::endl;
                    return grpc::Status(grpc::StatusCode::INTERNAL, "Signup failed");
                }
            } catch (const std::exception& e) {
                std::cerr << "Exception in Signup: " << e.what() << std::endl;
                std::cout << "[signup] Exception: " << e.what() << std::endl;
                return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
            }
        }

        grpc::Status login(grpc::ServerContext* context, const databaselayer::auth::Credentials* request, databaselayer::auth::LoginStatus* response) override {
            try {
                if (databaseService.LoginUser(request->email(), request->password())) {
                    response->set_statuscode(200);
                    response->set_statusmsg("Login successful");
                    std::cout << "[login] Login successful" << std::endl;
                    return grpc::Status::OK;
                } else {
                    response->set_statuscode(401);
                    response->set_statusmsg("Login failed");
                    std::cout << "[login] Login failed" << std::endl;
                    return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Login failed");
                }
            } catch (const std::exception& e) {
                std::cerr << "Exception in Login: " << e.what() << std::endl;
                std::cout << "[login] Exception: " << e.what() << std::endl;
                return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
            }
        }
};

std::string urlEncode(const std::string& str) {
    std::string result;
    for (unsigned char c : str) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            result += c;
        } else {
            result += '%';
            result += "0123456789ABCDEF"[c >> 4];
            result += "0123456789ABCDEF"[c & 0x0F];
        }
    }
    return result;
}

// Helper function to URL decode
std::string urlDecode(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '+') {
            result += ' ';
        } else if (str[i] == '%' && i + 2 < str.length()) {
            int value;
            std::istringstream is(str.substr(i + 1, 2));
            if (is >> std::hex >> value) {
                result += static_cast<char>(value);
                i += 2;
            } else {
                result += str[i];
            }
        } else {
            result += str[i];
        }
    }
    return result;
}

// Helper function to parse form data
std::map<std::string, std::string> parseFormData(const std::string& body) {
    std::map<std::string, std::string> params;
    std::istringstream iss(body);
    std::string pair;
    
    while (std::getline(iss, pair, '&')) {
        size_t pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = urlDecode(pair.substr(0, pos));
            std::string value = urlDecode(pair.substr(pos + 1));
            params[key] = value;
        }
    }
    return params;
}

// HTTP Server function
void RunHttpServer(DatabaseService* dbService) {
    httplib::Server svr;
    
    // Login endpoint
    svr.Post("/login", [dbService](const httplib::Request& req, httplib::Response& res) {
        std::cout << "[HTTP] Login request received" << std::endl;
        auto params = parseFormData(req.body);
        
        if (params.find("email") == params.end() || params.find("password") == params.end()) {
            res.status = 400;
            res.set_content("Missing email or password", "text/plain");
            return;
        }
        
        std::string email = params["email"];
        std::string password = params["password"];
        
        if (dbService->LoginUser(email, password)) {
            res.status = 200;
            res.set_content("Login successful", "text/plain");
            std::cout << "[HTTP] Login successful for: " << email << std::endl;
        } else {
            res.status = 401;
            res.set_content("Login failed", "text/plain");
            std::cout << "[HTTP] Login failed for: " << email << std::endl;
        }
    });
    
    // Signup endpoint
    svr.Post("/signup", [dbService](const httplib::Request& req, httplib::Response& res) {
        std::cout << "[HTTP] Signup request received" << std::endl;
        auto params = parseFormData(req.body);
        
        if (params.find("email") == params.end() || params.find("password") == params.end()) {
            res.status = 400;
            res.set_content("Missing email or password", "text/plain");
            return;
        }
        
        std::string email = params["email"];
        std::string password = params["password"];
        
        if (dbService->SignUp(email, password)) {
            res.status = 200;
            res.set_content("Signup successful", "text/plain");
            std::cout << "[HTTP] Signup successful for: " << email << std::endl;
        } else {
            res.status = 500;
            res.set_content("Signup failed", "text/plain");
            std::cout << "[HTTP] Signup failed for: " << email << std::endl;
        }
    });
    
    svr.Get("/status", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "[HTTP] Status request received" << std::endl;
        httplib::SSLClient client("data-api.polymarket.com", 443);
        auto apiRes = client.Get("/");
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status), "text/plain");
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
    });
    
    svr.Get("/teams", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "[HTTP] Teams request received" << std::endl;
        httplib::SSLClient client("gamma-api.polymarket.com", 443);
        auto apiRes = client.Get("/teams");
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status), "text/plain");
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
    });
    
    svr.Get("/sports", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "[HTTP] Sports metadata request received" << std::endl;
        httplib::SSLClient client("gamma-api.polymarket.com", 443);
        auto apiRes = client.Get("/sports");
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status), "text/plain");
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
    });
    
    svr.Get("/events", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "[HTTP] List events request received" << std::endl;
        httplib::SSLClient client("gamma-api.polymarket.com", 443);
        auto apiRes = client.Get("/events");
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status), "text/plain");
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
    });
    
    svr.Get("/events/:id", [](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        std::cout << "[HTTP] Get event by ID request received: " << id << std::endl;
        httplib::SSLClient client("gamma-api.polymarket.com", 443);
        std::string path = "/events/" + id;
        auto apiRes = client.Get(path.c_str());
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status), "text/plain");
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
    });
    
    svr.Get("/events/slug/:slug", [](const httplib::Request& req, httplib::Response& res) {
        std::string slug = req.matches[1];
        std::cout << "[HTTP] Get event by slug request received: " << slug << std::endl;
        httplib::SSLClient client("gamma-api.polymarket.com", 443);
        std::string path = "/events/slug/" + slug;
        auto apiRes = client.Get(path.c_str());
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status), "text/plain");
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
    });
    
    svr.Get("/events/:id/tags", [](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        std::cout << "[HTTP] Get event tags request received: " << id << std::endl;
        httplib::SSLClient client("gamma-api.polymarket.com", 443);
        std::string path = "/events/" + id + "/tags";
        auto apiRes = client.Get(path.c_str());
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status), "text/plain");
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
    });
    
    svr.Get("/markets", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "[HTTP] List markets request received" << std::endl;
        httplib::SSLClient client("gamma-api.polymarket.com", 443);
        auto apiRes = client.Get("/markets");
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status), "text/plain");
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
    });
    
    svr.Get("/markets/:id", [](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        std::cout << "[HTTP] Get market by ID request received: " << id << std::endl;
        httplib::SSLClient client("gamma-api.polymarket.com", 443);
        std::string path = "/markets/" + id;
        auto apiRes = client.Get(path.c_str());
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status), "text/plain");
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
    });
    
    svr.Get("/markets/slug/:slug", [](const httplib::Request& req, httplib::Response& res) {
        std::string slug = req.matches[1];
        std::cout << "[HTTP] Get market by slug request received: " << slug << std::endl;
        httplib::SSLClient client("gamma-api.polymarket.com", 443);
        std::string path = "/markets/slug/" + slug;
        auto apiRes = client.Get(path.c_str());
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status), "text/plain");
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
    });
    
    svr.Get("/markets/:id/tags", [](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        std::cout << "[HTTP] Get market tags request received: " << id << std::endl;
        httplib::SSLClient client("gamma-api.polymarket.com", 443);
        std::string path = "/markets/" + id + "/tags";
        auto apiRes = client.Get(path.c_str());
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status), "text/plain");
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
    });
    
    svr.Get("/series", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "[HTTP] List series request received" << std::endl;
        httplib::SSLClient client("gamma-api.polymarket.com", 443);
        auto apiRes = client.Get("/series");
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status), "text/plain");
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
    });
    
    svr.Get("/series/:id", [](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        std::cout << "[HTTP] Get series by ID request received: " << id << std::endl;
        httplib::SSLClient client("gamma-api.polymarket.com", 443);
        std::string path = "/series/" + id;
        auto apiRes = client.Get(path.c_str());
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status), "text/plain");
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
    });
    
    svr.Get("/search", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "[HTTP] Search request received" << std::endl;
        
        std::string query = req.get_param_value("q");
        if (query.empty()) {
            res.status = 400;
            res.set_content("Missing query parameter 'q'", "text/plain");
            return;
        }
        
        httplib::SSLClient client("gamma-api.polymarket.com", 443);
        client.set_follow_location(true);
        std::string encodedQuery = urlEncode(query);
        std::string path = "/public-search?q=" + encodedQuery;
        
        httplib::Headers headers = {
            {"Accept", "application/json"},
            {"User-Agent", "PolyTerminal/1.0"}
        };
        
        auto apiRes = client.Get(path.c_str(), headers);
        
        if (!apiRes) {
            res.status = 502;
            res.set_content("Failed to reach Polymarket API", "text/plain");
            return;
        }
        
        if (apiRes->status != 200) {
            res.status = apiRes->status;
            res.set_content("API error: " + std::to_string(apiRes->status) + " - " + apiRes->body, "text/plain");
            std::cout << "[HTTP] Search failed: " << apiRes->status << " - " << apiRes->body << std::endl;
            return;
        }
        
        res.status = 200;
        res.set_content(apiRes->body, "application/json");
        std::cout << "[HTTP] Search successful for: " << query << std::endl;
    });
    
    std::cout << "HTTP Server listening on 0.0.0.0:8888" << std::endl;
    svr.listen("0.0.0.0", 8888);
}

void RunServer() {
    // Create shared database service
    DatabaseService dbService("localhost", "admin", "PolyTerminal", "PolyTerminal");
    
    // Start HTTP server in a separate thread
    std::thread httpThread([&dbService]() {
        RunHttpServer(&dbService);
    });
    
    std::string grpc_address("0.0.0.0:8889");
    StatusServiceImpl status_service;
    SportsServiceImpl sports_service;
    EventsServiceImpl events_service;
    MarketsServiceImpl markets_service;
    SeriesServiceImpl series_service;
    SearchServiceImpl search_service;
    AuthServiceImpl   auth_service;

    ServerBuilder builder;
    builder.AddListeningPort(grpc_address, grpc::InsecureServerCredentials());
    
    // Register all services
    builder.RegisterService(&status_service);
    builder.RegisterService(&sports_service);
    builder.RegisterService(&events_service);
    builder.RegisterService(&markets_service);
    builder.RegisterService(&series_service);
    builder.RegisterService(&search_service);
    builder.RegisterService(&auth_service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "gRPC Server listening on " << grpc_address << std::endl;

    // Wait for the server to shutdown
    server->Wait();
    httpThread.join();
}

int main(int argc, char** argv) {
    RunServer();
    return 0;
}
