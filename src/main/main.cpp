#include <iostream>
#include <fstream>
#include <string>

#include "../utils/Vector.cpp"
#include "../utils/Color.cpp"

using namespace std;

int main() {

    int X = 200;
    int Y = 200;

    ofstream newFrame;
    newFrame.open("./frames/frame0.ppm", ios::out);
    newFrame << "P3\n" << X << " " << Y << "\n255\n";

    //Test for outputing frames
    //TODO implement Color class, Material class
    for (int i = Y - 1; i >= 0; i--) {
        for (int j = 0; j < X; j++) {
            
            float r = float(j) / float(X);
            float g = float(i) / float(Y); 
            float b = 0.5;

            int ir = int(255.99*r);    
            int ig = int(255.99*g);
            int ib = int(255.99*b);

            newFrame << ir << " " << ig << " " << ib << "\n";   
        }
    }

    newFrame.close();

    // Testing classes
    
    Vector vec1 = Vector(1,2,3);
    cout << "X: " << vec1.x << " Y: " << vec1.y << " Z: " << vec1.z << endl;
    Vector vec1_n = vec1.normalized();
    cout << "X: " << vec1_n.x << " Y: " << vec1_n.y << " Z: " << vec1_n.z << endl;

    Color newColor = Color(0, 255, 255);
    vec1_n.setColor(newColor);

    cout << "Red: " << vec1_n.color.r << " Green: " << vec1_n.color.g << " Blue: " << vec1_n.color.b << endl;
}