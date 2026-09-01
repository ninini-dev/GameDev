#include <stb_image.h>
#include <iostream>
#include <own/texture.h>

void SetUpStbiForTextures() {
	stbi_set_flip_vertically_on_load(true);
}

GLuint CreateTexture(const char* path) {

	GLuint ID;

	int wImg, hImg, numColch;
	unsigned char* bytes = stbi_load(path, &wImg, &hImg, &numColch, 0);

	if (!bytes) {
		std::cerr << "STB Error: Could not load file at " << path << std::endl;
		return 0; // Return invalid texture ID
	}

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wImg, hImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
	//MAYBE AND ONLY MAYBE, I MUST UNBIND THE TEXTURE
	

	return ID;
}