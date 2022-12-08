#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include <winsock2.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <thread>

class Client{

    SOCKET m_socket;

    std::string m_name;

    std::string m_server_ip;

    uint32_t m_server_port;

    bool m_is_server_active;

    public:

        Client():m_is_server_active{true}{

            init_winsock();

            create_client_socket();

        }

        void init_winsock();

        void create_client_socket();

        void connect_to(const std::string &ip,const uint32_t &port);

        void send_computer_name();

        void send_message();

        void get_message(const SOCKET &socket_for_thread,const bool &is_server_active_for_thread);

        void get_message_thread_function_thread();

};

#endif