#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <optional>
#include <functional>

#include <misc/Misc.h>

struct GLFWwindow;

namespace render
{
	namespace bwo
	{
		enum class BufferHint
		{
			STREAM_DRAW = GL_STREAM_DRAW,
			STREAM_READ = GL_STREAM_READ,
			STREAM_COPY = GL_STREAM_COPY,
			STATIC_DRAW = GL_STATIC_DRAW,
			STATIC_READ = GL_STATIC_READ,
			STATIC_COPY = GL_STATIC_COPY,
			DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
			DYNAMIC_READ = GL_DYNAMIC_READ,
			DYNAMIC_COPY = GL_DYNAMIC_COPY,
		};

		class Texture
		{
		public:
			GLuint ID;
			glm::ivec2 size;

			Texture() = default;
			Texture(GLuint ID_) : ID(ID_) {
			};
			~Texture();

			NOCOPYMOVE(Texture);
		};

		class Texture2DArray
		{
		public:
			GLuint ID;
			glm::ivec3 size;

			Texture2DArray(
				glm::ivec3 size,
				GLint level,
				GLenum internalFormat,
				GLenum magFilter, GLenum minFilter, GLenum wrapS, GLenum wrapT,
				GLint border,
				GLenum pixelFormat,
				GLenum type,
				void const* pixels
			);

			~Texture2DArray();

			Texture2DArray(Texture2DArray&& other);
			Texture2DArray& operator=(Texture2DArray&& other);

			NOCOPY(Texture2DArray);
		};

		class Texture2D
		{
		public:
			GLuint ID;
			glm::ivec2 size;

			Texture2D(GLuint handle);
			Texture2D(
				glm::ivec2 size,
				GLint level,
				GLenum internalFormat,
				GLenum magFilter, GLenum minFilter, GLenum wrapS, GLenum wrapT,
				GLint border,
				GLenum pixelFormat,
				GLenum type,
				void const* pixels
			);

			~Texture2D();

			Texture2D(Texture2D&& other);
			Texture2D& operator=(Texture2D&& other);

			NOCOPY(Texture2D);
		};

		namespace Texture2DHelper
		{
			Texture2D makeNoFiltering(glm::ivec2 size);
		};

		namespace Texture2DArrayHelper
		{
			Texture2DArray makeLinearFiltering(glm::ivec3 size);
		}

		class VertexArrayObject
		{
		private:
			GLuint ID;
		public:
			void gen(int32_t count) {
				glGenVertexArrays(1, &ID);
				bind();
				for (int32_t i = 0; i < count; i++) {
					glEnableVertexAttribArray(i);
				}
			};
			void bind() {
				glBindVertexArray(ID);
			};
			void unbind() {
				glBindVertexArray(0);
			};

			VertexArrayObject() = default;
			~VertexArrayObject();

			NOCOPYMOVE(VertexArrayObject);
		};

		class FrameBuffer
		{
		public:
			GLuint ID;
			glm::ivec2 size;

			FrameBuffer(GLFWwindow* window);
			FrameBuffer();
			~FrameBuffer();

			void bindTexture(GLenum attachment, bwo::Texture2D const& texture, GLint mipmap);
			void bindTextureLayer(GLenum attachment, bwo::Texture2DArray const& texture, int32_t mipmap, int32_t layer);

			void draw(glm::ivec2 size_, glm::ivec4 viewport, std::function<void()> f);

			NOCOPYMOVE(FrameBuffer);
		};

		template<class T>
		class ArrayBuffer
		{
		public:

		private:
			GLenum usageHint;

		public:
			GLuint ID;

			ArrayBuffer(BufferHint hint);
			~ArrayBuffer();

			void set(std::vector<T> const& data);
			void set(uint32_t size, std::vector<T> const& data);
			void setRaw(uint32_t size, void const* data);

			void bind(GLenum location);

			NOCOPYMOVE(ArrayBuffer);
		};

		class Program
		{
		private:
			friend class Uniform1f;
			friend class Uniform1i;
			friend class Uniform2fv;
			friend class Uniform2iv;
			friend class Uniform3fv;
			friend class Uniform3iv;
			friend class Uniform4fv;
			friend class Uniform4iv;
			friend class UniformMatrix4fv;
			friend class UniformTexture2D;
			friend class UniformTexture2DArray;

			GLuint ID;

			std::string description;

		public:
			static std::unordered_map<int32_t, Program*> refs;
			static std::string listAll();
			static std::optional<Program const*> lookup(int32_t id);

			void use();

			Program(char const* vert_raw, char const* frag_raw, std::string const& description);
			~Program();

			NOCOPYMOVE(Program);

		private:
			Program() = default;
		};

		class Uniform2iv
		{
		private:
			GLuint location;

		public:
			void set(glm::ivec2 vec);
			void set(std::vector<glm::ivec2> const& vecs);

			Uniform2iv() = default;
			Uniform2iv(std::string name, Program const& program);
			~Uniform2iv() = default;
		};

		class Uniform2fv
		{
		private:
			GLuint location;

		public:
			void set(glm::vec2 vec);

			Uniform2fv() = default;
			Uniform2fv(std::string name, Program const& program);
			~Uniform2fv() = default;
		};

		class UniformMatrix4fv
		{
		private:
			GLuint location;

		public:
			void set(glm::mat4 const& mat);

			UniformMatrix4fv() = default;
			UniformMatrix4fv(std::string name, Program const& program);
			~UniformMatrix4fv() = default;
		};

		class UniformTexture2D
		{
		private:
			int32_t unit;
			GLuint location;

		public:
			void set(GLuint texture);
			void set(bwo::Texture2D const& texture);

			UniformTexture2D() = default;
			UniformTexture2D(std::string name, Program const& program, int32_t unit);
			~UniformTexture2D() = default;
		};

		class Uniform3fv
		{
		private:
			GLuint location;

		public:
			void set(glm::vec3 vec);

			Uniform3fv() = default;
			Uniform3fv(std::string name, Program const& program);
			~Uniform3fv() = default;
		};

		class Uniform4fv
		{
		private:
			GLuint location;

		public:
			void set(glm::vec4 vec);

			Uniform4fv() = default;
			Uniform4fv(std::string name, Program const& program);
			~Uniform4fv() = default;
		};

		class Uniform1f
		{
		private:
			GLuint location;

		public:
			void set(float f);

			Uniform1f() = default;
			Uniform1f(std::string name, Program const& program);
			~Uniform1f() = default;
		};

		class UniformTexture2DArray
		{
		private:
			GLuint location;
			int32_t unit;

		public:
			void set(GLuint texture);

			UniformTexture2DArray() = default;
			UniformTexture2DArray(std::string name, Program const& program, int32_t unit);
			~UniformTexture2DArray() = default;
		};

		class Uniform1i
		{
		private:
			GLuint location;

		public:
			void set(int32_t i);

			Uniform1i() = default;
			Uniform1i(std::string name, Program const& program);
			~Uniform1i() = default;
		};

		template<class T>
		inline ArrayBuffer<T>::ArrayBuffer(BufferHint hint) {
			glGenBuffers(1, &this->ID);
			this->usageHint = static_cast<GLenum>(hint);
		}

		template<class T>
		inline ArrayBuffer<T>::~ArrayBuffer() {
			glDeleteBuffers(1, &this->ID);
		}

		template<class T>
		inline void ArrayBuffer<T>::set(std::vector<T> const& data) {
			this->setRaw(static_cast<uint32_t>(sizeof(T) * data.size()), &data[0]);
		}

		template<class T>
		inline void ArrayBuffer<T>::set(uint32_t size, std::vector<T> const& data) {
			this->setRaw(static_cast<uint32_t>(sizeof(T) * size), &data[0]);
		}

		template<class T>
		inline void ArrayBuffer<T>::setRaw(uint32_t size, void const* data) {
			glBindBuffer(GL_ARRAY_BUFFER, this->ID);
			glBufferData(GL_ARRAY_BUFFER, size, data, this->usageHint);
		}
		template<class T>
		inline void ArrayBuffer<T>::bind(GLenum location) {
			glBindBuffer(location, this->ID);
		}
	}
}
