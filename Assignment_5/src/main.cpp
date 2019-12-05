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
bool sizeChange = false;
bool click_state = false;
float temp_p_worldx, temp_p_worldy;
bool allow_rotate = false;
bool cRevert = false;
bool done = false;
bool OKey = false;
bool PKey  = false;
bool drag = false;
bool click_to_drag = false;
bool moveKey = false;
bool RKey = false;
bool lbutton_down = false;
float epsilon = 0.000001;
int start;
// VertexBufferObject wrapper
VertexBufferObject VBO;
int countClick = 0;
bool click1, click2, click3 = false; 
int numOfTriangles = 0;
int line = 0;
//int factor = numOfTriangles * 3;
int numOfVertex = 0;
int numOfLineVertex = 0;
// Contains the vertex positions of triangle
//Eigen::MatrixXf V(2,3);
Eigen::Matrix4f view(4,4);

Eigen::MatrixXf L(2, 4);
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

	if(RKey == false){
		//("%d", "%s", RKey, ' FUCK');
	
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && countClick == 0) {
			//cout << "HOLA\n";
			
			//printf("%d", countClick);
			//cout << "\n";
			//printf("%d", numOfVertex);
			//L.resize(2,4);
			//VBO.update(L);
			//L.resize(2,numOfTriangles*4+4);
			//cout << "HOLA2\n";
			//VBO.update(L);
			//cout << "HOLA3\n";
			T.conservativeResize(2,numOfTriangles*3+3);
			//cout << "HOLA4\n";
			VBO.update(T);
			//cout << "HOLA5\n";

			click1 = true;
			countClick++;
			//cout << "HOLA6\n";
			//V.col(0) << p_world[0], p_world[1];
			//V.col(3) << p_world[0], p_world[1];
			L.col(0) << p_world[0], p_world[1];
			T.col(numOfVertex) << p_world[0], p_world[1];
			
			//V.col(3) << p_world[0], p_world[1];
		}
		
		else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && countClick == 1){
			//cout << "SUGMA\n";
			
			//printf("%d", numOfVertex);
			//cout << "\n";
			click2 = true;
			countClick++;
			L.col(1) << p_world[0], p_world[1];
			L.col(2) << p_world[0], p_world[1];
			T.col(numOfVertex+1) << p_world[0], p_world[1];

		}

		else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && countClick == 2){
			//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);
			//cout << "ADIOS\n";
			
			//printf("%d", numOfVertex);
			//cout << "\n";
			click3 = true;
			L.col(3) << p_world[0], p_world[1];
			//L.col(4) << p_world[0], p_world[1];
			//L.col(5) = L.col(0);
			T.col(numOfVertex+2) << p_world[0], p_world[1];
			numOfLineVertex += 4;
			numOfVertex +=3;
			numOfTriangles++;
			
			

		}

	}


	// Upload the change to the GPU
	VBO.update(L);
	VBO.update(T);
	//VBO.update(V);
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
	//glFlush();
	//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	if(RKey == false){
        if(countClick == 1){
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            
            //glFlush();
            L.col(1) << p_world[0], p_world[1];
			VBO.update(L);
            glDrawArrays(GL_LINES, 0, 2);
            
            //glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);
        }
        else if(countClick == 2){
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            //glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);
            //glFlush();
            L.col(3) << p_world[0], p_world[1];
            VBO.update(L);
            glDrawArrays(GL_LINES, 0, 2);
            glDrawArrays(GL_LINES, 2, 2);
        }

        //glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);
        
    }

	// Upload the change to the GPU
	VBO.update(L);
	//VBO.update(V);
}


float area(float x1, float y1, float x2, float y2, float x3, float y3) { 
   return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0); 
} 


float get_distance(float x1, float y1, float x2, float y2){
	return sqrt(pow(x2 - x1, 2) +  pow(y2 - y1, 2) * 1.0); 
}


bool isInside(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y){
	bool ret = false;
   /* Calculate area of triangle ABC */
	float A = area (x1, y1, x2, y2, x3, y3); 
	//cout << "A ";
	//cout << A;
   /* Calculate area of triangle PBC */   
	float A1 = area (x, y, x2, y2, x3, y3); 
	//cout << "A1 ";
	//cout << A1;
  
   /* Calculate area of triangle PAC */   
	float A2 = area (x1, y1, x, y, x3, y3); 
	//cout << "A2 ";
	//cout << A2;
  
   /* Calculate area of triangle PAB */    
	float A3 = area (x1, y1, x2, y2, x, y); 
	//cout << "A3 ";
	//cout << A3;
    
   /* Check if sum of A1, A2 and A3 is same as A */ 
   	//cout << "FUN 11!\n";
	//cout << A1 + A2 + A3;
	ret = std::fabs(A - (A1 + A2 + A3)) < epsilon;
	//cout << "FUN 12!\n";
	//cout << ret;
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
	//cout << "FUN 21!\n";
	//cout << ret;
	//cout << "FUN 22!\n";
	return ret;
}



void select_obj(GLFWwindow* window, double xPos, double yPos) {
	bool inside = false;
	float closest_v;
	float distance = 1000;
	float clickX, clickY;
	float closeX, closeY;
	int index = 0;
    int col_ret;
    float x1, x2, x3, y1, y2, y3;;
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

	Eigen::Vector4f close;
    // The camera is orthographic, pointing in the direction -z and covering the unit square (-1,1) in x and y
   
    // Convert screen position to world coordinates
    //double xworld = ((xpos/double(width))*2)-1;
    //double yworld = (((height-1-ypos)/double(height))*2)-1; // NOTE: y axis is flipped in glfw

    //std::cerr << xworld << " " << yworld << std::endl;

    // Update the position of the first vertex if the left button is pressed
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		drag = false;
		//cRevert = true;
		click_to_drag = false;
		lbutton_down = false;
		//glDrawArrays(GL_TRIANGLES, start, 3);
		return;
	}
	
	done = false;
	cRevert = false;
	click_to_drag = true;
	click_state = true;
	//printf("%d", "%s", 1, "SUCK MY DICK MOTHER FUCKER\n");
	lbutton_down = true;
	// << "P \n";
	//cout << closest_v;
	//cout << "P \n";
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
		/*
		cout << "H\n";
		cout << closeX;
		cout << "   ";
		cout << closeY;
		cout << " PWORLD  ";
		cout << p_world[0];
		cout << "   ";
		cout << p_world[1];
		*/
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
	
	
	if(drag == false){
		temp_p_worldx = clickX;
		temp_p_worldy = clickY;

		drag = true;
	}

	if(isInside(x1, y1, x2, y2, x3, y3, clickX, clickY)){
		inside = true;

		T.col(col1) << T(0, col1) + p_world[0] - temp_p_worldx, T(1, col1) + p_world[1] - temp_p_worldy;
		T.col(col2) << T(0, col2) + p_world[0] - temp_p_worldx, T(1, col2) + p_world[1] - temp_p_worldy;
		T.col(col3) << T(0, col3) + p_world[0] - temp_p_worldx, T(1, col3) + p_world[1] - temp_p_worldy;
		VBO.update(T);

		temp_p_worldx = p_world[0];
		temp_p_worldy = p_world[1];

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);

		//cout << "\n";
		//cout << T;
	}   

}

void mouse_button_delete(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		bool inside = false;
		float closest_v;
		float distance = 1000;
		float clickX, clickY;
		float closeX, closeY;
		int index = 0;
		int col_ret;
		float x1, x2, x3, y1, y2, y3;;
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

		if(isInside(x1, y1, x2, y2, x3, y3, clickX, clickY) && PKey){
			inside = true;
			T.col(col1) << 0, 0;
			T.col(col2) << 0, 0;
			T.col(col3) << 0, 0;
			VBO.update(T);

			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);	
			glClear(GL_COLOR_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);
		}

		if(isInside(x1, y1, x2, y2, x3, y3, clickX, clickY) && OKey){
			inside = true;
			click_to_drag = true;
			click_state = true;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		click_state = true;
		//click_to_drag = true;
		allow_rotate = true;
		sizeChange = true;
	}
}
void insert_mode(GLFWwindow* window){

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_move);
		
}


void rotate(bool cc){
	float radians;
	cc ? radians = 10*(PI/180) : radians = -10*(PI/180);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	float x = (T(0, start) + T(0, start+1) + T(0, start+2)) / 3;
	float y = (T(1, start) + T(1, start+1) + T(1, start+2)) / 3;
	cout << x;
	cout << "\n";
	cout << y;
	cout << "\n";
	
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



	
	//VBO.update(T);
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
	cout << T;
	cout << "\n";
	
	VBO.update(T);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Update the position of the first vertex if the keys 1,2, or 3 are pressed
	bool cc;
	switch (key) {
		case GLFW_KEY_I:
			allow_rotate = false;
			sizeChange = false;
			click_state = false;
			click_to_drag = false;
			cRevert = false;
			done = false;
			RKey = false;
			OKey = false;
			PKey = false;
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwSetWindowTitle(window, "Entered Insert Mode");
			insert_mode(window);
			break;
		case GLFW_KEY_R:
			
			cRevert = true;	
			done = false;
			glfwSetWindowTitle(window, "[Float] Hello World");
			RKey = true;
			break;
		case GLFW_KEY_O:
			done = false;
			glfwSetWindowTitle(window, "Move Mode");
			//glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);
			glfwSetMouseButtonCallback(window, mouse_button_delete);
			glfwSetCursorPosCallback(window, select_obj);
			RKey = true;
			OKey = true;
			PKey = false;
			moveKey = true;
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
		case GLFW_KEY_P:
			cRevert = true;	
			done = false;
			glfwSetWindowTitle(window, "Delete Mode");
			glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);
			glfwSetMouseButtonCallback(window, mouse_button_delete);
			RKey = false;
			OKey = false;
			PKey = true;
			moveKey = true;
			break;
		default:
			break;
	}

	// Upload the change to the GPU
	
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

	L.resize(2,4);
	//L << 0, 0, 0, 0, //x coordinate
	//  0, 0, 0, 0;//y coordinate

	VBO.update(L);

	T.resize(2,numOfTriangles*3+3);
	//T << 0, 0, 0, //x coordinate
	//  0, 0, 0;//y coordinate
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
		uniform vec3 lineColor;
		out vec4 outColor;
		out vec4 outColorLine;

		void main() {
		    outColor = vec4(triangleColor, 1.0);
			outColorLine = vec4(lineColor, 2.0);
		}
	)";

	// Compile the two shaders and upload the binary to the GPU
	// Note that we have to explicitly specify that the output "slot" called outColor
	// is the one that we want in the fragment buffer (and thus on screen)
	program.init(vertex_shader, fragment_shader, "outColor"); 
	//program.init(vertex_shader, fragment_shader, "outColorLine"); 
	program.bind();

	// The vertex shader wants the position of the vertices as an input.
	// The following line connects the VBO we defined above with the position "slot"
	// in the vertex shader
	program.bindVertexAttribArray("position", VBO);

	// Save the current time --- it will be used to dynamically change the triangle color
	auto t_start = std::chrono::high_resolution_clock::now();

	// Register the keyboard callback
	//glfwSetKeyCallback(window, key_callback);
	//glfwSetKeyCallback(window, key_callback);
	// Register the mouse callback
	
	glfwSetKeyCallback(window, key_callback);
	
	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) { //rendering loop
		/*if(click1 && click2==false){
			mouse_move(window);
		}
		else if(click2 && click3==false){
			mouse_move(window);
		}
		*/
		// Set the size of the viewport (canvas) to the size of the application window (framebuffer)
		int width, height; //generate two return types first
		glfwGetFramebufferSize(window, &width, &height); //get the size of the current window - returns number of pixels
		glViewport(0, 0, width, height); //common place for erris

		// Bind your VAO (not necessary if you have only one)
		VAO.bind();

		// Bind your program
		program.bind();

		// Set the uniform value depending on the time difference
		//auto t_now = std::chrono::high_resolution_clock::now();
		//float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		glUniform3f(program.uniform("triangleColor"), 0.498f, 0.859f, 1.000f);
		//glUniform3f(program.uniform("lineColor"), (float)(sin(time * 4.0f) + 1.0f) / 6.0f, 2.0f, 0.5f);
		

		float aspect_ratio = float(height)/float(width); // corresponds to the necessary width scaling

        view <<
        aspect_ratio,0, 0, 0,
        0,           1, 0, 0,
        0,           0, 1, 0,
        0,           0, 0, 1;

        glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, view.data());

		
		// Clear the framebuffer
		//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);

		// Draw a triangle
		/*
		if(click1 && click2==false){
			glDrawArrays(GL_LINES, line*4, 4);
		}
		else if(click2 && click3==false){
			glDrawArrays(GL_LINES, line*4, 4);
		}
		*/
		

		if(click1 && click2 == false && click3 == false){
					
			//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);
			//glDrawArrays(GL_LINES, 0, 2);
			//glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);
			
			
		}
		if(click1 && click2 && click3 == false){
			
			
			//glDrawArrays(GL_LINES, 0, 2);
			//glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);
			//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);
			//glDrawArrays(GL_LINES, 2, 2);
			//glDrawArrays(GL_LINES, 2, 2);
			
		}
		
		if(click3 && lbutton_down == false){
			//std::cout << "Here is the matrix Tits:\n" << T << std::endl;
			//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);
			//cout << T (1 , 2) ;
			//cout << "FUCK MY LIFE PEEEEENIISSSS";
			//std::cout << T.col(0) << "\n";
			//cout << "FUCK MY LIFE PEEEEENIISSSS";
			//printf("%d", T.cols());
			//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);
			//glUniform3f(program.uniform("triangleColor"), 6.0f, 4.0f, 2.5f);
			glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);
			
		}
		
		if(numOfVertex % 3 == 0 && numOfVertex != 0 && click3){
			//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			
			//std::cout << "Here is the matrix LLLL:\n" << L << std::endl;
			//std::cout << "Here is the matrix LLLL:\n" << L << std::endl;
			//std::cout << "Here is the matrix TTTTT:\n" << T << std::endl;
			//cout << "UGH";
			//printf("%d", T.cols());
			//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);
			//glDrawArrays(GL_TRIANGLES, ((numOfTriangles-1)*3), 3);
			line++;
			click1 = false;
			click2 = false;
			click3 = false;
			//numOfTriangles++;
			countClick = 0;
			
		}

		if(click_to_drag || click_state){
			glUniform3f(program.uniform("triangleColor"), 0.498f, 0.859f, 1.000f);
			glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);

			glUniform3f(program.uniform("triangleColor"), 0.0f, 1.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, start, 3);
		}

		if(cRevert){
			glUniform3f(program.uniform("triangleColor"), 0.498f, 0.859f, 1.000f);
			glDrawArrays(GL_TRIANGLES, 0, numOfTriangles*3);
			
		}
		for(int i = 0; i< numOfTriangles; i++){
			//cout << "1und";
			if(click3){

				//cout << "DAFUCK";
				//glDrawArrays(GL_TRIANGLES, 0, 3);
				//click1 = false;
				//click2 = false;
				//click3 = false;
				//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
				//glClear(GL_COLOR_BUFFER_BIT);
			}
			
			
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