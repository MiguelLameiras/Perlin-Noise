#pragma once

#include "ofMain.h"
#include "PerlinNoise.h"
#include "ofxGui.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <cmath>

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void gotMessage(ofMessage msg);

	void EraseContents(string filename);
	void WriteToFile(string, std::vector<double>);
	std::vector<int> ReadFile(string, int);

	void GenerateMap();
	void calculateNormals();

	ofEasyCam cam;
	ofLight light;
	ofColor color;
	ofVboMesh mesh;
	ofImage img;

	// GUI

	ofxPanel GUI;

	ofxIntSlider intslider1,intslider2,intslider3,intslider4,intslider5,mapwidth,mapheight,water_offset_slider;
	ofxFloatSlider floatslider1,floatslider2,floatslider3,floatslider4,floatslider5;

private:
	// map size
	int h = 100;
	int w = 100;
	int water_offset = 10;
	double A[5] = {2, 1.5, 1, 0.5, 0.5}; // Variables to hold amplitude value
	int F[5] = {20,5,4,3,2};//Variables to hold frequency value
};
