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

#include <iostream>
using namespace std;
#include <limits>
#include <cmath>
////////////////////////////////////////////////////////////////////////////////
const double PI = std::atan(1.0)*4;
float epsilon = 0.000001;

bool sizeChange = false;
bool click_state = false;
float temp_p_worldx, temp_p_worldy;
bool allow_rotate = false;
bool CKey, OKey, PKey, IKey = false;
bool drag = false;
bool moving = false;
bool moving2 = false;
bool inside = false;
bool color_change = true;
int index_vertex = -1;
int start;
int prev_start = -1;

// VertexBufferObject wrapper
VertexBufferObject VBO;
VertexBufferObject VBO_C;

int countClick = 0;
bool click1, click2, click3 = false; 
int numOfTriangles = 0;
int numOfVertex = 0;

Eigen::Matrix3f temp_color(3,3);

Eigen::Matrix4f view(4,4);

Eigen::MatrixXf T(2,3);
Eigen::MatrixXf C(3,3);


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
	
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && countClick == 0) {
		T.conservativeResize(2,numOfTriangles*3+3);
		C.conservativeResize(3,numOfTriangles*3+3);

		C.col(numOfTriangles*3) << 0.498,  0.859, 1.000;
		C.col(numOfTriangles*3+1) << 0.498,  0.859, 1.000;
		C.col(numOfTriangles*3+2) << 0.498,  0.859, 1.000;
	
		VBO.update(T);
		VBO_C.update(C);
		T.col(numOfTriangles*3) << p_world[0], p_world[1];
		VBO.update(T);
		
		countClick++;
		
		numOfVertex++;
		click1 = true;
	}
	else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && countClick == 1){
		click2 = true;
		countClick++;
		T.col(numOfTriangles*3+1) << p_world[0], p_world[1];
		numOfVertex++;
	}
	else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && countClick == 2){
		click3 = true;
		T.col(numOfTriangles*3+2) << p_world[0], p_world[1];
		numOfVertex++;
		numOfTriangles++;
	}
	
	// Upload the change to the GPU
	VBO.update(T);
}


void mouse_move(GLFWwindow* window, double xPos, double yPos) {
	
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
		moving2 = true;
		T.col(numOfTriangles*3 + 1) << p_world[0], p_world[1];
		VBO.update(T);
	}
	else if(countClick == 2){
		moving = true;
		moving2 = false;
		T.col(numOfTriangles*3 + 2) << p_world[0], p_world[1];
		VBO.update(T);
	}
	// Upload the change to the GPU
	VBO.update(T);
}


float area(float x1, float y1, float x2, float y2, float x3, float y3) { 
   return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0); 
} 


float get_distance(float x1, float y1, float x2, float y2){
	return sqrt(pow(x2 - x1, 2) +  pow(y2 - y1, 2) * 1.0); 
}


bool isInside(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y){
	bool ret = false;

	float A = area (x1, y1, x2, y2, x3, y3);  
	float A1 = area (x, y, x2, y2, x3, y3);  
	float A2 = area (x1, y1, x, y, x3, y3);   
	float A3 = area (x1, y1, x2, y2, x, y); 

	ret = std::fabs(A - (A1 + A2 + A3)) < epsilon;
	return ret; 
} 


int get_which_vertex(int index){
	int ret = -1;

	if(index % 3 == 0){
		ret = 1;
	}
	else if((index - 1) % 3 == 0){
		ret = 2;
	}
	else if((index - 2) % 3 == 0){
		ret = 3;
	}
	return ret;
}


void revert_color(){
	if(prev_start != -1 && color_change == false){
		C.col(prev_start) << temp_color(0, 0), temp_color(1, 0), temp_color(2, 0);
		C.col(prev_start + 1) << temp_color(0, 1), temp_color(1, 1), temp_color(2, 1);
		C.col(prev_start + 2) << temp_color(0, 2), temp_color(1, 2), temp_color(2, 2);
		VBO_C.update(C);
	}
	else if(index_vertex !=-1 && color_change == false){
		C.col(index_vertex) << temp_color(0, 0), temp_color(1, 0), temp_color(2, 0);
		VBO_C.update(C);
	}
}


void handle_click_on_obj(int which, Eigen::Vector4f p_world){
	float closest_v;
	float distance = 1000;
	float clickX, clickY;
	float closeX, closeY;
	int index = 0;
    int col_ret;
    float x1, x2, x3, y1, y2, y3;
	int col1, col2, col3;
    // Get viewport size (canvas in number of pixels)
	
	for(int i = 0; i < T.cols(); i++){
		float temp = get_distance(T(0, i), T(1, i), p_world[0], p_world[1]);
		if(temp < distance){
			clickX = p_world[0];
			clickY = p_world[1];
			distance = temp;
			closeX = T(0, i);
			closeY = T(1, i);
			index = i;
		}
	}
	
	if(get_which_vertex(index) == 1){
		x1 = T(0, index);
		x2 = T(0, index+1);
		x3 = T(0, index+2);
		y1 = T(1, index);
		y2 = T(1, index+1);
		y3 = T(1, index+2);
		col1 = index;
		start = col1;
		col2 = index + 1;
		col3 = index + 2;
	}

	else if(get_which_vertex(index) == 2){
		x1 = T(0, index - 1);
		x2 = T(0, index);
		x3 = T(0, index + 1);
		y1 = T(1, index - 1);
		y2 = T(1, index);
		y3 = T(1, index + 1);
		col1 = index - 1;
		col2 = index;
		start = col1;
		col3 = index + 1;
	}

	else if(get_which_vertex(index) == 3){
		x1 = T(0, index - 2);
		x2 = T(0, index - 1);
		x3 = T(0, index);
		y1 = T(1, index - 2);
		y2 = T(1, index - 1);
		y3 = T(1, index);
		col1 = index - 2;
		col2 = index - 1;
		col3 = index;
		start = col1;
	}

	if(drag == false && which){
		temp_p_worldx = clickX;
		temp_p_worldy = clickY;

		drag = true;
	}

	if(OKey){
		if(isInside(x1, y1, x2, y2, x3, y3, clickX, clickY) && which == 1){
			T.col(col1) << T(0, col1) + p_world[0] - temp_p_worldx, T(1, col1) + p_world[1] - temp_p_worldy;
			T.col(col2) << T(0, col2) + p_world[0] - temp_p_worldx, T(1, col2) + p_world[1] - temp_p_worldy;
			T.col(col3) << T(0, col3) + p_world[0] - temp_p_worldx, T(1, col3) + p_world[1] - temp_p_worldy;
			VBO.update(T);

			temp_p_worldx = p_world[0];
			temp_p_worldy = p_world[1];

			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}

	if(which == 0)
	{
		if(isInside(x1, y1, x2, y2, x3, y3, clickX, clickY) && PKey){
			T.col(col1) << 0, 0;
			T.col(col2) << 0, 0;
			T.col(col3) << 0, 0;
			VBO.update(T);
			}

		if(isInside(x1, y1, x2, y2, x3, y3, clickX, clickY) && OKey){
			revert_color();
			temp_color.col(0) = C.col(col1);
			temp_color.col(1) = C.col(col2);
			temp_color.col(2) = C.col(col3);
			C.col(col1) << 0, 1, 0;
			C.col(col2) << 0, 1, 0;
			C.col(col3) << 0, 1, 0;
			VBO_C.update(C);
			inside = true;
			click_state = true;
			prev_start = start;
		}
		if(isInside(x1, y1, x2, y2, x3, y3, clickX, clickY) == false && OKey){
			revert_color();
			inside = false;
		} 
	}

	if(CKey){
		if(which == 2){
			index_vertex = index;
			temp_color.col(0) = C.col(index);
			C.col(index) << 0.0, 1.0, 0.0;
			VBO_C.update(C);
		}
	}
}


void select_obj(GLFWwindow* window, double xPos, double yPos) {
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

	Eigen::Vector4f close;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		drag = false;
		return;
	}
	click_state = true;
	handle_click_on_obj(1, p_world);
}


void mouse_button_click_select(GLFWwindow* window, int button, int action, int mods) {
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

	Eigen::Vector4f close;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		handle_click_on_obj(0, p_world);
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && inside){
		click_state = true;
		allow_rotate = true;
		sizeChange = true;
	}

	else{
		click_state = false;
		allow_rotate = false;
		sizeChange = false;
	}
}


void rotate(bool cc){
	float radians;
	cc ? radians = 10*(PI/180) : radians = -10*(PI/180);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	float x = (T(0, start) + T(0, start+1) + T(0, start+2)) / 3;
	float y = (T(1, start) + T(1, start+1) + T(1, start+2)) / 3;
	
	float v1d = get_distance(T(0, start), T(1, start), x, y);
	float v2d = get_distance(T(0, start+1), T(1, start+1), x, y);
	float v3d = get_distance(T(0, start+2), T(1, start+2), x, y);

	float v1x = (T(0, start) - x);
	float v1y = (T(1, start) - y);
	float v2x = (T(0, start+1) - x);
	float v2y = (T(1, start+1) - y);
	float v3x = (T(0, start+2) - x);
	float v3y = (T(1, start+2) - y);

	
	T.col(start) << (v1x * cos(radians)) - (v1y * sin(radians)), (v1x * sin(radians)) + (v1y * cos(radians));
	T.col(start+1) << (v2x * cos(radians)) - (v2y * sin(radians)), (v2x * sin(radians)) + (v2y * cos(radians));
	T.col(start+2) << (v3x * cos(radians)) - (v3y * sin(radians)), (v3x * sin(radians)) + (v3y * cos(radians));

	T.col(start) << T(0, start) + x, T(1, start) + y;
	T.col(start+1) << T(0, start+1) + x, T(1, start+1) + y;
	T.col(start+2) << T(0, start+2) + x, T(1, start+2) + y;
	VBO.update(T);
}


void change_size(bool big){
	float factor;
	big ? factor = 1.25 : factor = 0.75;
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	float x = (T(0, start) + T(0, start+1) + T(0, start+2)) / 3;
	float y = (T(1, start) + T(1, start+1) + T(1, start+2)) / 3;

	T.col(start) << factor*(T(0, start) - x) + x, factor*(T(1, start) - y) + y;
	T.col(start+1) << factor*(T(0, start+1) - x) + x, factor*(T(1, start+1) - y) + y;
	T.col(start+2) << factor*(T(0, start+2) - x) + x, factor*(T(1, start+2) - y) + y;
	
	VBO.update(T);
}


void change_color(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		revert_color();
		float closest_v;
		float distance = 1000;
		float clickX, clickY;
		float closeX, closeY;
		int index = 0;
		int col_ret;
		float x1, x2, x3, y1, y2, y3;
		int col1, col2, col3;
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

		handle_click_on_obj(2, p_world);

		}
	}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
		case GLFW_KEY_I:
			revert_color();
			index_vertex = -1;
			IKey = true;
			allow_rotate = false;
			sizeChange = false;
			click_state = false;
			OKey = false;
			PKey = false;
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwSetWindowTitle(window, "Entered Insert Mode");
			glfwSetMouseButtonCallback(window, mouse_button_callback);
			glfwSetCursorPosCallback(window, mouse_move);
			break;
		case GLFW_KEY_O:
			revert_color();
			color_change = false;	
			index_vertex = -1;
			PKey = false;
			OKey = true;
			PKey = false;
			glfwSetWindowTitle(window, "Move Mode");	
			glfwSetMouseButtonCallback(window, mouse_button_click_select);
			glfwSetCursorPosCallback(window, select_obj);
			IKey = false;		
			break;
		case GLFW_KEY_J:
			if(OKey && allow_rotate && click_state && action == GLFW_PRESS){
				rotate(true);			
			}
			break;
		case GLFW_KEY_H:
			if(OKey && allow_rotate && click_state && action == GLFW_PRESS){
				rotate(false);			
			}
			break;
		case GLFW_KEY_K:
			if(OKey && sizeChange && click_state && action == GLFW_PRESS){
				change_size(true);			
			}
			break;
		case GLFW_KEY_L:
			if(OKey && sizeChange && click_state && action == GLFW_PRESS){
				change_size(false);			
			}
			break;
		case GLFW_KEY_C:
			revert_color();
			prev_start = -1;
			color_change = false;
			glfwSetWindowTitle(window, "Color Mode");
			IKey = false;
			OKey = false;
			PKey = false;
			CKey = true;
			glfwSetMouseButtonCallback(window, change_color);
			break;
		case GLFW_KEY_0:
			if(CKey && index_vertex != -1){
				color_change = true;
				C.col(index_vertex) << 1.0, 0.0, 0.0;
				temp_color.col(0) << 1.0, 0.0, 0.0;
				VBO_C.update(C);
			}		
			break;
		case GLFW_KEY_1:
			if(CKey && index_vertex != -1){
				color_change = true;
				C.col(index_vertex) << 1.0, 0.5, 0.0;	
				temp_color.col(0) << 1.0, 0.5, 0.0;	
				VBO_C.update(C);	
			}		
			break;
		case GLFW_KEY_2:
			if(CKey && index_vertex != -1){
				color_change = true;
				C.col(index_vertex) << 1.0, 1.0, 0.0;
				temp_color.col(0) << 1.0, 1.0, 0.0;
				VBO_C.update(C);	
			}		
			break;
		case GLFW_KEY_3:
			if(CKey && index_vertex != -1){
				color_change = true;
				C.col(index_vertex) << 0.0, 0.5, 0.0;
				temp_color.col(0) << 0.0, 0.5, 0.0;	
				VBO_C.update(C);	
			}			
			break;
		case GLFW_KEY_4:
			if(CKey && index_vertex != -1){
				color_change = true;
				C.col(index_vertex) << 0.0, 0.0, 1.0;
				temp_color.col(0) << 0.0, 0.0, 1.0;
				VBO_C.update(C);	
			}		
			break;
		case GLFW_KEY_5:
			if(CKey && index_vertex != -1){
				color_change = true;
				C.col(index_vertex) << 1.0, 0.0, 1.0;
				temp_color.col(0) <<  1.0, 0.0, 1.0;
				VBO_C.update(C);	
			}			
			break;
		case GLFW_KEY_6:
			if(CKey && index_vertex != -1){
				color_change = true;
				C.col(index_vertex) << 0.917647, 0.678431, 0.917647;
				temp_color.col(0) << 0.917647, 0.678431, 0.917647;
				VBO_C.update(C);
			}				
			break;
		case GLFW_KEY_7:
			if(CKey && index_vertex != -1){
				color_change = true;
				C.col(index_vertex) << 0.309804, 0.184314, 0.309804;	
				temp_color.col(0) << 0.309804, 0.184314, 0.309804;	
				VBO_C.update(C);
			}		
			break;
		case GLFW_KEY_8:
			if(CKey && index_vertex != -1){
				color_change = true;
				C.col(index_vertex) << 0.737255, 0.560784, 0.560784;
				temp_color.col(0) << 0.737255, 0.560784, 0.560784;
				VBO_C.update(C);	
			}	
			break;
		case GLFW_KEY_9:
			if(CKey && index_vertex != -1){
				color_change = true;
				C.col(index_vertex) << 0.85, 0.53, 0.10;	
				temp_color.col(0) << 0.85, 0.53, 0.10;	
				VBO_C.update(C);
			}			
			break;
		case GLFW_KEY_P:
			revert_color();
			index_vertex = -1;
			IKey = false;
			OKey = false;
			PKey = true;
			glfwSetWindowTitle(window, "Delete Mode");
			//glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);
			glfwSetMouseButtonCallback(window, mouse_button_click_select);
			break;
		default:
			break;
	}
	
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

	T.resize(2,numOfTriangles*3+3);
	VBO.update(T);

	VBO_C.init();

    C.resize(3,3);
    C <<
    0.498, 0.498, 0.498,
    0.859, 0.859, 0.859,
    1.000, 1.000, 1.000;

    VBO_C.update(C);
	
	// Initialize the OpenGL Program
	// A program controls the OpenGL pipeline and it must contains
	// at least a vertex shader and a fragment shader to be valid
	Program program;
	//point comes in
	const GLchar* vertex_shader = R"(
		#version 150 core

		in vec2 position;
		in vec3 color;
		out vec3 f_color;
		uniform mat4 view;
		void main() {
			gl_Position = view * vec4(position, 0.0, 1.0);
			f_color = color;
		}
	)";

	//assigns constant color to triangle
	const GLchar* fragment_shader = R"(
		#version 150 core
		in vec3 f_color;

		uniform vec3 triangleColor;
		
		out vec4 outColor;
		

		void main() {
		    outColor = vec4(f_color, 1.0);
	
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
	program.bindVertexAttribArray("color",VBO_C);

	// Save the current time --- it will be used to dynamically change the triangle color
	auto t_start = std::chrono::high_resolution_clock::now();

	glfwSetKeyCallback(window, key_callback);
	
	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) { //rendering loop
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
		glUniform3f(program.uniform("triangleColor"), (float)(sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);
		//glUniform3f(program.uniform("lineColor"), (float)(sin(time * 4.0f) + 1.0f) / 6.0f, 2.0f, 0.5f);
		

		float aspect_ratio = float(height)/float(width); // corresponds to the necessary width scaling

        view <<
        aspect_ratio,0, 0, 0,
        0,           1, 0, 0,
        0,           0, 1, 0,
        0,           0, 0, 1;

        glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, view.data());

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT);
		if(IKey)
		{
			if((click1 && click2 == false && moving2) || (click2 && moving == false && click3 == false)){
				glDrawArrays(GL_LINE_LOOP, numOfTriangles*3, 2);
			}
			else if(click2 && moving && click3 == false){
				glDrawArrays(GL_LINE_LOOP, numOfTriangles*3, 3);
			}
		}
		
		if(T.cols() % 3  == 0){
			glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);
		}
		
		if(numOfVertex % 3 == 0 && numOfVertex != 0 && click3){
			click1 = false;
			click2 = false;
			click3 = false;
			moving = false;
			countClick = 0;
			
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
	VBO_C.free();

	// Deallocate glfw internals
	glfwTerminate();
	return 0;
}