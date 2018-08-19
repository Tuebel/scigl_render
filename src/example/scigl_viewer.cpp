#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>
#include <scigl_render/gl_context.hpp>
#include <scigl_render/example/example_render.hpp>
#include <scigl_render/scene/cv_camera.hpp>
#include <scigl_render/scene/diffuse_light.hpp>
#include <scigl_render/shader/single_texture_shader.hpp>

// forward declare callbacks
// get mouse and keyboard inputs
void process_input(GLFWwindow *window, scigl_render::CartesianPose &camera_pose,
                   const scigl_render::CvCamera &camera);

// Debug printing
std::ostream &operator<<(std::ostream &out, const glm::mat4 &mat)
{
    out << "[" << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << " " << mat[3][0] << "\n"
        << mat[0][1] << " " << mat[1][1] << " " << mat[2][1] << " " << mat[3][1] << "\n"
        << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << " " << mat[3][2] << "\n"
        << mat[0][3] << " " << mat[1][3] << " " << mat[2][3] << " " << mat[3][3] << "]";

    return out;
}

/*!
Provide the model as argument via command line.
*/
int main(int argc, char *argv[])
{
    // Some default parameters
    const int WIDTH = 1024;
    const int HEIGHT = 720;
    using namespace scigl_render;
    if (argc < 2)
    {
        throw std::runtime_error(
            "No model file provided. Run as */scigl_viwer <model_filename>!");
    }
    // Intrinsics of my shitty webcam
    CameraIntrinsics camera_intrinsics;
    camera_intrinsics.near = 0.01;
    camera_intrinsics.far = 15;
    camera_intrinsics.width = 640;
    camera_intrinsics.height = 480;
    camera_intrinsics.c_x = 411;
    camera_intrinsics.c_y = 310;
    camera_intrinsics.f_x = 511;
    camera_intrinsics.f_y = 513;
    // camera_intrinsics.f_x = 250;
    // camera_intrinsics.f_y = 250;
    // camera_intrinsics.c_x = 320;
    // camera_intrinsics.c_y = 240;
    CvCamera camera(camera_intrinsics);
    // Test if Cartesian -> Quaternion works
    CartesianPose camera_pose = {glm::vec3(0, 0, -2), glm::vec3(0, 0, 0)};
    camera.pose = camera_pose.to_quaternion_pose();
    DiffuseLight light;
    light.position = glm::vec3(2, 0, 0);
    light.color = glm::vec3(0.36, 0.4, 0.3);
    // Setup renderer
    std::shared_ptr<GLContext> context =
        std::make_shared<GLContext>(true, WIDTH, HEIGHT);
    ExampleRender render(context, SingleTextureShader::create_shader(),
                         GL_BGR, GL_UNSIGNED_BYTE, GL_RGB8, 3,
                         argv[1],
                         std::move(light));
    // main loop
    while (!glfwWindowShouldClose(context->get_window()))
    {
        process_input(context->get_window(), camera_pose, camera);
        camera.pose = camera_pose.to_quaternion_pose();
        render.next_frame(camera);
        glfwPollEvents();
    }
    // Finally release the context
    glfwTerminate();
    return EXIT_SUCCESS;
}

void process_input(GLFWwindow *window,
                   scigl_render::CartesianPose &camera_pose,
                   const scigl_render::CvCamera &camera)
{
    // exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    // movement
    float cameraSpeed = 0.01;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera_pose.position.y += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera_pose.position.y -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera_pose.position.x -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera_pose.position.x += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        camera_pose.position.z += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        camera_pose.position.z -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        camera_pose.orientation.x += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        camera_pose.orientation.x -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        camera_pose.orientation.y -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        camera_pose.orientation.y += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        camera_pose.orientation.z -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        camera_pose.orientation.z += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        std::cout << "camera info\n\n";
        std::cout << camera.get_projection_matrix() << "\n\n";
        std::cout << camera.get_view_matrix() << "\n\n";
    }
}
