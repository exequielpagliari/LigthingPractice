#include "Texture.h"
#include <stb_image.h>
#include <Logger.h>




bool Texture::loadTexture(std::string textureFilename,
	bool flipImage) {
	int mTexWidth, mTexHeight, mNumberOfChannels;

	stbi_set_flip_vertically_on_load(flipImage);
	unsigned char* textureData = stbi_load(textureFilename.c_str(), &mTexWidth,
		&mTexHeight, &mNumberOfChannels, 0);
	if (!textureData) {
		Logger::log(1, "%s: Error Charge Texture Data\n", __FUNCTION__);
		stbi_image_free(textureData);
		return false;
	}
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTexWidth,
		mTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(textureData);
	return true;
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, mTexture);
}
void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::cleanup() {
	glDeleteTextures(1, &mTexture);
}