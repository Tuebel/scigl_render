#include <scigl_render/render/frame_buffer.h>
#include <iostream>
#include <stdexcept>

namespace scigl_render
{
FrameBuffer::FrameBuffer(int width, int height, GLint internal_format,
                         GLenum format, GLenum type)
{
  this->width = width;
  this->height = height;
  this->format = format;
  this->type = type;
  // Create framebuffer with texture attachement
  glGenFramebuffers(1, &texture_fbo);
  activate();
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexStorage2D(GL_TEXTURE_2D, 1, internal_format, width, height);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture, 0);
  // Faster renderbuffer for depth testing and stencil
  glGenRenderbuffers(1, &depth_stencil_rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, depth_stencil_rbo);
  // Check framebuffer, will check rbo, too, since it is attached
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    throw std::runtime_error("Framebuffer is not completed");
  }
  deactivate();
}

FrameBuffer::~FrameBuffer()
{
  std::cout << "destroyed fbo\n";
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteRenderbuffers(1, &depth_stencil_rbo);
  glDeleteFramebuffers(1, &texture_fbo);
}

void FrameBuffer::activate()
{
  glBindFramebuffer(GL_FRAMEBUFFER, texture_fbo);
}

void FrameBuffer::deactivate()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::clear(float color, float depth, int stencil)
{
  activate();
  glClearColor(color, color, color, 1);
  glClearDepth(depth);
  glClearStencil(stencil);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  deactivate();
}

GLuint FrameBuffer::get_texture()
{
  return texture;
}

int FrameBuffer::get_width()
{
  return width;
}

int FrameBuffer::get_height()
{
  return height;
}

GLenum FrameBuffer::get_format()
{
  return format;
}

GLenum FrameBuffer::get_type()
{
  return type;
}
} // namespace scigl_render