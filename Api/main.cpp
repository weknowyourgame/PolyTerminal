#include <iostream>
#include <memory>
#include <string>
#include <grpc++/grpc++.h>
#include <httplib.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
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
            std::string path = "/search?q=" + request->q();
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
                    return grpc::Status::OK;
                } else {
                    response->set_statuscode(500);
                    response->set_statusmsg("Signup failed");
                    return grpc::Status(grpc::StatusCode::INTERNAL, "Signup failed");
                }
            } catch (const std::exception& e) {
                std::cerr << "Exception in Signup: " << e.what() << std::endl;
                return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
            }
        }

        grpc::Status login(grpc::ServerContext* context, const databaselayer::auth::Credentials* request, databaselayer::auth::LoginStatus* response) override {
            try {
                if (databaseService.LoginUser(request->email(), request->password())) {
                    response->set_statuscode(200);
                    response->set_statusmsg("Login successful");
                    return grpc::Status::OK;
                } else {
                    response->set_statuscode(401);
                    response->set_statusmsg("Login failed");
                    return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Login failed");
                }
            } catch (const std::exception& e) {
                std::cerr << "Exception in Login: " << e.what() << std::endl;
                return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
            }
        }
};

void RunServer() {
    std::string server_address("0.0.0.0:8888");
    StatusServiceImpl status_service;
    SportsServiceImpl sports_service;
    EventsServiceImpl events_service;
    MarketsServiceImpl markets_service;
    SeriesServiceImpl series_service;
    SearchServiceImpl search_service;
    AuthServiceImpl   auth_service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    
    // Register all services
    builder.RegisterService(&status_service);
    builder.RegisterService(&sports_service);
    builder.RegisterService(&events_service);
    builder.RegisterService(&markets_service);
    builder.RegisterService(&series_service);
    builder.RegisterService(&search_service);
    builder.RegisterService(&auth_service);

    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();
    return 0;
}