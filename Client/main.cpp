#include <iostream>
#include <winsock2.h>

using namespace std;

void send_message(SOCKET server, char *message){
    send(server, message, sizeof(message), 0);
    cout << "Message sent!" << endl;
}

int main()
{
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // replace the ip with your futur server ip address. If server AND client are running on the same computer, you can use the local ip 127.0.0.1
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr));
    cout << "Connected to server!" << endl;

    char buffer[1024]={};
    cout << "Write message: " << endl;
    cin.getline(buffer,1024);
    send_message(server, buffer);
    cout << "Message sent!" << endl;
    closesocket(server);
    WSACleanup();
    cout << "Socket closed." << endl << endl;
}
