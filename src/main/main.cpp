#include <iostream>
#include <fstream>
#include <string>

#include "../utils/Vector.cpp"
#include "../utils/Color.cpp"

using namespace std;

int main() {

    int X = 200;
    int Y = 200;

    Vector vectors[X][Y];

    ofstream newFrame;
    newFrame.open("./frames/frame0.ppm", ios::out);
    newFrame << "P3\n" << X << " " << Y << "\n255\n";

    // Test for outputing frames
    // TODO implement Material class
    for (int i = Y - 1; i >= 0; i--) {
        for (int j = 0; j < X; j++) {

            float r = float(j) / float(X);
            float g = float(i) / float(Y); 
            float b = 0.5;

            int ir = int(255.99*r);    
            int ig = int(255.99*g);
            int ib = int(255.99*b);

            Vector newVector = Vector(float(i), float(j), float(i*j));
            Color color = Color(float(ir), float(ig), float(ib));
            
            newVector.setColor(color);
            vectors[i][j] = newVector;

            newFrame << newVector.color.r << " " << newVector.color.g << " " << newVector.color.b << "\n";   
        }
    }

    newFrame.close();

    // Testing classes and implementation

    for (int i = Y - 1; i >= 0; i--) {
        for (int j = 0; j < X; j++) {
           cout << vectors[i][j].color.r << " " << vectors[i][j].color.g << " " << vectors[i][j].color.b << endl;
        }
    }
    cout << "Number of vectors " << X*Y << endl;
}