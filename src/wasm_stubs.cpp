// wasm_stubs.cpp — Weak-symbol GLFW stubs for the WebAssembly/Emscripten build.
//
// The emscripten-glfw contrib port (--use-port=contrib.glfw3) does not implement
// every GLFW function. These weak fallbacks satisfy wasm-ld for the missing ones.
// When the port provides a strong symbol the linker picks it; when it doesn't,
// the stub below is used instead.  On non-Emscripten targets this file is empty.

#ifdef __EMSCRIPTEN__
#include <GLFW/glfw3.h>
#include <emscripten.h>
#include <emscripten/html5.h>

extern "C" {

// The browser drives its own event loop via requestAnimationFrame;
// waking it with an empty platform event is a no-op.
__attribute__((weak)) void glfwPostEmptyEvent(void) {}

// No windowing system to shut down in the browser.
__attribute__((weak)) void glfwTerminate(void) {}

// No primary monitor in the browser; return NULL.
__attribute__((weak)) GLFWmonitor* glfwGetPrimaryMonitor(void) { return nullptr; }

// Return a plausible 1920x1080 work area so callers can compute step sizes.
__attribute__((weak)) void glfwGetMonitorWorkarea(GLFWmonitor* /*mon*/,
                                                   int* xpos, int* ypos,
                                                   int* width, int* height) {
    if (xpos)   *xpos   = 0;
    if (ypos)   *ypos   = 0;
    if (width)  *width  = 1920;
    if (height) *height = 1080;
}

// Window position is not meaningful in the browser.
__attribute__((weak)) void glfwGetWindowPos(GLFWwindow* /*win*/,
                                             int* xpos, int* ypos) {
    if (xpos) *xpos = 0;
    if (ypos) *ypos = 0;
}
__attribute__((weak)) void glfwSetWindowPos(GLFWwindow* /*win*/, int /*x*/, int /*y*/) {}

// Map window/framebuffer queries to the Emscripten canvas size.
__attribute__((weak)) void glfwGetWindowSize(GLFWwindow* /*win*/,
                                              int* width, int* height) {
    int w = 0, h = 0;
    emscripten_get_canvas_element_size("#canvas", &w, &h);
    if (width)  *width  = w;
    if (height) *height = h;
}
__attribute__((weak)) void glfwSetWindowSize(GLFWwindow* /*win*/, int w, int h) {
    emscripten_set_canvas_element_size("#canvas", w, h);
}
__attribute__((weak)) void glfwGetFramebufferSize(GLFWwindow* /*win*/,
                                                   int* width, int* height) {
    int w = 0, h = 0;
    emscripten_get_canvas_element_size("#canvas", &w, &h);
    if (width)  *width  = w;
    if (height) *height = h;
}

}  // extern "C"

#endif  // __EMSCRIPTEN__
