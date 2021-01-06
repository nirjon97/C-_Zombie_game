#include "IOManager.h"

#include <fstream>

namespace Bengine{
	bool IOManager::readFileToBuffer(std::string filePath,std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filePath,std::ios::binary);
		if(file.fail())
		{
			perror(filePath.c_str());
			return false;
		}
		//Determine how big the file which read 
		//seek to the end of the file to get the byte size of file
		file.seekg(0,std::ios::end);

		//get file size
		int fileSize = file.tellg();
		//return to the begin after get the size
		file.seekg(0,std::ios::beg);

		//Reduce file size by any header bytes that might be present
		fileSize -= file.tellg();

		buffer.resize(fileSize);
		file.read((char *)&buffer[0],fileSize);

		file.close();

		return true;

	}
}