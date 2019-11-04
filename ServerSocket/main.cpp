#include <iostream>
#include <winsock2.h>
#include <stdio.h>

using namespace std;

class Server{
public:
    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN serverAddr, clientAddr;
    char buffer[1024];
    Server()
    {
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

        bool primerConexion=true;
        int clientAddrSize = sizeof(clientAddr);

        if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
        {
            if(primerConexion==true){
            cout << "Se recibio una solicitud de conexion" << endl;
            cout << "Cliente conectado!!" << endl;
            }
            primerConexion=false;
        }
    }

    void Recibir()
    {

       recv(client, buffer, sizeof(buffer), 0);

       string varRespuesta(buffer);
       string substringRespuesta=varRespuesta.substr(0,3);
       string mensaje=varRespuesta.substr(3);
       cout << "El cliente dice: " << mensaje << endl;

            if(substringRespuesta=="(F)"|| substringRespuesta=="(f)"){
                CerrarSocket();
            }
      memset(buffer, 0, sizeof(buffer));
    }
    void Enviar()
    {
        cout << "Escribe el mensaje a enviar: " << endl;
        fflush(stdin);
        scanf("%[^\n]",this->buffer);

        string var(buffer);
        string substring=var.substr(0,3);

        if(substring=="(M)"|| substring=="(m)" ){
        send(client, buffer, sizeof(buffer), 0);
        cout << "Mensaje enviado!" << endl;

        }else if(substring=="(T)"|| substring=="(t)" ){

        }else if(substring=="(F)"|| substring=="(f)" ){
        send(client, buffer, sizeof(buffer), 0);
        cout << "Mensaje enviado!" << endl;
        }else{
        cout << "Error de comando!" << endl;
        memset(buffer, 0, sizeof(buffer));
        Enviar();
        }
        memset(buffer, 0, sizeof(buffer));
    }
    void CerrarSocket()
    {
       cout << "Se verifico una solicitud de finalizacion de la comunicacion. Se procedera a cerrar la aplicacion" << endl;
       cout << "Cliente Desconectado" <<endl;
       closesocket(client);
       exit(0);
    }
};


int main()
{
  Server *Servidor = new Server();
  while(true)
  {
     Servidor->Recibir();
     Servidor->Enviar();
  }
}
