#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <stdio.h>

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
        cout << "Ingrese el numero de direccion IP en el que desea inciar el servidor" << endl;
        cin.ignore();
        cin >> ip;


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
        fflush(stdin);
        scanf("%[^\n]",this->buffer);

        string var(buffer);
        string substring=var.substr(0,3);

        if(substring=="(M)" || substring=="(m)" ){
            send(server, buffer, sizeof(buffer), 0);
            cout << "Mensaje enviado!" << endl;
            memset(buffer, 0, sizeof(buffer));
        }
        else if(substring=="(T)"|| substring=="(t)" ){
            string ruta=var.substr(3);
            FILE *File;
            char *Buffer;
            unsigned long Size;
            char nombre[512]= "\0";
            char rutaChar[1024] = "\0";
            cout<<"Comenzando transferencia de archivo..."<<endl;
            string mensaje = "(T)Enviando archivo...";
            cout<<mensaje<<endl;
            send(server, buffer, sizeof(buffer), 0);

            strcpy(nombre, &ruta[0]);
            Envio(this->server, nombre, sizeof(nombre));
            strcpy(rutaChar, &ruta[0]);
            File = fopen(rutaChar, "rb");

                if(!File){
                    cout<<"ERROR AL LEER EL ARCHIVO"<<endl;
                    cin.ignore();
                }
                else{
                    fseek(File, 0, SEEK_END);
                    Size = ftell(File);
                    fseek(File, 0, SEEK_SET);
                    Buffer = new char[Size];
                    fread(Buffer, Size, 1, File);
                    char cSize[MAX_PATH];
                    sprintf(cSize, "%lu", Size);
                    fclose(File);
                    Sleep(1000);
                    Envio(this->server, cSize, MAX_PATH);
                    Sleep(1000);
                    Envio(this->server,Buffer,Size);
                }
            free(Buffer);
        }
        else if(substring=="(F)"|| substring=="(f)" ){
            send(server, buffer, sizeof(buffer), 0);
            cout << "Mensaje enviado!" << endl;
        }
        else{
            cout << "Error de comando!" << endl;
            memset(buffer, 0, sizeof(buffer));
            Enviar();
        }
        memset(buffer, 0, sizeof(buffer));
    }

    void Recibir()
    {
        recv(server, buffer, sizeof(buffer), 0);

        string varRespuesta(buffer);
        string substringRespuesta=varRespuesta.substr(0,3);
        string mensaje=varRespuesta.substr(3);
        if(substringRespuesta=="(T)"|| substringRespuesta=="(t)"){
            cout<<"Se recibio solicitud de transferencia de archivo..."<<endl;
            cout<<"Recibiendo archivo..."<<endl;
            int Size;
            char Filesize[MAX_PATH] = "\0";
            char nombre[512] = "\0";

            recv(this->server, nombre, sizeof(nombre), 0);

                if(recv(this->server, Filesize, sizeof(Filesize), 0)){
                    Size = atoi((const char*)Filesize);
                    cout<< "Longitud del archivo: "<< Size<<endl;
                }

            char *Buffer = new char[Size];
            recv(this->server, Buffer, Size, 0);
            cout<< "Se recibio el archivo: "<< nombre <<endl;
            char new_name[50];

            cout<<"Ingresar nombre y extension del archivo a guardar: "<<endl;
            scanf("%49s",new_name);

            FILE* File;
            File = fopen(new_name, "wb");
            fwrite(Buffer, 1, Size, File);
            fclose(File);

            cout<<"Archivo recibido con exito!"<<endl;
            free(Buffer);
        }
        if(substringRespuesta=="(F)"|| substringRespuesta=="(f)"){
            CerrarSocket();
        }
        cout << "El servidor dice: " << mensaje << endl;

        memset(buffer, 0, sizeof(buffer));
    }

    int Envio(SOCKET socket, char* buffer, int longitud_buffer)
    {
        int bytes_a_transferir = 0;
        int totalenviado = 0;
        int bytes_faltantes = longitud_buffer;
        while(bytes_a_transferir != longitud_buffer){
            bytes_a_transferir = send(socket, &buffer[totalenviado], bytes_faltantes, 0);
            if(bytes_a_transferir == SOCKET_ERROR){
                return SOCKET_ERROR;
            }
            totalenviado = totalenviado + bytes_a_transferir;
            bytes_faltantes = bytes_a_transferir - totalenviado;
        }
        return 0;
    }

    void CerrarSocket()
    {
        cout << "Se verifico una solicitud de finalizacion de la comunicacion. Se procedera a cerrar la aplicacion" << endl;
        cout << "Servidor Desconectado" << endl;
        system("pause");
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
