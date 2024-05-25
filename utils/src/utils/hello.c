#include <utils/hello.h>

void decir_hola(char* quien) {
    printf("Hola desde %s!!\n", quien);
}
/////////////////////////////////////////////////////////Funciones Servidor
int crear_conexion_servidor(char* puerto)
{
	int socket_servidor;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	// preguntar si la direccion en NULL esta bien
	int rv = getaddrinfo(NULL, puerto, &hints, &servinfo);
    if (rv != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(rv));
        return EXIT_FAILURE;
    }
	socket_servidor = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol);
	if (socket_servidor == -1) {	
        printf("error al crear socket \n%s", strerror(errno));
        return EXIT_FAILURE;
    }
	int bindVal = bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
    if (bindVal == -1) {
        printf("error de bind mismatch\n%s", strerror(errno));
        return EXIT_FAILURE;
    }
	int listenVal = listen(socket_servidor, SOMAXCONN);
    if (listenVal == -1) {
        printf("error de listen \n%s", strerror(errno));
        return EXIT_FAILURE;
    }
    freeaddrinfo(servinfo);
	
	return socket_servidor;
}
int esperar_cliente(int socket_servidor, t_log* logger)
{
	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);
	log_info(logger, "Conexion recibida");

	return socket_cliente;
}
/////////////////////////////////////////////////////////Funciones cliente
int crear_conexion_cliente(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family,server_info->ai_socktype,server_info->ai_protocol);
                         
	// Ahora que tenemos el socket, vamos a conectarlo
	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return socket_cliente;
}
