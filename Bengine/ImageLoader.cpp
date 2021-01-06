#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "ErrorMess.h"

namespace Bengine{
	GLTexture ImageLoader::loadPNG(std::string filePath)
	{
		GLTexture texture={};

		//Out put texture after decode
		std::vector<unsigned char> out;
		//Input PNG file in binary on the buffer
		std::vector<unsigned char> in;
		unsigned long width,height;

		if(IOManager::readFileToBuffer(filePath,in)==false)
		{
			fatalError("Faile to load PNG file to buffer");
		};

		int errorCode = decodePNG(out,width,height,&(in[0]),in.size());

		if(errorCode !=0)
		{
			fatalError("Decode PNG failed with error: " +  errorCode);
		}

		//Generate texture
		glGenTextures(1,&(texture.id));

		//Bind texture
		glBindTexture(GL_TEXTURE_2D,texture.id);

		//Upload data into texture
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,&out[0]);

		//Tell open gl how to treat the texture
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		//undind the current texture
		glBindTexture(GL_TEXTURE_2D,0);

		//Set heigth and width to texture before return it
		texture.width = width;
		texture.height = height;

		return texture;

	}
}