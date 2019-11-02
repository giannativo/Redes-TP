#include <iostream>
#include <winsock2.h>

using namespace std;

class Client{
public:
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

    char buffer[1024];

    Client()
    {
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
        exit(0);
        }

        cout << "Conectado con exito al server..." << endl;

    }

    void Enviar()
    {
        cout << "Escribe el mensaje a enviar: " << endl;
        cin.getline(this->buffer,1024);

        string var(buffer);
        string substring=var.substr(0,3);

        if(substring=="(M)" || substring=="(m)" ){
        send(server, buffer, sizeof(buffer), 0);
        cout << "Mensaje enviado!" << endl;

        }else if(substring=="(T)"|| substring=="(t)" ){

        }else if(substring=="(F)"|| substring=="(f)" ){
        send(server, buffer, sizeof(buffer), 0);
        cout << "Mensaje enviado!" << endl;
        }else{
        cout << "Error de comando!" << endl;
        memset(buffer, 0, sizeof(buffer));
        cin.ignore();
        Enviar();
        }
        memset(buffer, 0, sizeof(buffer));
    }

    void Recibir()
    {
         recv(server, buffer, sizeof(buffer), 0);
         cout << "El servidor dice: " << buffer << endl;

         string varRespuesta(buffer);
         string substringRespuesta=varRespuesta.substr(0,3);

         if(substringRespuesta=="(F)"|| substringRespuesta=="(f)"){
            CerrarSocket();
         }
         memset(buffer, 0, sizeof(buffer));
    }

    void CerrarSocket()
    {
        cout << "Se verifico una solicitud de finalizacion de la comunicacion. Se procedera a cerrar la aplicacion" << endl;
        cout << "Servidor Desconectado" << endl;
        system("pause"); // aca habria que ponerle tipo un sleep en ves de un pause, pero conla funcion sleep no funciona
        closesocket(server);
        WSACleanup();
        exit(0);
    }
};

int main()
{
    Client *Cliente = new Client();
    while(true)
    {
        Cliente->Enviar();
        Cliente->Recibir();
    }
}
