#include "ipc_nano.h"

int main()
{
	char *buf="hello,world.";
	send_ipc(buf);

	return 0;
}
