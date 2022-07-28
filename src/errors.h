#pragma once
#include <fstream>
#include <string>

class Error {
	public:
		static void logError(std::string error) {
			std::ofstream file("errores.txt", std::ios::app);
			file << error << std::endl;
			file.close();
		}
};