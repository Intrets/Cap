#include "BufferWrappers.h"

#include <GLFW/glfw3.h>

#include <mem/Locator.h>
#include <misc/PathManager.h>
#include <render/loaders/ShaderLoader.h>

namespace render
{
	std::unordered_map<int32_t, bwo::Program*> bwo::Program::refs;

	bwo::VertexArrayObject::~VertexArrayObject() {
		glDeleteVertexArrays(1, &this->ID);
	}

	bwo::Texture::~Texture() {
		glDeleteTextures(1, &this->ID);
	}

	std::string bwo::Program::listAll() {
		std::stringstream out;

		for (auto& [index, program] : bwo::Program::refs) {
			out << "ID: " << index << '\n' << program->description << "\n\n";
		}

		return out.str();
	}

	std::optional<bwo::Program const*> bwo::Program::lookup(int32_t id) {
		auto it = bwo::Program::refs.find(id);

		if (it == bwo::Program::refs.end()) {
			return std::nullopt;
		}
		else {
			return it->second;
		}
	}

	void bwo::Program::use() {
		glUseProgram(this->ID);
	}

	bwo::Program::Program(char const* vert_raw, char const* frag_raw, std::string const& description_) {
		//this->ID = Locator<misc::PathManager>::ref().LoadShadersP(vert + ".vert", frag + ".frag");
		this->ID = LoadShaders(vert_raw, frag_raw);
		this->description = description_;
		bwo::Program::refs[this->ID] = this;
	}

	bwo::Program::~Program() {
		Program::refs.erase(this->ID);
		glDeleteProgram(this->ID);
	}

	bwo::UniformMatrix4fv::UniformMatrix4fv(std::string name, Program const& program) {
		this->location = glGetUniformLocation(program.ID, name.c_str());
	}

	void bwo::UniformMatrix4fv::set(glm::mat4 const& mat) {
		glUniformMatrix4fv(this->location, 1, GL_FALSE, &mat[0][0]);
	}

	void bwo::UniformTexture2D::set(GLuint texture) {
		glActiveTexture(GL_TEXTURE0 + this->unit);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(this->location, this->unit);
	}

	void bwo::UniformTexture2D::set(bwo::Texture2D const& texture) {
		this->set(texture.ID);
	}

	bwo::UniformTexture2D::UniformTexture2D(std::string name, Program const& program, int32_t unit_) {
		this->unit = unit_;
		this->location = glGetUniformLocation(program.ID, name.c_str());
	}

	void bwo::Uniform3fv::set(glm::vec3 vec) {
		glUniform3fv(this->location, 1, &vec[0]);
	}

	bwo::Uniform3fv::Uniform3fv(std::string name, Program const& program) {
		this->location = glGetUniformLocation(program.ID, name.c_str());
	}

	void bwo::Uniform1f::set(float f) {
		glUniform1f(this->location, f);
	}

	bwo::Uniform1f::Uniform1f(std::string name, Program const& program) {
		this->location = glGetUniformLocation(program.ID, name.c_str());
	}

	void bwo::UniformTexture2DArray::set(GLuint texture) {
		glActiveTexture(GL_TEXTURE0 + this->unit);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
		glUniform1i(location, this->unit);
	}

	void bwo::UniformTexture2DArray::set(Texture2DArray const& texture) {
		glActiveTexture(GL_TEXTURE0 + this->unit);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture.ID);
		glUniform1i(location, this->unit);
	}

	bwo::UniformTexture2DArray::UniformTexture2DArray(std::string name, Program const& program, int32_t unit_) {
		this->unit = unit_;
		this->location = glGetUniformLocation(program.ID, name.c_str());
	}

	void bwo::Uniform1i::set(int32_t i) {
		glUniform1i(this->location, i);
	}

	bwo::Uniform1i::Uniform1i(std::string name, Program const& program) {
		this->location = glGetUniformLocation(program.ID, name.c_str());
	}

	void bwo::Uniform2fv::set(glm::vec2 vec) {
		glUniform2fv(this->location, 1, &vec[0]);
	}

	bwo::Uniform2fv::Uniform2fv(std::string name, Program const& program) {
		this->location = glGetUniformLocation(program.ID, name.c_str());
	}

	void bwo::Uniform2iv::set(glm::ivec2 vec) {
		glUniform2iv(this->location, 1, &vec[0]);
	}

	void bwo::Uniform2iv::set(std::vector<glm::ivec2> const& vecs) {
		glUniform2iv(this->location, static_cast<GLsizei>(vecs.size()), &vecs[0][0]);
	}

	bwo::Uniform2iv::Uniform2iv(std::string name, Program const& program) {
		this->location = glGetUniformLocation(program.ID, name.c_str());
	}

	void bwo::Uniform4fv::set(glm::vec4 vec) {
		glUniform4fv(this->location, 1, &vec[0]);
	}

	bwo::Uniform4fv::Uniform4fv(std::string name, Program const& program) {
		this->location = glGetUniformLocation(program.ID, name.c_str());
	}

	bwo::FrameBuffer::FrameBuffer(GLFWwindow* window) {
		this->ID = 0;
		glfwGetWindowSize(window, &this->size.x, &this->size.y);
	}

	bwo::FrameBuffer::FrameBuffer() {
		glGenFramebuffers(1, &this->ID);
	}

	bwo::FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &this->ID);
	}

	void bwo::FrameBuffer::bindTexture(GLenum attachment, bwo::Texture2D const& texture, GLint mipmap) {
		this->size = texture.size;
		glBindFramebuffer(GL_FRAMEBUFFER, this->ID);
		glFramebufferTexture(GL_FRAMEBUFFER, attachment, texture.ID, mipmap);
	}

	void bwo::FrameBuffer::bindTextureLayer(GLenum attachment, bwo::Texture2DArray const& texture, int32_t mipmap, int32_t layer) {
		this->size.x = texture.size.x;
		this->size.y = texture.size.y;
		glBindFramebuffer(GL_FRAMEBUFFER, this->ID);
		glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, texture.ID, mipmap, layer);
	}

	void bwo::FrameBuffer::draw(glm::ivec2 size_, glm::ivec4 viewport, std::function<void()> f) {
		if (this->ID == 0) {
			this->size = size_;
			//assert(this->size.x >= viewport[0] + viewport[2] && this->size.y >= viewport[1] + viewport[3]);
		}
		else {
			if (this->size != size_) {

			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, this->ID);
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

		f();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	bwo::Texture2D::Texture2D(GLuint handle) {
		this->size = glm::ivec2(0, 0);
		this->ID = handle;
	}

	bwo::Texture2D::Texture2D(glm::ivec2 size_, GLint level, GLenum internalFormat, GLenum magFilter, GLenum minFilter, GLenum wrapS, GLenum wrapT, GLint border, GLenum pixelFormat, GLenum type, void const* pixels) {
		glGenTextures(1, &this->ID);
		glBindTexture(GL_TEXTURE_2D, this->ID);

		int32_t maxSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);

		this->size = glm::min(glm::ivec2(maxSize), size_);

		glTexImage2D(GL_TEXTURE_2D, level, internalFormat, this->size.x, this->size.y, border, pixelFormat, type, pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	}

	bwo::Texture2D::~Texture2D() {
		glDeleteTextures(1, &this->ID);
	}

	bwo::Texture2D::Texture2D(Texture2D&& other) {
		this->ID = other.ID;
		other.ID = 0;

		this->size = other.size;
	}

	bwo::Texture2D& bwo::Texture2D::operator=(Texture2D&& other) {
		if (this == &other) {
			return *this;
		}

		glDeleteTextures(1, &this->ID);
		this->ID = other.ID;
		other.ID = 0;

		this->size = other.size;

		return *this;
	}

	bwo::Texture2D bwo::Texture2DHelper::makeNoFiltering(glm::ivec2 size) {
		Texture2D res{
			size,
			0,
			GL_RGBA,
			GL_NEAREST,
			GL_NEAREST,
			GL_REPEAT,
			GL_REPEAT,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			(void*) 0
		};

		return res;
	}

	bwo::Texture2DArray::Texture2DArray(glm::ivec3 size_, GLint level, GLenum internalFormat, GLenum magFilter, GLenum minFilter, GLenum wrapS, GLenum wrapT, GLint border, GLenum pixelFormat, GLenum type, void const* pixels) {
		glGenTextures(1, &this->ID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, this->ID);

		int32_t maxSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);

		int32_t maxLayers;
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayers);

		this->size = glm::min(glm::ivec3(maxSize, maxSize, maxLayers), size_);
		glTexImage3D(GL_TEXTURE_2D_ARRAY, level, internalFormat, size.x, size.y, size.z, border, pixelFormat, type, pixels);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapT);
	}

	bwo::Texture2DArray::~Texture2DArray() {
		glDeleteTextures(1, &this->ID);
	}

	bwo::Texture2DArray::Texture2DArray(Texture2DArray&& other) {
		this->ID = other.ID;
		other.ID = 0;

		this->size = other.size;
	}

	bwo::Texture2DArray& bwo::Texture2DArray::operator=(Texture2DArray&& other) {
		if (this == &other) {
			return *this;
		}

		glDeleteTextures(1, &this->ID);
		this->ID = other.ID;
		other.ID = 0;

		this->size = other.size;

		return *this;
	}

	bwo::Texture2DArray bwo::Texture2DArrayHelper::makeLinearFiltering(glm::ivec3 size) {
		Texture2DArray res{
			size,
			0,
			GL_RGBA,
			GL_LINEAR,
			GL_LINEAR,
			GL_REPEAT,
			GL_REPEAT,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			(void*) 0
		};
		return res;
	}
}
