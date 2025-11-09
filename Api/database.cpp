#include "database.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
// #include <bcrypt/BCrypt.hpp>

DatabaseService::DatabaseService(const std::string& host, const std::string& user, const std::string& password, const std::string& database) {
    driver = get_driver_instance();
    connection = driver->connect(host, user, password);
    connection->setSchema(database);
}

DatabaseService::~DatabaseService() {
    delete connection;
}

bool DatabaseService::LoginUser(const std::string& email, const std::string& password) {
    try {
        // TODO: Implement password hashing
        std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement("SELECT id, username FROM users WHERE email = ? AND password = ?"));

        pstmt->setString(1, email);
        pstmt->setString(2, password);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            int userId = res->getInt("id");
            std::string username = res->getString("username");
            return true;
        } else {
            return false;
        }

    } catch (sql::SQLException &e) {
        std::cerr << "Login error: " << e.what() << std::endl;
        return false;
    }
}


bool DatabaseService::SignUp(const std::string &email, const std::string &password) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement("INSERT INTO users (username, email, password) VALUES (NULL, ?, ?)"));
        pstmt->setString(1, email);
        pstmt->setString(2, password);
        pstmt->execute();
        return true;
    } catch (sql::SQLException &e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}