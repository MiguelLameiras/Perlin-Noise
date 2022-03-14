#include "ofApp.h"

using namespace std;

void ofApp::EraseContents(string filename)
{
    ofstream FILE;
    FILE.open(filename, ofstream::out | ofstream::trunc);
    FILE.close();
}

void ofApp::WriteToFile(string filename, vector<int> coordinates)
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

double ofApp::PerlinNoise(int x, int y)
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
    corners.push_back(std::make_pair((int)real_x / 10, (int)real_y / 10));
    corners.push_back(std::make_pair((int)real_x / 10 + 1, (int)real_y / 10));
    corners.push_back(std::make_pair((int)real_x / 10, (int)real_y / 10 + 1));
    corners.push_back(std::make_pair((int)real_x / 10 + 1, (int)real_y / 10 + 1));
    // Calculate dot product between de distance vector and distance vector
    for (int i = 0; i < 4; i++)
    {
        dot_product.push_back(PerlinGradient[corners[i].first][corners[i].second].first * (real_x - corners[i].first * 10) + PerlinGradient[corners[i].first][corners[i].second].second * (real_y - corners[i].second * 10));
        // std::cout << i << " " << dot_product[i] << " " << corners[i].first << " " << corners[i].second << std::endl;
    }

    // Interpolate dot product results -> Bilinear Interpolation

    // Interpolate in x direction
    std::vector<double> f;
    f.clear();
    f.push_back(((corners[1].first * 10 - real_x) / (corners[1].first * 10 - corners[0].first * 10)) * dot_product[0] + ((real_x - corners[0].first * 10) / (corners[1].first * 10 - corners[0].first * 10)) * dot_product[1]);
    f.push_back(((corners[1].first * 10 - real_x) / (corners[1].first * 10 - corners[0].first * 10)) * dot_product[2] + ((real_x - corners[0].first * 10) / (corners[1].first * 10 - corners[0].first * 10)) * dot_product[3]);

    // Interpolate in the y direction
    z = ((corners[0].second * 10 - real_y) / (corners[2].second * 10 - corners[0].second * 10)) * f[1] + ((real_y - corners[2].second * 10) / (corners[2].second * 10 - corners[0].second * 10)) * f[0];

    return z;
}

void ofApp::GenerateMap()
{
    EraseContents("map.txt");

    // Generate grid of vectors
    for (int y = 0; y < perlin_h; y++)
    {
        for (int x = 0; x < perlin_w; x++)
        {
            PerlinGradient[x][y] = std::make_pair(ofRandom(-alt, alt), ofRandom(-alt, alt));
            // std::cout << PerlinGradient[x][y].first << " " << PerlinGradient[x][y].second << std::endl;
        }
    }

    std::vector<int> coordinates;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            coordinates.clear();
            coordinates.push_back(x - w / 2);
            coordinates.push_back(y - w / 2);
            coordinates.push_back(PerlinNoise(x, y));

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
    ofSetBackgroundColor(21, 26, 30);
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

    mesh.draw();

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