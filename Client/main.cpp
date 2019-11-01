#include <iostream>
#include <winsock2.h>

using namespace std;

int main()
{
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(PF_INET, SOCK_STREAM, 0);

    int puerto;
    string ip;
    cout << "Ingrese el numero de puerto en el que desea inciar el servidor" << endl;
    cin >> puerto;
    cin.ignore();
    cout << "Ingrese el numero de direccion IP en el que desea inciar el servidor" << endl;
    cin >> ip;
    cin.ignore();

    addr.sin_addr.s_addr = inet_addr(ip.data());
    addr.sin_family = AF_INET;
    addr.sin_port = htons(puerto);

    int conector= connect(server, (SOCKADDR *)&addr, sizeof(addr));
    cout << "Conectando al servidor..." << endl;

    if(conector!=0){
    cout << "Error de conexion" << " Ip:" << ip <<" Puerto:"<<puerto << endl;
    closesocket(server);
    WSACleanup();
    cout << "Socket cerrado" << endl << endl;
    exit(0);
    }

    cout << "Conectado con exito al server..." << endl;

    int conexion=1;
    char buffer[1024]={};

    while(conexion!=0){

    cout << "Escribe un mensaje: " << endl;
    cin.getline(buffer,1024);

    string var(buffer);
    string substring=var.substr(0,3);

        if(substring=="(M)"){
        //llamo funcion de envio de mensaje y saco las proximas dos lineas
        send(server, buffer, sizeof(buffer), 0);
        cout << "Mensaje enviado!" << endl;
        }else if(substring=="(T)"){
        //llamo funcion de transferencia
        }else if(substring=="(F)"){
         send(server, buffer, sizeof(buffer), 0);
         conexion= closesocket(server);
         WSACleanup();
         cout << "Se recibio una solicitud de finalizacion de la comunicacion. Se procedera a cerrar la aplicacion" << endl;
         cout << "Servidor Desconectado" << endl;
        }
        memset(buffer, 0, sizeof(buffer));
    }
}
