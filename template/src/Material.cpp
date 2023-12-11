// Local includes
#include "Material.h"
#include "Texture.h"
#include "Shader.h"
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>

Material::~Material()
{
	if (m_program != 0)
	{
		glDeleteProgram(m_program);
	}
}

void Material::init()
{
	// TODO : Change shader by your
	m_program = load_shaders("shaders/unlit/vertex.glsl", "shaders/unlit/fragment.glsl");
	check();
	// TODO : set initial parameters
	m_color = {1.0, 1.0, 1.0, 1.0};
	m_texture = loadTexture2DFromFilePath("./data/Default_albedo.jpg");
	normalMap = loadTexture2DFromFilePath("./data/Default_normal.jpg");

	metalRoughness = loadTexture2DFromFilePath("./data/Default_metalRoughness.jpg");
	aoMap = loadTexture2DFromFilePath("./data/Default_AO.jpg");
	emissiveMap = loadTexture2DFromFilePath("./data/Default_emissive.jpg");
}

void Material::clear()
{
	// nothing to clear
	// TODO: Add the texture or buffer you want to destroy for your material
}

void Material::bind()
{
	check();
	glUseProgram(m_program);
	internalBind();
}

void Material::internalBind()
{
	// bind parameters
	GLint color = getUniform("color");
	glUniform4fv(color, 1, glm::value_ptr(m_color));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(getUniform("colorTexture"), 0);

	// TODO : Add your custom parameters here
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	glUniform1i(getUniform("normalMap"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, metalRoughness);
	glUniform1i(getUniform("metalRoughnessMap"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, aoMap);
	glUniform1i(getUniform("aoMap"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, emissiveMap);
	glUniform1i(getUniform("emissiveMap"), 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glUniform1i(getUniform("skybox"), 5);
}

void Material::setMatrices(glm::mat4 &projectionMatrix, glm::mat4 &viewMatrix, glm::mat4 &modelMatrix)
{
	check();
	glUniformMatrix4fv(getUniform("projection"), 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(getUniform("view"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(getUniform("model"), 1, false, glm::value_ptr(modelMatrix));
}

GLint Material::getAttribute(const std::string &in_attributeName)
{
	check();
	return glGetAttribLocation(m_program, in_attributeName.c_str());
}

GLint Material::getUniform(const std::string &in_uniformName)
{
	check();
	return glGetUniformLocation(m_program, in_uniformName.c_str());
}
