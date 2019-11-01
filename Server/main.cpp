#include <iostream>
#include <winsock2.h>

using namespace std;

int main()
{
    WSADATA WSAData;

    SOCKET server, client;

    SOCKADDR_IN serverAddr, clientAddr;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    int puerto;
    cout << "Ingrese el numero de puerto en el que desea inciar el servidor" << endl;
    cin >> puerto;

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(puerto);

    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server, 0);
    cout <<"El servidor se inicio correctamente y esta esperando solicitudes en el puerto:"<< puerto << endl;
    cout << "Esperando solicitud de conexion...." << endl;

    char buffer[1024];
    int conexion=1;
    bool primerConexion=true;
    int clientAddrSize = sizeof(clientAddr);



    if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET){
        while(conexion!=0){
            if(primerConexion==true){
            cout << "Se recibio una solicitud de conexion" << endl;
            cout << "Cliente conectado!!" << endl;
            }
            primerConexion=false;
            recv(client, buffer, sizeof(buffer), 0);
            cout << "El cliente dice: " << buffer << endl;

            string var(buffer);
            string substring=var.substr(0,3);
            memset(buffer, 0, sizeof(buffer));

            if(substring=="(F)"){
            conexion= closesocket(client);
            cout << "Se envio una solicitud de finalizacion de la comunicacion. Se procedera a cerrar la aplicacion" << endl;
            cout << "Cliente Desconectado" <<endl;
            }
        }
    }
}
