#include <iostream>
#include <string>

#include "../utils/Vector.cpp"
using namespace std;

int main() {
    Vector vec1 = Vector(1,2,3);
    cout << "X: " << vec1.x << " Y: " << vec1.y << " Z: " << vec1.z << endl;
    Vector vec1_n = vec1.normalized();
    cout << "X: " << vec1_n.x << " Y: " << vec1_n.y << " Z: " << vec1_n.z << endl;
}