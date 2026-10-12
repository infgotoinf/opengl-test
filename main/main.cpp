#include <sys/types.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <filesystem>



#define WIDTH 640
#define HEIGHT 640



namespace fs = std::filesystem;



struct ShaderFile {
    const char* path;
    fs::file_time_type last_change_time;
    ShaderFile(const char* path) : path(path) {
        last_change_time = fs::last_write_time(path);
    }
};

struct Shader {
    ShaderFile file;
    uint shader;
};



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void glfw_error_callback(int code, const char* description)
{
    printf("GLFW error %d: %s\n", code, description);
}

void compileShader(Shader shader)
{
    std::ifstream shader_file(shader.file.path);
    if (!shader_file) {
        printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n%s\n", shader.file.path);
        return;
    }

    std::stringstream shader_stream;
    // read file's buffer contents into streams
    shader_stream << shader_file.rdbuf();
    // close file handlers
    shader_file.close();
    // convert stream into string
    std::string shader_code = shader_stream.str();
    const char* cstr_shader_code = shader_code.c_str();

    glShaderSource(shader.shader, 1, &cstr_shader_code, nullptr);
    glCompileShader(shader.shader);

    int success;
    glGetShaderiv(shader.shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader.shader, 512, nullptr, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }
}

uint createShaderProgram(std::list<Shader> shaders)
{
    unsigned int shaderProgram = glCreateProgram();
    for (auto shader : shaders)
        glAttachShader(shaderProgram, shader.shader);

    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINK_FAILED\n%s\n", infoLog);
    }

    for (auto shader : shaders)
        glDeleteShader(shader.shader);

    return shaderProgram;
}

void updateShaderProgram(std::list<Shader> *shaders, uint *shaderProgram) {
    bool shader_was_recompiled = false;

    for (auto &shader : *shaders) {
        if (fs::file_time_type file_last_time_write = fs::last_write_time(shader.file.path)
           ; shader.file.last_change_time < file_last_time_write)
        {
            printf("Recompiling %s\n", shader.file.path);
            compileShader(shader);
            shader.file.last_change_time = file_last_time_write;
            shader_was_recompiled = true;
        };
    }

    if (shader_was_recompiled)
    {
        printf("Recompiling shader program\n");
        *shaderProgram = createShaderProgram(*shaders);
    }
}



int main(void)
{
    glfwSetErrorCallback(glfw_error_callback);

    // Initialize the library
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // This makes window floating in sway
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Shader test", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    glfwSetWindowSize(window, WIDTH, HEIGHT);

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Loading GLAD
    if (gladLoadGL(glfwGetProcAddress) == 0)
        return 1;


    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    std::list<Shader> shaders = {
        { { "./resources/triangle.vert" }, glCreateShader(GL_VERTEX_SHADER) },
        { { "./resources/triangle.frag" }, glCreateShader(GL_FRAGMENT_SHADER) },
    };

    for (auto shader : shaders) {
        compileShader(shader);
    }

    uint shaderProgram = createShaderProgram(shaders);

    int mouseLoc = glGetUniformLocation(shaderProgram, "u_mouse");
    int resolutionLoc = glGetUniformLocation(shaderProgram, "u_resolution");
    int timeLoc = glGetUniformLocation(shaderProgram, "u_time");
    double mouse_x, mouse_y;

    double shaderUpdateTime = glfwGetTime();

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetTime() - shaderUpdateTime > 0.5) {
            shaderUpdateTime = glfwGetTime();
            updateShaderProgram(&shaders, &shaderProgram);
        }

        glViewport(0, 0, WIDTH, HEIGHT);

        // Render here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // be sure to activate the shader
        glUseProgram(shaderProgram);

        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        glUniform2f(mouseLoc,
            static_cast<float>(mouse_x),
            static_cast<float>(mouse_y));
        glUniform2f(resolutionLoc,
            static_cast<float>(WIDTH),
            static_cast<float>(HEIGHT));
        glUniform1f(timeLoc, static_cast<float>(glfwGetTime()));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
