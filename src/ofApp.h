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
	void WriteToFile(string, std::vector<int>);
	std::vector<int> ReadFile(string, int);
	double PerlinNoise(int, int);

	void GenerateMap();
	void setNormals(ofMesh &);

	ofEasyCam cam;
	ofLight light;
	ofColor color;
	ofMesh mesh;

private:
	// map size
	int h = 100;
	int w = 100;
	int freq = 10;
	double alt = 1;//Intensity of terrain
	int perlin_h = h / freq + 1;
	int perlin_w = w / freq + 1;
	std::vector<std::vector<std::pair<double, double>>> PerlinGradient{perlin_w, std::vector<std::pair<double, double>>(perlin_h)};
};
