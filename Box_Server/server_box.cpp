#include"server_box.h"

int server_box::POST_RECV(LPIO_DATA io_data)
{
	send(io_data->socket, "success recv", 13, 0);

	Basic_Server::POST_RECV(io_data);
	return 0;
}
