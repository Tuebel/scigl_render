#include <scigl_render/render/frame_buffer.hpp>
#include <iostream>
#include <stdexcept>

namespace scigl_render
{
FrameBuffer::FrameBuffer(int width, int height, GLenum internal_format)
{
  this->width = width;
  this->height = height;
  // Create framebuffer with renderbuffer attachements
  glGenFramebuffers(1, &fbo);
  activate();
  glGenRenderbuffers(1, &color_rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, color_rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER, color_rbo);
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
  glDeleteRenderbuffers(1, &color_rbo);
}

void FrameBuffer::activate()
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
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
} // namespace scigl_render