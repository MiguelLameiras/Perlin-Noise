#include "ofApp.h"

using namespace std;

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

    return coordinates;
}

void ofApp::GenerateMap()
{
    EraseContents("map.txt");

    PerlinNoise P1(w,h,20);
    PerlinNoise P2(w,h,5);
    PerlinNoise P3(w,h,4);
    PerlinNoise P4(w,h,3);
    PerlinNoise P5(w,h,2);

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
            coordinates.push_back(2*P1.Compute_Height(x,y) + 1.5*P2.Compute_Height(x,y) + 1*P3.Compute_Height(x,y) + 0.5*P4.Compute_Height(x,y)  + 0.5*P5.Compute_Height(x,y));

            mesh.addVertex(ofPoint(coordinates[0], coordinates[1], coordinates[2]));
            mesh.addColor(ofFloatColor(62, 99, 86));

            WriteToFile("map.txt", coordinates);
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

    setNormals(mesh);
}

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetVerticalSync(true);
    ofSetBackgroundColor(12, 17, 23);
    glShadeModel(GL_FLAT);
    ofSetSmoothLighting(true);

    GenerateMap();
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofEnableLighting();
    ofEnableDepthTest();
    light.enable();
    cam.begin();

    mesh.drawWireframe();

    cam.end();
    // light.disable();
    // ofDisableDepthTest();
    // ofDisableLighting();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    //Take a ScreenShot
    if(key == 'p'){
        img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
        img.save("5_Layers.png");
    }
    //Generate a new map
    if(key == ' '){
        mesh.clear();
        GenerateMap();
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

//--------------------------------------------------------------
void ofApp::setNormals( ofMesh &mesh ){
    //The number of the vertices
    int nV = mesh.getNumVertices();
    //The number of the triangles
    int nT = mesh.getNumIndices() / 3;
    vector<glm::vec3> norm( nV ); //Array for the normals
    //Scan all the triangles. For each triangle add its
    //normal to norm’s vectors of triangle’s vertices
    for (int t=0; t<nT; t++) {
        //Get indices of the triangle t
        int i1 = mesh.getIndex( 3 * t );
        int i2 = mesh.getIndex( 3 * t + 1 );
        int i3 = mesh.getIndex( 3 * t + 2 );
        //Get vertices of the triangle
        const ofPoint &v1 = mesh.getVertex( i1 );
        const ofPoint &v2 = mesh.getVertex( i2 );
        const ofPoint &v3 = mesh.getVertex( i3 );
        //Compute the triangle’s normal
        ofPoint dir = ( (v2 - v1).getCrossed( v3 - v1 ) ).getNormalized();
        //Accumulate it to norm array for i1, i2, i3
        norm[ i1 ] += dir;
        norm[ i2 ] += dir;
        norm[ i3 ] += dir;
    }
    //Normalize the normal’s length
    for (int i=0; i<nV; i++) {
        norm[i] = glm::normalize(norm[i]);
    }
    //Set the normals to mesh
    mesh.clearNormals();
    mesh.addNormals(norm);
    
}