#include"server_box.h"

int server_box::POST_RECV(LPIO_DATA io_data)
{

	LPHead_code temp = (LPHead_code)&io_data->Buffer;
	temp.
	

	send(io_data->socket, "success recv", 13, 0);

	Basic_Server::POST_RECV(io_data);
	return 0;
}


