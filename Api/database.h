#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <iostream>
#include <memory>

namespace sql {
    class Driver;
    class Connection;
}

class DatabaseService {
private:
    sql::Driver* driver;
    sql::Connection* connection;

public:
    DatabaseService(const std::string& host, const std::string& user, const std::string& password, const std::string& database);
    ~DatabaseService();

    bool LoginUser(const std::string& email, const std::string& password);
    bool SignUp(const std::string& email, const std::string& password);
};

#endif // DATABASE_H
