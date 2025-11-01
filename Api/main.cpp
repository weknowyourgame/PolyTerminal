#include <iostream>
#include <memory>
#include <string>
#include <grpc++/grpc++.h>
#include <httplib.h>

#include "polymarket_clob.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class StatusServiceImpl final : public polymarket::clob::status::Service {
public:
    grpc::Status CheckStatus(grpc::ServerContext* context, const polymarket::clob::Empty* request, polymarket::clob::ClobApiStatus* response) override {
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
            std::cout << "[CheckStatus] Called," << res->body << std::endl;
            return grpc::Status::OK;
        } catch (const std::exception& e) {
            std::cerr << "Exception in CheckStatus: " << e.what() << std::endl;
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());}
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:8888");
    StatusServiceImpl status_service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&status_service);
    
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