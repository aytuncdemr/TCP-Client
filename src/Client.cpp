#include "Client.hpp"

void Client::init_winsock(){

    std::cout << "[Initializing winsock]" << std::endl;

    WSADATA ws_data;

    int result = WSAStartup(MAKEWORD(2,2),&ws_data);

    if(result != -1)
        std::cout << "[Done]" << std::endl;
    else
        std::cout << "Error [Code:" << WSAGetLastError() << "]" << std::endl;

}

void Client::create_client_socket(){
    
    std::cout << "[Creating client socket]" << std::endl;

    m_socket = socket(AF_INET,SOCK_STREAM,0);

    if(m_socket != INVALID_SOCKET)
        std::cout << "[Done]" << std::endl;
    else
        std::cout << "Error [Code:" << WSAGetLastError() << "]" << std::endl;

}

void Client::connect_to(const std::string &ip,const uint32_t &port){

    std::cout << "[Client socket connecting to " << ip << ":" << port << "]" << std::endl;

    m_server_port = port;

    m_server_ip = ip;

    struct sockaddr_in server_addr;

    server_addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(port);

    int result = connect(m_socket,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in));

    if(result != -1)
        std::cout << "[Client successfully connected to the server]" << std::endl;
    else
        std::cout << "Error [Code:" << WSAGetLastError() << "]" << std::endl;
    
    send_computer_name();

    send_message();

}

void Client::send_computer_name(){

    char client_name[256];

    gethostname(client_name,256);

    std::cout << "Computer name: " << client_name << std::endl; 
    
    int result = send(m_socket,client_name,256,0);

    if(result == -1)
        std::cout << "Error [Code:" << WSAGetLastError() << "]" << std::endl;

}

void Client::send_message(){
   
    get_message_thread_function_thread();
    
    while(m_is_server_active){

        std::string message;

        std::getline(std::cin,message);

        int result = send(m_socket,message.c_str(),256,0);

        if(result == -1){
        
            std::cout << "Error [Code:" << WSAGetLastError() << "]" << std::endl;
        
            m_is_server_active = false;
            
        }

    }
    
}

void Client::get_message(const SOCKET &socket_for_thread,const bool &is_server_active_for_thread){

    while(is_server_active_for_thread){

        char data[256];

        int result1 = recv(socket_for_thread,data,256,0);

        std::string data_string {data};

        if(data_string == "file_sending"){

            char file_name[256],file_length[256];

            int result = recv(socket_for_thread,file_name,256,0);

            if(result != -1){

                int result2 = recv(socket_for_thread,file_length,256,0);

                if(result2 != -1){

                    std::cout << "[Server is sending " << file_name << " file that has " << file_length << " bytes" << "]" << std::endl;

                    uint64_t file_int_length = atoi(file_length);

                    uint64_t recvied_length {0};

                    std::fstream file(file_name,std::ios::out |std::ios::binary);

                    while(file_int_length > recvied_length){

                        char file_data[256];

                        int r = recv(socket_for_thread,file_data,256,0);

                        std::string file_data_string {file_data};

                        recvied_length += file_data_string.length() + 1;

                        file << file_data_string << std::endl;

                        std::cout << "Progress [%" << std::setprecision(2) << static_cast<int> (static_cast<double>(recvied_length) / file_int_length * 100) << "]"; 

                        std::cout << "\r"; // <- clearing current line

                    }

                    file.close();

                    std::cout << "[File is successfully recvied]" << std::endl;

                }

            }


        }   
        else if(data_string == "message_sending"){

            char message_data[256];

            int result = recv(socket_for_thread,message_data,256,0);

            if(result != -1)
                std::cout << "Server [" << message_data << "]" << std::endl;

        }
        else
            std::cout << "[Invalid data type]" << std::endl;

    }

}

void Client::get_message_thread_function_thread(){

    std::thread get_message_thread_function_variable(&Client::get_message,this,m_socket,m_is_server_active);

    get_message_thread_function_variable.detach();
    
}