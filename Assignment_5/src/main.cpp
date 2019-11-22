// This example is heavily based on the tutorial at https://open.gl

////////////////////////////////////////////////////////////////////////////////
// OpenGL Helpers to reduce the clutter
#include "helpers.h"
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
// Linear Algebra Library
#include <Eigen/Dense>
// Timer
#include <chrono>
////////////////////////////////////////////////////////////////////////////////

// VertexBufferObject wrapper
VertexBufferObject VBO;
int countClick = 0;
bool click1, click2, click3 = false; 
// Contains the vertex positions of triangle
//Eigen::MatrixXf V(2,3);
Eigen::Matrix4f view(4,4);

Eigen::MatrixXf L(2, 2);
Eigen::MatrixXf T(2,3);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	// Get viewport size (canvas in number of pixels)
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	// Get the size of the window (may be different than the canvas size on retina displays)
	int width_window, height_window;
	glfwGetWindowSize(window, &width_window, &height_window);

	// Get the position of the mouse in the window
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Deduce position of the mouse in the viewport
	double highdpi = (double) width / (double) width_window;
	xpos *= highdpi;
	ypos *= highdpi;


	Eigen::Vector4f p_screen(xpos,height-1-ypos,0,1);
    Eigen::Vector4f p_canonical((p_screen[0]/width)*2-1,(p_screen[1]/height)*2-1,0,1);
    Eigen::Vector4f p_world = view.inverse()*p_canonical;
	// Convert screen position to world coordinates
	//double xworld = ((xpos/double(width))*2)-1;
	//double yworld = (((height-1-ypos)/double(height))*2)-1; // NOTE: y axis is flipped in glfw

	//std::cerr << xworld << " " << yworld << std::endl;

	// Update the position of the first vertex if the left button is pressed
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && countClick == 0) {
		click1 = true;
		countClick++;
		//V.col(0) << p_world[0], p_world[1];
		//V.col(3) << p_world[0], p_world[1];
		L.col(0) << p_world[0], p_world[1];
		T.col(0) << p_world[0], p_world[1];
		
		
		//V.col(3) << p_world[0], p_world[1];
	}

	else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && countClick == 1){
		click2 = true;
		countClick++;
		L.col(1) << p_world[0], p_world[1];
		L.col(2) << p_world[0], p_world[1];
		T.col(1) << p_world[0], p_world[1];

	}

	else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && countClick == 2){
		click3 = true;
		countClick++;
		L.col(3) << p_world[0], p_world[1];
		L.col(4) << p_world[0], p_world[1];
		L.col(5) = L.col(0);
		T.col(2) << p_world[0], p_world[1];
		

	}



	// Upload the change to the GPU
	VBO.update(L);
	VBO.update(T);
	//VBO.update(V);
}


void mouse_move(GLFWwindow* window) {
	// Get viewport size (canvas in number of pixels)
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	// Get the size of the window (may be different than the canvas size on retina displays)
	int width_window, height_window;
	glfwGetWindowSize(window, &width_window, &height_window);

	// Get the position of the mouse in the window
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Deduce position of the mouse in the viewport
	double highdpi = (double) width / (double) width_window;
	xpos *= highdpi;
	ypos *= highdpi;


	Eigen::Vector4f p_screen(xpos,height-1-ypos,0,1);
    Eigen::Vector4f p_canonical((p_screen[0]/width)*2-1,(p_screen[1]/height)*2-1,0,1);
    Eigen::Vector4f p_world = view.inverse()*p_canonical;

	if(countClick == 1){
		L.col(1) << p_world[0], p_world[1];
	}
	else if(countClick == 2){
		L.col(3) << p_world[0], p_world[1];
	}
	
	

	// Upload the change to the GPU
	VBO.update(L);
	//VBO.update(V);
}



int main(void) {
	// Initialize the GLFW library
	if (!glfwInit()) {
		return -1;
	}

	// Activate supersampling
	glfwWindowHint(GLFW_SAMPLES, 8);

	// Ensure that we get at least a 3.2 context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	// On apple we have to load a core profile with forward compatibility
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a windowed mode window and its OpenGL context
	GLFWwindow * window = glfwCreateWindow(640, 480, "[Float] Hello World", NULL, NULL); //opens a window
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Make the window's context current so every new command works on this window
	glfwMakeContextCurrent(window);

	// Load OpenGL and its extensions
	if (!gladLoadGL()) { 
		printf("Failed to load OpenGL and its extensions");
		return(-1);
	}
	printf("OpenGL Version %d.%d loaded", GLVersion.major, GLVersion.minor);

	int major, minor, rev;
	major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
	minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
	rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
	printf("OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
	printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
	printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Initialize the VAO
	// A Vertex Array Object (or VAO) is an object that describes how the vertex
	// attributes are stored in a Vertex Buffer Object (or VBO). This means that
	// the VAO is not the actual object storing the vertex data,
	// but the descriptor of the vertex data.
	VertexArrayObject VAO; //create and bind a vertexArray object. we only need one of these
	VAO.init();
	VAO.bind();

	// Initialize the VBO with the vertices data
	// A VBO is a data container that lives in the GPU memory
	VBO.init(); //intialize on gpu

	//V.resize(2,6); //one column per vertex
	//V << 0,  0.5, -0.5, 0.5, -0.5, -0.5;
	//V << 0,  0.5, -0.5, 0.1, 0.6, -0.4, //x coordinate
	//  0.5, -0.5, -0.5, 0.6, -0.4, -0.4;//y coordinate
	//VBO.update(V); //sends the matrix to the gpu

	L.resize(2,6);
	L << 0, 0, 0, 0, 0, 0, //x coordinate
	  0, 0, 0, 0, 0, 0;//y coordinate

	
	//VBO.update(L);
	T << 0, 0, 0, //x coordinate
	  0, 0, 0;//y coordinate
	VBO.update(T);
	
	// Initialize the OpenGL Program
	// A program controls the OpenGL pipeline and it must contains
	// at least a vertex shader and a fragment shader to be valid
	Program program;
	//point comes in
	const GLchar* vertex_shader = R"(
		#version 150 core

		in vec2 position;
		uniform mat4 view;
		void main() {
			gl_Position = view * vec4(position, 0.0, 1.0);
		}
	)";

	//assigns constant color to triangle
	const GLchar* fragment_shader = R"(
		#version 150 core

		uniform vec3 triangleColor;
		out vec4 outColor;

		void main() {
		    outColor = vec4(triangleColor, 1.0);
		}
	)";

	// Compile the two shaders and upload the binary to the GPU
	// Note that we have to explicitly specify that the output "slot" called outColor
	// is the one that we want in the fragment buffer (and thus on screen)
	program.init(vertex_shader, fragment_shader, "outColor"); 
	program.bind();

	// The vertex shader wants the position of the vertices as an input.
	// The following line connects the VBO we defined above with the position "slot"
	// in the vertex shader
	program.bindVertexAttribArray("position", VBO);

	// Save the current time --- it will be used to dynamically change the triangle color
	auto t_start = std::chrono::high_resolution_clock::now();

	// Register the keyboard callback
	//glfwSetKeyCallback(window, key_callback);

	// Register the mouse callback
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	
	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) { //rendering loop
		if(click1 && click2==false){
			mouse_move(window);
		}
		else if(click2 && click3==false){
			mouse_move(window);
		}
		
		// Set the size of the viewport (canvas) to the size of the application window (framebuffer)
		int width, height; //generate two return types first
		glfwGetFramebufferSize(window, &width, &height); //get the size of the current window - returns number of pixels
		glViewport(0, 0, width, height); //common place for erris

		// Bind your VAO (not necessary if you have only one)
		VAO.bind();

		// Bind your program
		program.bind();

		// Set the uniform value depending on the time difference
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		glUniform3f(program.uniform("triangleColor"), (float)(sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.1f);
		

		float aspect_ratio = float(height)/float(width); // corresponds to the necessary width scaling

        view <<
        aspect_ratio,0, 0, 0,
        0,           1, 0, 0,
        0,           0, 1, 0,
        0,           0, 0, 1;

        glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, view.data());

		// Clear the framebuffer
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw a triangle
		

		glDrawArrays(GL_LINES, 0, 6);

		if(countClick == 3){
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			
		}
		
		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	// Deallocate opengl memory
	program.free();
	VAO.free();
	VBO.free();

	// Deallocate glfw internals
	glfwTerminate();
	return 0;
}