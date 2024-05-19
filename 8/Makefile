all: Client Server
Client: ./client/Common.h ./client/TCPClient.c ./client/ChillInGallery.c ./client/CreateTCPClientSocket.c ./common/DieWithError.c
	gcc -pthread ./client/TCPClient.c ./client/ChillInGallery.c ./client/CreateTCPClientSocket.c ./common/DieWithError.c -o TCPClient
Server: ./server/Common.h ./server/TCPServer.c ./server/AcceptTCPConnection.c ./server/CreateTCPServerSocket.c ./server/HandleTCPClient.c ./common/DieWithError.c
	gcc ./server/TCPServer.c ./server/AcceptTCPConnection.c ./server/CreateTCPServerSocket.c ./server/HandleTCPClient.c ./common/DieWithError.c -o TCPServer
