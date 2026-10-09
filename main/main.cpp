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



#define WIDTH 640
#define HEIGHT 640

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void glfw_error_callback(int code, const char* description)
{
    printf("GLFW error %d: %s\n", code, description);
}

void compileShaderFromFile(const char* shader_path, uint shader)
{
    std::ifstream shader_file(shader_path);
    if (!shader_file) {
        printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n%s\n", shader_path);
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

    glShaderSource(shader, 1, &cstr_shader_code, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }
}

uint createShaderProgram(std::list<uint> shaders)
{
    unsigned int shaderProgram = glCreateProgram();
    for (auto shader : shaders)
        glAttachShader(shaderProgram, shader);

    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINK_FAILED\n%s\n", infoLog);
    }

    for (auto shader : shaders)
        glDeleteShader(shader);

    return shaderProgram;
}

int main(void)
{
    glfwSetErrorCallback(glfw_error_callback);

    // Initialize the library
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World from GLFW + OpenGL", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Loading GLAD
    if (gladLoadGL(glfwGetProcAddress) == 0)
        return 1;


    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    compileShaderFromFile("./resources/triangle.vert", vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    compileShaderFromFile("./resources/triangle.frag", fragmentShader);

    uint shaderProgram = createShaderProgram({
            vertexShader,
            fragmentShader
     });


    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Render here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // be sure to activate the shader
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
