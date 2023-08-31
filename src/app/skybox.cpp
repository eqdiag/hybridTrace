#include <cassert>
#include "core/stb_image.h"
#include "core/settings.h"
#include "math/matrix.h"
#include "skybox.h"


Skybox::Skybox() {

}


void Skybox::init(const std::vector<std::string> skyboxFaces, const core::Camera& camera, const math::Mat4& projMatrix)
{
	assert(skyboxFaces.size() == 6);

	mSkyboxShader.init(SHADER_DIR, "skybox.vs", "skybox.fs");

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	GLuint posAttrib = mSkyboxShader.getAttributeLocation("position");
	assert(posAttrib >= 0);
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);



	//Skybox stuff
	glGenTextures(1, &mTexId);
	glBindTexture(GL_TEXTURE_CUBE_MAP,mTexId);

	int width, height, nrChannels;
	unsigned char* data;
	for (unsigned int i = 0; i < skyboxFaces.size(); i++)
	{

		std::string path{IMAGE_DIR};
		path += skyboxFaces[i];


		data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);
		stbi_image_free(data);
	}


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);



	mSkyboxShader.use();
	mSkyboxShader.setUniformMat4("model", math::Mat4::identity().getRawData());
	mSkyboxShader.setUniformMat4("view", camera.getViewMatrix().removeTranslation().getRawData());
	mSkyboxShader.setUniformMat4("projection", projMatrix.getRawData());

	//Set skybox sampler to point to default tex unit 0
	mSkyboxShader.setUniformInt("skybox", 0);

}


Skybox::~Skybox()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteTextures(1, &mTexId);
}

void Skybox::Render(const core::Camera& camera, bool skyToggle)
{

	mSkyboxShader.use();
	auto viewMatrix = camera.getViewMatrix();
	mSkyboxShader.setUniformMat4("view", viewMatrix.removeTranslation().getRawData());
	mSkyboxShader.setUniformBool("skyToggle", skyToggle);

	glBindVertexArray(mVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexId);
	glDrawArrays(GL_TRIANGLES, 0, 36);

}
