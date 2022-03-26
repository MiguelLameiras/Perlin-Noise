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

std::vector<std::vector<std::pair<double, double>>> ofApp::PerlinNoiseGradient()
{

    std::vector<std::vector<std::pair<double, double>>> Gradient{w, std::vector<std::pair<double, double>>(h)};

    // Generate grid of vectors
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            Gradient[x][y] = std::make_pair(ofRandom(-1, 1), ofRandom(-1, 1));
            // std::cout << PerlinGradient[x][y].first << " " << PerlinGradient[x][y].second << std::endl;
        }
    }

    return Gradient;
}

double ofApp::PerlinNoise(int x, int y,	int freq, std::vector<std::vector<std::pair<double, double>>> PerlinGradient)
{
    // Variable to hold final result
    double z = 0;

    // Center of the cell and not on corners
    double real_x = x + 0.5;
    double real_y = y + 0.5;

    // Compute dot product between distance vector and gradient vectors
    std::vector<double> dot_product;
    std::vector<std::pair<int, int>> corners;
    dot_product.clear();
    corners.clear();
    // Closest corners to point
    corners.push_back(std::make_pair((int)real_x / freq, (int)real_y / freq));
    corners.push_back(std::make_pair((int)real_x / freq + 1, (int)real_y / freq));
    corners.push_back(std::make_pair((int)real_x / freq, (int)real_y / freq + 1));
    corners.push_back(std::make_pair((int)real_x / freq + 1, (int)real_y / freq + 1));
    // Calculate dot product between de distance vector and gradient vector
    for (int i = 0; i < 4; i++)
    {
        dot_product.push_back(PerlinGradient[corners[i].first][corners[i].second].first * (real_x - corners[i].first * freq) + PerlinGradient[corners[i].first][corners[i].second].second * (real_y - corners[i].second * freq));
        // std::cout << i << " " << dot_product[i] << " " << corners[i].first << " " << corners[i].second << std::endl;
    }

    // Interpolate dot product results -> Bilinear Interpolation

    // Interpolate in x direction
    std::vector<double> f;
    f.clear();
    f.push_back(((corners[1].first * freq - real_x) / (corners[1].first * freq - corners[0].first * freq)) * dot_product[0] + ((real_x - corners[0].first * freq) / (corners[1].first * freq - corners[0].first * freq)) * dot_product[1]);
    f.push_back(((corners[1].first * freq - real_x) / (corners[1].first * freq - corners[0].first * freq)) * dot_product[2] + ((real_x - corners[0].first * freq) / (corners[1].first * freq - corners[0].first * freq)) * dot_product[3]);

    // Interpolate in the y direction
    z = ((corners[0].second * freq - real_y) / (corners[2].second *freq - corners[0].second * freq)) * f[1] + ((real_y - corners[2].second * freq) / (corners[2].second * freq - corners[0].second * freq)) * f[0];

    return z;
}

void ofApp::GenerateMap()
{
    EraseContents("map.txt");

    std::vector<std::vector<std::pair<double, double>>> PerlinGradient = PerlinNoiseGradient();

    std::vector<double> coordinates;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            coordinates.clear();
            coordinates.push_back(x - w / 2);
            coordinates.push_back(y - w / 2);
            coordinates.push_back(2*PerlinNoise(x, y, 20,PerlinGradient) + 1.5*PerlinNoise(x, y, 5,PerlinGradient) + 1*PerlinNoise(x, y, 4,PerlinGradient) + 0.5*PerlinNoise(x, y, 3,PerlinGradient)  + 0.5*PerlinNoise(x, y, 2,PerlinGradient) );

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