#pragma once

#include "ofMain.h"

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
	double PerlinNoise(int, int,int,std::vector<std::vector<std::pair<double, double>>>);
	std::vector<std::vector<std::pair<double, double>>> PerlinNoiseGradient();

	void GenerateMap();
	void setNormals(ofMesh &);

	ofEasyCam cam;
	ofLight light;
	ofColor color;
	ofMesh mesh;
	ofImage	img;

private:
	// map size
	int h = 100;
	int w = 100;
};
