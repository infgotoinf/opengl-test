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

void compileShaderFromFile(const char* shader_path, uint shader_type)
{
    std::string shader_code;
    std::ifstream shader_file;

    // ensure ifstream objects can throw exceptions:
    shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open file
        shader_file.open(shader_path);
        std::stringstream shader_stream;
        // read file's buffer contents into streams
        shader_stream << shader_file.rdbuf();
        // close file handlers
        shader_file.close();
        // convert stream into string
        shader_code = shader_stream.str();
    }
    catch(std::ifstream::failure e)
    {
        printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }
    const char* cstr_shader_code = shader_code.c_str();
    unsigned int vertexShader;
    glShaderSource(shader_type, 1, &cstr_shader_code, NULL);
    glCompileShader(shader_type);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
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
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World from GLFW + OpenGL", NULL, NULL);

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

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // as we only have a single shader, we could also just activate our shader once beforehand if we want to
    glUseProgram(shaderProgram);

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
        // glUseProgram(shaderProgram);

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
