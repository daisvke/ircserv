// #include "../incs/headers.hpp"
// #include "../incs/server.hpp"
#include "headers.hpp"
#include "server.hpp"
int main(){
	Server	Server;
	try {
			Server.initServer();
	}
	catch (std::exception &e){
		std::cout << e.what() << std::endl;
	}
}

