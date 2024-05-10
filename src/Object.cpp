#include "../include/Object.hpp"

Object::Object(string filename) :_filename(filename){
    parseObj(filename);
}

Object::Object(){}

Object::~Object(){}

int Object::parseObj(string resource)
{
    if (!isObjFile(resource)){
        cout << "Not an obj file\n";
        return 0;
    }
    ifstream file(resource.c_str());
    if (!file.is_open()){
        cerr << "probleme open" << endl;
        return 0;
    }
    string line;
    int vertexIndex = 1;
    while (getline(file, line)){
        if (line[0] == 'v'){
            storePoint(line, vertexIndex);
            vertexIndex++;
        }
    }
    ifstream filetwo(resource.c_str());
    while(getline(filetwo,line)){
        if (line[0] == 'f')
            storeFace(line);
    }
    filetwo.close();
    file.close();
    return 1;
}

bool Object::isObjFile(const string filename){
    size_t dotPosition = filename.find_last_of('.');
    if (dotPosition != string::npos && dotPosition < filename.length() - 1){
        string extension = filename.substr(dotPosition + 1);
        if (extension == "obj")
            return true;
    }
    return false;
}

void Object::storePoint(string line, int vertexIndex){
    stringstream pointdata(line);
    string v, x, y, z;
    pointdata >> v >> x >> y >> z;
    _vertices.insert(make_pair(vertexIndex, new Vertex(stod(x), stod(y), stod(z))));
    cout << setprecision(7);
    cout << vertexIndex << " x:" << _vertices[vertexIndex]->getX() << " y:" << _vertices[vertexIndex]->getY() << " z:" << _vertices[vertexIndex]->getZ() << endl;
}

void Object::storeFace(string line){
    stringstream facedata(line);
    string f, a, b, c, d, e;
    facedata >> f >> a >> b >> c >> d >> e;
    _triangles.push_back(new Triangle(_vertices[stoi(a)], _vertices[stoi(b)], _vertices[stoi(c)]));
    if (!d.empty())
        _triangles.push_back(new Triangle(_vertices[stoi(a)], _vertices[stoi(c)], _vertices[stoi(d)]));
    if (!e.empty())
        _triangles.push_back(new Triangle(_vertices[stoi(a)], _vertices[stoi(d)], _vertices[stoi(e)]));
}
