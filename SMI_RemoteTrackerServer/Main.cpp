#include "Server.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif

char* crossbarAddress = "127.0.0.1:8080";
char* retID = "remote_tracker1";

void WaitForUserInteraction(Server* server) {

	// Wait for data to arrive via callback
	printf("Press 'Q' to exit\n");

	int key = -1;

	bool connectionWarning = false;

	// while key is not 'Escape'
	while (key != 0x1B && key != 'q' &&  key != 'Q')
	{
		if (!_kbhit())
		{
			// no key pressed, just wait.
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		//Setup user options here
		key = _getch();
		if (key == 'q' || key == 'Q')
		{
			printf("Waiting for the server to stop\n");
			server->StopServer();
		}
	}

	return;
}



void parseCommandLine(int argc, char ** argv) {
	// check each parameter
	for (int i = 1; i < argc; i++) {

		if (argv[i][0] == '-') {
			if (0 == strncmp(argv[i], "--crossbarAddress", MAX(strlen(argv[i]), strlen("--crossbarAddress")))) {

				if (argc <= ++i) {
					fprintf(stderr, "ERROR: missing argument for parameter '--crossbarAddress'");
				}

				crossbarAddress = argv[i];
				continue;
			}

			if (0 == strncmp(argv[i], "--remoteTrackerID", MAX(strlen(argv[i]), strlen("--remoteTrackerID")))) {

				if (argc <= ++i) {
					fprintf(stderr, "ERROR: missing argument for parameter '--remoteTrackerID'");
				}

				retID = argv[i];
				continue;
			}

			fprintf(stderr, "ERROR: unknown parameter: %s", argv[i]);
		}
	}
}

int main(int argc, char *argv[], char* OtherParameters[])
{
	parseCommandLine(argc, argv);

	bool running = true;

	Server* server = new Server("127.0.0.1", 4444, "127.0.0.1", 5555, crossbarAddress, retID);
	std::thread SMIServerThread(&Server::Run, server);

	WaitForUserInteraction(server);

	//Wait for the thread to finish
	SMIServerThread.join();

	printf("Server stopped, press any key to exit\n");
	_getch();

	delete server;
}