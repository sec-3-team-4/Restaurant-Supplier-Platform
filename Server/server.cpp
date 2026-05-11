#include "server.h"
#include "UserManager.h"
#include "JsonValidator.h"
#include "MessageRouter.h"

#include <iostream>
#include <array>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

using boost::asio::ip::tcp;

// ===== Managers =====
UserManager userManager;
JsonValidator validator;
MessageRouter router;

Server::Server(boost::asio::io_context& io, int port)
    : acceptor_(io, tcp::endpoint(tcp::v4(), port))
{
    std::cout << "Server started on port " << port << std::endl;
    startAccept();
}

// ===== Accept new clients =====
void Server::startAccept()
{
    auto socket = std::make_shared<tcp::socket>(acceptor_.get_executor());

    acceptor_.async_accept(*socket,
        [this, socket](boost::system::error_code ec)
        {
            if (!ec)
            {
                std::cout << "\nClient connected" << std::endl;
                handleClient(socket);
            }

            startAccept();
        });
}

// ===== Handle each client =====
void Server::handleClient(std::shared_ptr<tcp::socket> socket)
{
    auto buffer = std::make_shared<std::array<char, 1024>>();

    socket->async_read_some(
        boost::asio::buffer(*buffer),
        [this, socket, buffer](boost::system::error_code ec, std::size_t length)
        {
            if (ec)
            {
                std::cout << "Client disconnected" << std::endl;
                return;
            }

            std::string msg(buffer->data(), length);

            std::cout << "\nReceived:\n" << msg << std::endl;

            // validate JSON
            if (!validator.isValid(msg))
            {
                std::cout << "INVALID MESSAGE\n";
                handleClient(socket);
                return;
            }

            std::cout << "VALID MESSAGE\n";

<<<<<<< Updated upstream
            std::string result = router.route(msg);
            std::cout << result << std::endl;

=======
>>>>>>> Stashed changes
            try
            {
                auto json = nlohmann::json::parse(msg);

                std::string type = json.value("type", "");
                std::string sender = json.value("sender", "");
                std::string receiver = json.value("receiver", "");

                std::string text = "";
<<<<<<< Updated upstream

                if (json.contains("data") &&
                    json["data"].contains("text"))
                {
=======
                if (json.contains("data") && json["data"].contains("text"))
>>>>>>> Stashed changes
                    text = json["data"]["text"];

<<<<<<< Updated upstream
                std::cout << "Type: " << type << std::endl;
                std::cout << "Sender: " << sender << std::endl;
                std::cout << "Receiver: " << receiver << std::endl;
=======
                // ===== CLEAN OUTPUT (THIS IS WHAT YOU WANTED) =====
                std::cout << "\nType: " << type << std::endl;
                std::cout << "Sender: " << sender << std::endl;
                std::cout << "Receiver: " << receiver << std::endl;

                if (!text.empty())
                {
                    std::cout << "Message: " << text << std::endl;
                }
>>>>>>> Stashed changes

                if (!text.empty())
                {
                    std::cout << "Message: " << text << std::endl;
                }

                // LOGIN
                if (type == "login_request")
                {
                    userManager.addUser(sender);
                    clients[sender] = socket;

<<<<<<< Updated upstream
                    std::cout << sender << " logged in" << std::endl;
                }

                // LOGOUT
                if (type == "logout")
=======
                    std::cout << "\nLOGIN SUCCESS: " << sender << std::endl;

                    nlohmann::json response;
                    response["type"] = "login_response";
                    response["status"] = "success";
                    response["message"] = "Login successful";

                    std::string reply = response.dump() + "\n";

                    boost::asio::write(*socket, boost::asio::buffer(reply));

                    std::cout << "Login response sent\n";
                }

                // ================= LOGOUT =================
                else if (type == "logout")
>>>>>>> Stashed changes
                {
                    userManager.removeUser(sender);
                    clients.erase(sender);

                    std::cout << "LOGOUT: " << sender << std::endl;
                }

<<<<<<< Updated upstream
                // CHAT MESSAGE
                if (type == "chat_message")
=======
                // ================= CHAT =================
                else if (type == "chat_message")
>>>>>>> Stashed changes
                {
                    std::cout << "\nCHAT from " << sender << " to " << receiver << std::endl;

                    if (clients.find(receiver) != clients.end())
                    {
                        std::string forwardMsg =
                            sender + ": " + text;

                        boost::asio::write(
                            *clients[receiver],
<<<<<<< Updated upstream
                            boost::asio::buffer(forwardMsg));
=======
                            boost::asio::buffer(forwardMsg + "\n")
                        );
>>>>>>> Stashed changes

                        std::cout << "Message forwarded successfully to "
                                  << receiver << std::endl;
                    }
                    else
                    {
                        std::cout << "Receiver not online: "
                                  << receiver << std::endl;
                    }
                }

<<<<<<< Updated upstream
                // ONLINE CHECK
=======
                // ===== USER STATUS =====
>>>>>>> Stashed changes
                if (userManager.isOnline(sender))
                {
                    std::cout << sender << " is ONLINE" << std::endl;
                }
            }
            catch (...)
            {
                std::cout << "JSON parsing error" << std::endl;
            }

            handleClient(socket);
        });
}
