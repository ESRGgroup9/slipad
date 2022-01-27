
int main(int argc, char *argv[])
{
	server(port);

	db = new MYSQL;
	mysql_init(db);

	if(!db)
		panic("MySQL: initialization failed");

	db = mysql_real_connect(db, HOST, USER, PASSWORD, DATABASE, 0, NULL, 0);
	if(!db)
	{
		cout << mysql_error(db) << endl;
		panic("MySQL: Connection Error");
	}

	int sd;

	DEBUG_MSG("[CRemoteSystem::run] Listening for new connections...");

	while(1)
	{
		sd = server.accept();

		if(sd != -1)
		{
			// CRemoteClient *client = new CRemoteClient(sd);
			RCGateway *client = new RCGateway(sd, db);
			// client index
			static int i = 0;
			// DEBUG_MSG("[CRemoteSystem::run] client[" << i << "] on sockfd[" << sd << "]");
			
			// add new client
			clientList.push_back(client);
			// execute respective init and run methods
			clientList[i]->init(1,2);
			clientList[i]->run();
			i++;
			
			DEBUG_MSG("[CRemoteSystem::run] continue listening for new connections...");
		}
	}
	
}