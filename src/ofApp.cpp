#include "ofApp.h"

using namespace std;

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetVerticalSync(true);
    ofSetBackgroundColor(12, 17, 23);
    glShadeModel(GL_FLAT);
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    mesh.enableColors();
    //ofEnableAlphaBlending();
    //ofEnableSmoothing();
    ofSetSmoothLighting(true);

    // GUI
    GUI.setup();
    GUI.add(mapwidth.setup("Width", 100, 10, 500));
    GUI.add(mapheight.setup("Height", 100, 10, 500));
    GUI.add(floatslider1.setup("Amplitude 1", A[0], 0, 5));
    GUI.add(intslider1.setup("Frequency 1", F[0], 2, 100));
    GUI.add(floatslider2.setup("Amplitude 2", A[1], 0, 5));
    GUI.add(intslider2.setup("Frequency 2", F[1], 2, 50));
    GUI.add(floatslider3.setup("Amplitude 3", A[2], 0, 5));
    GUI.add(intslider3.setup("Frequency 3", F[2], 2, 40));
    GUI.add(floatslider4.setup("Amplitude 4", A[3], 0, 5));
    GUI.add(intslider4.setup("Frequency 4", F[3], 2, 40));
    GUI.add(floatslider5.setup("Amplitude 5", A[4], 0, 5));
    GUI.add(intslider5.setup("Frequency 5", F[4], 2, 5));

    GenerateMap();

    light.enable();
    light.setPointLight();
    light.setPosition(0,0,100);
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
    mesh.enableNormals();
    cam.begin();
    mesh.enableColors();
    //mesh.drawWireframe();

    mesh.disableColors();
    ofSetColor(137,137,140);
    ofFill();

    ofEnableLighting();
    mesh.drawFaces();
    ofDisableLighting();

    cam.end();

    ofDisableDepthTest();
    GUI.draw();
    ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    // Take a ScreenShot
    if (key == 's')
    {
        img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        img.save("../../output/5_Layers.png", OF_IMAGE_QUALITY_BEST);
    }
    // Generate a new map
    if (key == ' ')
    {
        w = mapwidth;
        h = mapheight;
        A[0] = floatslider1;
        F[0] = intslider1;
        A[1] = floatslider2;
        F[1] = intslider2;
        A[2] = floatslider3;
        F[2] = intslider3;
        A[3] = floatslider4;
        F[3] = intslider4;
        A[4] = floatslider5;
        F[4] = intslider5;
        mesh.clear();
        GenerateMap();
    }
    if (key == 'p')
    {
        std::cout << "\nSaving Plot..." << std::endl;
        system("python3 ../2D_Map.py");
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::EraseContents(string filename)
{
    ofstream FILE;
    FILE.open(filename, ofstream::out | ofstream::trunc);
    FILE.close();
}

void ofApp::WriteToFile(string filename, vector<double> coordinates)
{
    ofstream FILE;
    FILE.open(filename, ios::app);

    if (!FILE)
    {
        cout << "Error: Unable to open the map file!" << endl;
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        FILE << coordinates[i] << " ";
    }

    FILE << endl;
    FILE.close();
}

vector<int> ofApp::ReadFile(string filename, int LINE)
{
    vector<int> coordinates;

    ifstream FILE(filename);
    std::string s;

    for (int i = 1; i <= LINE; i++)
        getline(FILE, s);

    // Construct a stream from the string
    std::stringstream ss(s);
    std::vector<std::string> out;
    std::string temp;
    while (std::getline(ss, temp, ' '))
    {
        out.push_back(temp);
    }
    for (auto &s : out)
    {
        coordinates.push_back(stoi(s));
    }

    FILE.close();

    return coordinates;
}

void ofApp::GenerateMap()
{
    EraseContents("../output/map.txt");

    PerlinNoise P1(w, h, F[0]);
    PerlinNoise P2(w, h, F[1]);
    PerlinNoise P3(w, h, F[2]);
    PerlinNoise P4(w, h, F[3]);
    PerlinNoise P5(w, h, F[4]);

    P1.Generate_Gradient();
    P2.Generate_Gradient();
    P3.Generate_Gradient();
    P4.Generate_Gradient();
    P5.Generate_Gradient();

    std::vector<double> coordinates;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            coordinates.clear();
            coordinates.push_back(x - w / 2);
            coordinates.push_back(y - w / 2);
            coordinates.push_back(A[0] * P1.Compute_Height(x, y) + A[1] * P2.Compute_Height(x, y) + A[2] * P3.Compute_Height(x, y) + A[3] * P4.Compute_Height(x, y) + A[4] * P5.Compute_Height(x, y));

            mesh.addVertex(ofPoint(coordinates[0], coordinates[1], coordinates[2]));
            mesh.addColor(ofFloatColor(62, 99, 86));

            WriteToFile("../output/map.txt", coordinates);
        }
    }

    for (int y = 0; y < h - 1; y++)
    {
        for (int x = 0; x < w - 1; x++)
        {
            mesh.addIndex(x + y * w);       // 0
            mesh.addIndex((x + 1) + y * w); // 1
            mesh.addIndex(x + (y + 1) * w); // 10

            mesh.addIndex((x + 1) + y * w);       // 1
            mesh.addIndex((x + 1) + (y + 1) * w); // 11
            mesh.addIndex(x + (y + 1) * w);       // 10
        }
    }

    setNormals();
}

//--------------------------------------------------------------
void ofApp::setNormals()
{
    for (int i = 0; i < mesh.getNumVertices(); i += 3)
    {

        ofVec3f v0 = mesh.getVertex(i);
        ofVec3f v1 = mesh.getVertex(i + 1);
        ofVec3f v2 = mesh.getVertex(i + 2);

        ofVec3f U = v1 - v0;
        ofVec3f V = v2 - v0;

        float x = (U.y * V.z) - (U.z * V.y);
        float y = (U.z * V.x) - (U.x * V.z);
        float z = (U.x * V.y) - (U.y * V.x);

        ofVec3f normal(x, y, z);

        normal.normalize();

        mesh.addNormal(normal);
        mesh.addNormal(normal);
        mesh.addNormal(normal);
    }
}
