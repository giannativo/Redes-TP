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
        if(substringRespuesta=="(M)"|| substringRespuesta=="(m)"){
            cout << "El cliente dice: " << mensaje << endl;
        }
        else if(substringRespuesta=="(F)"|| substringRespuesta=="(f)"){
            CerrarSocket();
        }
        else if(substringRespuesta=="(T)"|| substringRespuesta=="(t)"){
            cout<<"Recibiendo archivo..."<<endl;
            int Size;
            char Filesize[MAX_PATH] = "\0";
            char nombre[512] = "\0";

            recv(this->client, nombre, sizeof(nombre), 0);

            if(recv(this->client, Filesize, sizeof(Filesize), 0)){
                Size = atoi((const char*)Filesize);
                cout<< "Longitud del archivo: "<< Size<<endl;
            }

            char *Buffer = new char[Size];
            recv(this->client, Buffer, Size, 0);
            cout<< "Se trajo el archivo: "<< nombre <<endl;
            char new_name[50];

            cout<<"Ingresar nombre y extension del archivo a guardar: "<<endl;
            scanf("%49s",new_name);

            FILE* File;
            File = fopen(new_name, "wb");
            fwrite(Buffer, 1, Size, File);
            fclose(File);

            cout<<"Archivo recibido!"<<endl;
            free(Buffer);
        }
        else{
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

        }
        else if(substring=="(T)"|| substring=="(t)" ){
            string ruta=var.substr(3);
            FILE *File;
            char *Buffer;
            unsigned long Size;
            char nombre[512]= "\0";
            char rutaChar[1024] = "\0";
            string mensaje = "(T)Enviando archivo...";
            cout<<mensaje<<endl;
            send(server, buffer, sizeof(buffer), 0);

            strcpy(nombre, &ruta[0]);
            SafeSend(this->server, nombre, sizeof(nombre));
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
                SafeSend(this->server, cSize, MAX_PATH);
                Sleep(1000);
                SafeSend(this->server,Buffer,Size);
            }
            free(Buffer);
        }
        else if(substring=="(F)"|| substring=="(f)" ){
            send(client, buffer, sizeof(buffer), 0);
            cout << "Mensaje enviado!" << endl;
        }
        else{
            cout << "Error de comando!" << endl;
            memset(buffer, 0, sizeof(buffer));
            Enviar();
        }
        memset(buffer, 0, sizeof(buffer));
    }
    int SafeSend(SOCKET s, char* buf, int buflen)
    {
	int sendlen = 0;
	int totalsend = 0;
	int remaining = buflen;

	while(sendlen != buflen)
	{
		sendlen = send(s, &buf[totalsend], remaining, 0);

		if(sendlen == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}

		totalsend = totalsend + sendlen;
		remaining = sendlen - totalsend;
	}
    return 0;
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
