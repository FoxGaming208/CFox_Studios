#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
template <typename T>
class CustomArray {
private:
    T* data;
    size_t size;
    size_t capacity;

    void resize(size_t new_capacity) {
        if (new_capacity < size) {
            new_capacity = size;
        }
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size; i++) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

    void push_back_single(const T& value) {
        if (size == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        data[size++] = value;
    }

public:
    CustomArray() : data(nullptr), size(0), capacity(0) {}

    ~CustomArray() {
        delete[] data;
    }

    template<typename... Args>
    void push_back(Args... args) {
        (void)std::initializer_list<int>{ (push_back_single(args), 0)... };
    }

    void pop_back() {
        if (size > 0) {
            size--;
        }
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    size_t getSize() const {
        return size;
    }

    size_t getCapacity() const {
        return capacity;
    }

    void clear() {
        size = 0;
    }

    bool empty() const {
        return size == 0;
    }

    void reset() {
        delete[] data;
        data = nullptr;
        size = 0;
        capacity = 0;
    }
};

class getCursor {
private:
    static bool wasPressedLastFrame[GLFW_MOUSE_BUTTON_LAST + 1];

public:
    // Static method to check if a specific mouse button was clicked
    static bool clicked(int button) {
        bool isPressed = glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_PRESS;
        bool clicked = isPressed && !wasPressedLastFrame[button];
        wasPressedLastFrame[button] = isPressed;
        return clicked;
    }

    // Static method to check if a specific mouse button is being held down
    static bool held(int button) {
        bool isPressed = glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_PRESS;
        wasPressedLastFrame[button] = isPressed;
        return isPressed;
    }

    // Static method to check if a specific mouse button was released
    static bool released(int button) {
        bool isPressed = glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_PRESS;
        bool released = !isPressed && wasPressedLastFrame[button];
        wasPressedLastFrame[button] = isPressed;
        return released;
    }
};

// Initialize the static member
bool getCursor::wasPressedLastFrame[GLFW_MOUSE_BUTTON_LAST + 1] = { false };

float mapToNormalizedCoordX(float mouseX, int windowWidth) {
    return (mouseX / windowWidth) * 2.0f - 1.0f;
}

float mapToNormalizedCoordY(float mouseY, int windowHeight) {
    return 1.0f - (mouseY / windowHeight) * 2.0f;
}
// -0.5f, -0.5f

class Button {
private:
    float x;
    float y;
    float sizeX;
    float sizeY;
    GLFWwindow* window;

public:
    Button(float PosX, float PosY, float SizeX, float SizeY, GLFWwindow* Window)
        : x(PosX), y(PosY), sizeX(SizeX), sizeY(SizeY), window(Window) {}

    bool hovered() const {
        double Mx, My;
        glfwGetCursorPos(window, &Mx, &My);

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        // Calculate aspect ratio
        float aspectRatio = static_cast<float>(windowWidth) / windowHeight;

        // Convert GLFW coordinates to OpenGL NDC
        glm::vec2 mouseNDC = convertGLFWToOpenGL(Mx, My, windowWidth, windowHeight);

        // Adjust x-coordinate based on the aspect ratio
        float adjustedX = x * aspectRatio;
        float adjustedSizeX = sizeX * aspectRatio;

        // Check if the mouse is within the adjusted button's bounds
        if (mouseNDC.x >= adjustedX && mouseNDC.x <= adjustedX + adjustedSizeX &&
            mouseNDC.y >= y && mouseNDC.y <= y + sizeY) {
            return true;
        }

        return false;
    }

    bool clicked() const {
        return hovered() && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    }

private:
    glm::vec2 convertGLFWToOpenGL(double mouseX, double mouseY, int windowWidth, int windowHeight) const {
        float x = (2.0f * static_cast<float>(mouseX)) / windowWidth - 1.0f;
        float y = 1.0f - (2.0f * static_cast<float>(mouseY)) / windowHeight;
        return glm::vec2(x, y);
    }
};

/* // Converts GLFW window coordinates to OpenGL normalized device coordinates
glm::vec2 convertGLFWToOpenGL(double mouseX, double mouseY, int windowWidth, int windowHeight) {
    float x = (2.0f * static_cast<float>(mouseX)) / windowWidth - 1.0f;
    float y = 1.0f - (2.0f * static_cast<float>(mouseY)) / windowHeight;
    return glm::vec2(x, y);
}

// Converts OpenGL normalized device coordinates to GLFW window coordinates
glm::vec2 convertOpenGLToGLFW(float x, float y, int windowWidth, int windowHeight) {
    double mouseX = (x + 1.0f) * windowWidth / 2.0f;
    double mouseY = (1.0f - y) * windowHeight / 2.0f;
    return glm::vec2(mouseX, mouseY);
}
*/