// Hong Luu
// Project

// This code is based on SNHU tutorials from Module 2 to 6, and it also utilizes resources provided in those modules.
// The tutorials have been followed to understand concepts related to implementing lighting in the 3D object.
// For the original tutorial content and explanations, please refer to the SNHU CS-330 Module 2 to 6 Tutorials and Resources.

#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

using namespace std;

#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

namespace {
    const char* const WINDOW_TITLE = "Chicken Broth Box";
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    struct GLMesh {
        GLuint vao;
        GLuint vbos[2];
        GLuint nIndices;
    };

    GLFWwindow* gWindow = nullptr;
    GLMesh gCubeMesh;
    GLMesh gCylinderMesh;
    GLuint gProgramId;
    GLuint gTextureId;
    GLMesh gPlaneMesh;
    GLMesh gSphereMesh;
    GLMesh gPyramidMesh;

    const GLchar* vertexShaderSource = GLSL(440,
        layout(location = 0) in vec3 position;
    layout(location = 2) in vec2 textureCoordinate;

    out vec2 vertexTextureCoordinate;


    //Global variables for the transform matrices
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
        vertexTextureCoordinate = textureCoordinate;
    }
    );


    /* Fragment Shader Source Code*/
    const GLchar* fragmentShaderSource = GLSL(440,
        in vec2 vertexTextureCoordinate;
    out vec4 fragmentColor;

    uniform sampler2D uTexture;
    uniform vec3 lightDirection; // Directional light direction
    uniform vec3 lightColor;     // Directional light color
    uniform float ambientStrength = 0.3; // Ambient light strength

    void main()
    {
        vec3 norm = normalize(texture(uTexture, vertexTextureCoordinate).rgb * 2.0 - 1.0);
        vec3 lightDir = normalize(-lightDirection);

        // Calculate the diffuse lighting intensity
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // Calculate the ambient lighting intensity
        vec3 ambient = ambientStrength * lightColor;

        // Final color with lighting
        vec3 result = (ambient + diffuse) * texture(uTexture, vertexTextureCoordinate).rgb;

        fragmentColor = vec4(result, 1.0);
    }
    );
}

// camera variables
glm::vec3 cameraPosition(0.0f, 2.0f, 5.0f); // camera position (initially at X:0, Y:2, Z:5)
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f); // camera's front direction (initially looking forward)
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f); // camera's up direction (initially pointing up)

// define camera values
float yaw = -90.0f;   // Initial yaw angle (left to right)
float pitch = 0.0f;   // Initial pitch angle (up and down)
float zoom = 45.0f;   // Initial zoom level
float mouseSensitivity = 0.1f;
float cameraSpeed = 0.05f;

glm::mat4 view;
glm::mat4 projection;

// Add a boolean variable to toggle between perspective and orthographic views
bool usePerspective = true;

// Declare all functions will be adding to this program
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
bool UInitialize(int argc, char* argv[], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UCreateMesh(GLMesh& mesh, GLfloat* vertices, GLushort* indices, int vertexCount, int indexCount);
void UDestroyMesh(GLMesh& mesh);
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);
void URender();
void UCubeMesh(GLMesh& mesh);
void UCylinderMesh(GLMesh& mesh);
void UPlaneMesh(GLMesh& mesh, float width, float length);
void USphereMesh(GLMesh& mesh, float radius, int segments);
void UPyramidMesh(GLMesh& mesh);


// Function to initialize the pyramid mesh - cheese piece
void UPyramidMesh(GLMesh& mesh) {
    GLfloat pyramidVertices[] = {
        // Vertex Positions         // Colors (r, g, b, a)   // Position of vertice for visualization 
        0.0f,  0.5f,  0.0f,         1.0f, 1.0f, 0.0f, 1.0f,  // Top Vertex
        -0.5f, -0.5f, -0.5f,        1.0f, 0.8f, 0.0f, 1.0f,  // Bottom Left Back
        0.5f, -0.5f, -0.5f,         1.0f, 0.8f, 0.0f, 1.0f,  // Bottom Right Back

        0.0f,  0.5f,  0.0f,         1.0f, 1.0f, 0.0f, 1.0f,  // Top Vertex
        0.5f, -0.5f, -0.5f,         1.0f, 0.8f, 0.0f, 1.0f,  // Bottom Right Back
        0.5f, -0.5f,  0.5f,         1.0f, 0.8f, 0.0f, 1.0f,  // Bottom Right Front

        0.0f,  0.5f,  0.0f,         1.0f, 1.0f, 0.0f, 1.0f,  // Top Vertex
        0.5f, -0.5f,  0.5f,         1.0f, 0.8f, 0.0f, 1.0f,  // Bottom Right Front
       -0.5f, -0.5f,  0.5f,         1.0f, 0.8f, 0.0f, 1.0f,  // Bottom Left Front

        0.0f,  0.5f,  0.0f,         1.0f, 1.0f, 0.0f, 1.0f,  // Top Vertex
       -0.5f, -0.5f,  0.5f,         1.0f, 0.8f, 0.0f, 1.0f,  // Bottom Left Front
       -0.5f, -0.5f, -0.5f,         1.0f, 0.8f, 0.0f, 1.0f   // Bottom Left Back
    };

    GLushort pyramidIndices[] = {
        0,1,2,3,4,5,6,7,8,9,10,11
    };

    UCreateMesh(mesh, pyramidVertices, pyramidIndices, sizeof(pyramidVertices) / sizeof(pyramidVertices[0]) / 7, sizeof(pyramidIndices) / sizeof(pyramidIndices[0]));
}

// Function to initialize the sphere mesh - an orange
void USphereMesh(GLMesh& mesh, float radius, int segments) {
    // Calculate the number of vertices and indices required for the sphere
    int numVertices = (segments + 1) * (segments + 1);
    int numIndices = segments * segments * 6;

    GLfloat* sphereVertices = new GLfloat[numVertices * 3];
    GLushort* sphereIndices = new GLushort[numIndices];

    // Generate sphere vertices
    int index = 0;
    for (int i = 0; i <= segments; ++i) {
        float phi = static_cast<float>(i) / static_cast<float>(segments) * static_cast<float>(M_PI);
        for (int j = 0; j <= segments; ++j) {
            float theta = static_cast<float>(j) / static_cast<float>(segments) * static_cast<float>(2.0 * M_PI);

            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi);
            float z = radius * sin(phi) * sin(theta);

            sphereVertices[index++] = x;
            sphereVertices[index++] = y;
            sphereVertices[index++] = z;
        }
    }

    // Generate sphere indices
    index = 0;
    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < segments; ++j) {
            int vertexIndex = i * (segments + 1) + j;

            sphereIndices[index++] = static_cast<GLushort>(vertexIndex);
            sphereIndices[index++] = static_cast<GLushort>(vertexIndex + 1);
            sphereIndices[index++] = static_cast<GLushort>(vertexIndex + segments + 1);

            sphereIndices[index++] = static_cast<GLushort>(vertexIndex + 1);
            sphereIndices[index++] = static_cast<GLushort>(vertexIndex + segments + 2);
            sphereIndices[index++] = static_cast<GLushort>(vertexIndex + segments + 1);
        }
    }

    UCreateMesh(mesh, sphereVertices, sphereIndices, numVertices, numIndices);

    // Clean up dynamic arrays
    delete[] sphereVertices;
    delete[] sphereIndices;
}

// Function to initialize a 3D plane mesh - table surface
void UPlaneMesh(GLMesh& mesh, float width, float length) {
    float halfWidth = width / 2.0f;
    float halfLength = length / 2.0f;
    float yOffset = -0.8f; // Adjust the yOffset to position the plane lower

    GLfloat planeVertices[] = {
        // Vertex Positions         // Colors (r, g, b, a)
        -halfWidth, yOffset, halfLength,     0.0f, 1.0f, 0.0f, 1.0f, // Bottom Left Front
         halfWidth, yOffset, halfLength,     0.0f, 1.0f, 0.0f, 1.0f, // Bottom Right Front
         halfWidth, yOffset, -halfLength,    0.0f, 1.0f, 0.0f, 1.0f, // Bottom Right Back
        -halfWidth, yOffset, -halfLength,    0.0f, 1.0f, 0.0f, 1.0f  // Bottom Left Back
    };

    GLushort planeIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    UCreateMesh(mesh, planeVertices, planeIndices, 4, 6);
}

// Function to initialize the cube mesh - chicken broth box
void UCubeMesh(GLMesh& mesh) {
    GLfloat cubeVertices[] = {
        // Vertex Positions         // Colors (r, g, b, a)
        0.3f,  0.5f,  0.6f,         1.0f, 1.0f, 0.0f, 1.0f, // Top Right Front
        0.3f, -0.7f,  0.6f,         1.0f, 0.5f, 0.0f, 1.0f, // Bottom Right Front
        -0.3f, -0.7f,  0.6f,         1.0f, 1.0f, 0.0f, 1.0f, // Bottom Left Front
        -0.3f,  0.5f,  0.6f,         1.0f, 0.0f, 1.0f, 1.0f, // Top Left Front

        0.3f,  0.5f, -0.25f,         1.0f, 1.0f, 0.0f, 1.0f, // Top Right Back
        0.3f, -0.7f, -0.25f,         1.0f, 0.5f, 0.0f, 1.0f, // Bottom Right Back
        -0.3f, -0.7f, -0.25f,         1.0f, 1.0f, 0.0f, 1.0f, // Bottom Left Back
        -0.3f,  0.5f, -0.25f,         1.0f, 0.5f, 0.0f, 1.0f  // Top Left Back
    };

    GLushort cubeIndices[] = {
        0,1,4,1,4,5,4,5,7,5,6,7,1,2,5,2,5,6,0,3,4,3,4,7,2,3,6,3,6,7,0,1,3,1,2,3
    };


    UCreateMesh(mesh, cubeVertices, cubeIndices, sizeof(cubeVertices) / sizeof(cubeVertices[0]) / 7, sizeof(cubeIndices) / sizeof(cubeIndices[0]));
}

// Function to initialize the cylinder mesh - the cap of the chicken broth box
void UCylinderMesh(GLMesh& mesh) {
    GLfloat cylinderVertices[360 * 3 + 3]; // Updated cylinder vertices
    GLushort cylinderIndices[720 * 3];    // Updated cylinder indices

    // Center vertex at the bottom
    cylinderVertices[0] = 0.0f;
    cylinderVertices[1] = 0.0f;
    cylinderVertices[2] = 0.0f;

    // Vertices for the top circle
    float radius = 0.2f;
    int numSegments = 360;
    float segmentAngle = 2.0f * M_PI / numSegments;
    float cylinderHeight = 0.7f; // Set the height of the cylinder

    for (int i = 0; i < numSegments; ++i) {
        int baseIndex = 3 * (i + 1);
        float x = radius * cos(i * segmentAngle);
        float y = radius * sin(i * segmentAngle);
        cylinderVertices[baseIndex] = x;
        cylinderVertices[baseIndex + 1] = y;
        cylinderVertices[baseIndex + 2] = cylinderHeight;
    }

    // Define cylinder indices
    for (int i = 0; i < numSegments; ++i) {
        int baseIndex = 3 * i;
        cylinderIndices[3 * i] = 0;
        cylinderIndices[3 * i + 1] = baseIndex + 1;
        cylinderIndices[3 * i + 2] = (baseIndex + 4) % (numSegments * 3);
    }

    UCreateMesh(mesh, cylinderVertices, cylinderIndices, numSegments + 2, numSegments * 3);
}

// Function to create and load a texture
bool UCreateTexture(const char* filename, GLuint& textureId) {
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image) {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}

// Function to destroy a texture
void UDestroyTexture(GLuint textureId) {
    glDeleteTextures(1, &textureId);
}

// main function
int main(int argc, char* argv[]) {
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create cube and cylinder meshes
    UCubeMesh(gCubeMesh);
    UCylinderMesh(gCylinderMesh);
    UPlaneMesh(gPlaneMesh, 5.0f, 5.0f); // Create a 3D plane with width 5.0 and length 5.0
    USphereMesh(gSphereMesh, 0.5f, 32);
    UPyramidMesh(gPyramidMesh);

    if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
        return EXIT_FAILURE;

    // Load the texture 
    const char* texFilename = "textures/broth.png";

    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gProgramId);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gProgramId, "uTexture"), 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set the framebuffer size callback
    glfwSetFramebufferSizeCallback(gWindow, UResizeWindow);

    // Set the cursor position callback
    glfwSetCursorPosCallback(gWindow, UMousePositionCallback);

    // Set the scroll callback
    glfwSetScrollCallback(gWindow, UMouseScrollCallback);

    // Set cursor mode to normal
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Enable capturing mouse cursor movement
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(gWindow)) {
        glfwPollEvents();
        UProcessInput(gWindow);
        URender();

    }

    UDestroyMesh(gCubeMesh);
    UDestroyMesh(gCylinderMesh);
    UDestroyShaderProgram(gProgramId);
    // Release texture
    UDestroyTexture(gTextureId);

    exit(EXIT_SUCCESS);
}

void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = WINDOW_WIDTH / 2.0f;
    static float lastY = WINDOW_HEIGHT / 2.0f;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = (xpos - lastX) * mouseSensitivity;
    float yoffset = (lastY - ypos) * mouseSensitivity;

    lastX = xpos;
    lastY = ypos;

    yaw += xoffset;
    pitch += yoffset;

    // constrain pitch to avoid camera flipping
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    // Update the view matrix
    view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    zoom -= (float)yoffset;

    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;

    // Update the projection matrix
    projection = glm::perspective(glm::radians(zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
}

bool UInitialize(int argc, char* argv[], GLFWwindow** window) {
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (*window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);

    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);

    // set cursor mode to normal
    // glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // enable capturing mouse cursor movement
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // register the scroll callback
    glfwSetScrollCallback(*window, UMouseScrollCallback);

    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult) {
        cerr << glewGetErrorString(GlewInitResult) << endl;
        return false;
    }

    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}

void UResizeWindow(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// function for keys
void UProcessInput(GLFWwindow* window) {

    // Esc button
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // WASD keys to control the forward, backward, left, and right motion
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraSpeed * cameraFront; // forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * cameraFront; // backward
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; // left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; // right

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cout << "W key pressed" << endl;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cout << "S key pressed" << endl;

    // QE keys for upward and downward movement
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPosition += cameraSpeed * cameraUp; // upward
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * cameraUp; // downward

    // Toggle between perspective and orthographic views with the "P" key
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        usePerspective = !usePerspective;
    }
}

void UCreateMesh(GLMesh& mesh, GLfloat* vertices, GLushort* indices, int vertexCount, int indexCount) {
    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerColor = 4;

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * (floatsPerVertex + floatsPerColor) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    mesh.nIndices = indexCount;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLushort), indices, GL_STATIC_DRAW);

    GLint stride = sizeof(GLfloat) * (floatsPerVertex + floatsPerColor);
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    // Update this part for texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(GLfloat) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(GLfloat) * (floatsPerVertex + 2))); // Offset by 2 floats
    glEnableVertexAttribArray(2);
}

void UDestroyMesh(GLMesh& mesh) {
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}

bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId) {
    int success = 0;
    char infoLog[512];

    programId = glCreateProgram();
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    glCompileShader(vertexShaderId);
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        return false;
    }

    glCompileShader(fragmentShaderId);
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        return false;
    }

    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        return false;
    }

    glUseProgram(programId);
    return true;
}

void UDestroyShaderProgram(GLuint programId) {
    glDeleteProgram(programId);
}

void URender() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection;
    if (usePerspective) {
        // Perspective projection
        projection = glm::perspective(glm::radians(zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    }
    else {
        // Orthographic projection
        float orthoSize = 3.0f;  // Adjust the size as needed
        projection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.1f, 100.0f);
    }

    //glm::mat4 projection = glm::perspective(glm::radians(zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp); // Updated view matrix
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 modelViewProjection = projection * view * model;

    // Set light source properties
    glm::vec3 lightDirection(-0.5f, -0.5f, -0.5f); // light direction
    glm::vec3 lightColor(1.0f, 1.0f, 0.0f);       // yellow light color
    float ambientStrength = 0.3f;                // Adjust ambient strength 

    // Set the light source properties as uniforms
    glUniform3fv(glGetUniformLocation(gProgramId, "lightDirection"), 1, glm::value_ptr(lightDirection));
    glUniform3fv(glGetUniformLocation(gProgramId, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform1f(glGetUniformLocation(gProgramId, "ambientStrength"), ambientStrength);

    // Draw the cube
    glm::mat4 modelCube = glm::mat4(1.0f);
    glUseProgram(gProgramId);
    GLint modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCube));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(gCubeMesh.vao);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureId);
    glDrawElements(GL_TRIANGLES, gCubeMesh.nIndices, GL_UNSIGNED_SHORT, nullptr);


    // Draw the cylinder
    glm::mat4 modelCylinder = glm::mat4(1.0f);
    glUseProgram(gProgramId);
    modelLoc = glGetUniformLocation(gProgramId, "model");
    viewLoc = glGetUniformLocation(gProgramId, "view");
    projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCylinder));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(gCylinderMesh.vao);
    glDrawElements(GL_TRIANGLES, gCylinderMesh.nIndices, GL_UNSIGNED_SHORT, nullptr);

    glBindVertexArray(gPlaneMesh.vao);
    glDrawElements(GL_TRIANGLES, gPlaneMesh.nIndices, GL_UNSIGNED_SHORT, nullptr);

    // Draw the sphere
    glm::mat4 modelSphere = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f)); // Position the sphere next to the cube
    glUseProgram(gProgramId); // Use the same shader program as the cube
    modelLoc = glGetUniformLocation(gProgramId, "model");
    viewLoc = glGetUniformLocation(gProgramId, "view");
    projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSphere));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(gSphereMesh.vao);

    glDrawElements(GL_TRIANGLES, gSphereMesh.nIndices, GL_UNSIGNED_SHORT, nullptr);

    // Draw the pyramid
    glm::mat4 modelPyramid = glm::mat4(1.0f);
    modelPyramid = glm::translate(modelPyramid, glm::vec3(-1.5f, 0.0f, 0.0f)); // Move pyramid to the left of the cube
    modelLoc = glGetUniformLocation(gProgramId, "model");
    viewLoc = glGetUniformLocation(gProgramId, "view");
    projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPyramid));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(gPyramidMesh.vao);
    glDrawElements(GL_TRIANGLES, gPyramidMesh.nIndices, GL_UNSIGNED_SHORT, nullptr);

    glfwSwapBuffers(gWindow);
}

