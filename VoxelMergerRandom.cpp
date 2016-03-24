#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <cstdlib>
#include <time.h> 
#include <sstream>

using namespace std;

class Point3D{
public:
	int x,y,z;
	Point3D(){}
	Point3D(int _x,int _y,int _z){
		x = _x;
		y = _y;
		z = _z;
	};
	~Point3D(){}

	bool operator==(const Point3D& other) const{
		return (x == other.x && y == other.y && z == other.z);
	}

	Point3D(const Point3D& other){
		x = other.x;
		y = other.y;
		z = other.z;
	}

	bool operator<(const Point3D& other) const{
		if(x > other.x)
			return false;
		if(y > other.y)
			return false;
		if(z > other.z)
			return false;
		return true;
	}

};


namespace std {

  template <>
  struct hash<Point3D>
  {
    std::size_t operator()(const Point3D& k) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:

      return ((hash<int>()(k.x)
               ^ (hash<int>()(k.y) << 1)) >> 1)
               ^ (hash<int>()(k.z) << 1);
    }
  };
}


unordered_map<Point3D,int> voxels;
int cubes=0;
map<int,pair<Point3D,Point3D> > cuboids;



void findLocalMaxCuboid(Point3D p){
	if(voxels[p])
		return;
	cubes++;
	voxels[p] = cubes;
	int x = p.x;
	int y = p.y;
	int z = p.z;
	int width = 2,height = 2,depth = 2;
	bool change=true;
	int direction[6] = {0,1,2,3,4,5};
	while(change){
		int index = rand()%6;
		for(int k=0;k<6;k++){
			bool temp;
			switch(direction[(index+k)%6]){
				case 0:// -X
				temp=true;
				for(int i=y-height/2+1;i<=y+height/2-1;i++)
					for(int j=z-depth/2+1;j<=z+depth/2-1;j++){
						if(voxels.find(Point3D(x-width/2-1,i,j))!=voxels.end()){
							if(voxels[Point3D(x-width/2-1,i,j)]){
								temp = false;
								break;
							}
						}
						else
							temp=false;
					}
				if(temp){
					for(int i=y-height/2+1;i<=y+height/2-1;i++)
						for(int j=z-depth/2+1;j<=z+depth/2-1;j++)
							if(voxels.find(Point3D(x-width/2-1,i,j))!=voxels.end())
								voxels[(Point3D(x-width/2-1,i,j))] = cubes;
					x--;
					width+=2;
					change = true;
				}
				else change = false;
				break;
				case 1: // +X 
				temp = true;
				for(int i=y-height/2+1;i<=y+height/2-1;i++)
					for(int j=z-depth/2+1;j<=z+depth/2-1;j++)
						if(voxels.find(Point3D(x+width/2+1,i,j))!=voxels.end()){
							if(voxels[Point3D(x+width/2+1,i,j)]){
								temp = false;
								break;
							}
						}
						else
							temp = false;
				if(temp){
					for(int i=y-height/2+1;i<=y+height/2-1;i++)
					 for(int j=z-depth/2+1;j<=z+depth/2-1;j++)
					 	if(voxels.find(Point3D(x+width/2+1,i,j))!=voxels.end())
					 	 	voxels[(Point3D(x+width/2+1,i,j))] = cubes;
					x++;
					width+=2;
					change = true;
				}
				else change = false;
				break;
				case 2: // -Y
				temp = true;
				for(int i=x-width/2+1;i<=x+width/2-1;i++)
					for(int j=z-depth/2+1;j<=z+depth/2-1;j++)
						if(voxels.find(Point3D(i,y-height/2-1,j))!=voxels.end()){
							if(voxels[Point3D(i,y-height/2-1,j)]){
								temp = false;
								break;
							}
						}
						else 
							temp = false;
				if(temp){
					for(int i=x-width/2+1;i<=x+width/2-1;i++)
						for(int j=z-depth/2+1;j<=z+depth/2-1;j++)
							if(voxels.find(Point3D(i,y-height/2-1,j))!=voxels.end())
								voxels[(Point3D(i,y-height/2-1,j))] = cubes;
				
					y--;
					height+=2;
					change = true;
				}
				else change = false;
				break;
				case 3: // +Y
				temp = true;
				for(int i=x-width/2+1;i<=x+width/2-1;i++)
					for(int j=z-depth/2+1;j<=z+depth/2-1;j++)
						if(voxels.find(Point3D(i,y+height/2+1,j))!=voxels.end()){
							if(voxels[Point3D(i,y+height/2+1,j)]){
								temp = false;
								break;
							}
						}
						else
							temp = false;
				if(temp){
					for(int i=x-width/2+1;i<=x+width/2-1;i++)
						for(int j=z-depth/2+1;j<=z+depth/2-1;j++)
							if(voxels.find(Point3D(i,y+height/2+1,j))!=voxels.end())
								voxels[(Point3D(i,y+height/2+1,j))] = cubes;
					y++;
					height+=2;
					change = true;
				}
				else change = false;
				break;
				case 4: // -Z
				temp = true;
				for(int i=x-width/2+1;i<=x+width/2-1;i++)
					for(int j=y-height/2+1;j<=y+height/2-1;j++)
						if(voxels.find(Point3D(i,j,z-depth/2-1))!=voxels.end()){	
							if(voxels[Point3D(i,j,z-depth/2-1)]){
								temp = false;
								break;
							}
						}
						else
							temp = false;
				if(temp){
					for(int i=x-width/2+1;i<=x+width/2-1;i++)
						for(int j=y-height/2+1;j<=y+height/2-1;j++)
							if(voxels.find(Point3D(i,j,z-depth/2-1))!=voxels.end())
								voxels[(Point3D(i,j,z-depth/2-1))] = cubes;
					z--;
					depth+=2;
					change = true;
				}
				else change = false;
				break;
				case 5: // +Z
				temp = true;
				for(int i=x-width/2+1;i<=x+width/2-1;i++)
					for(int j=y-height/2+1;j<=y+height/2-1;j++)
						if(voxels.find(Point3D(i,j,z+depth/2+1))!=voxels.end()){
							if(voxels[Point3D(i,j,z+depth/2+1)]){
								temp = false;
								break;
							}
						}
						else
							temp = false;
				if(temp){
					for(int i=x-width/2+1;i<=x+width/2-1;i++)
						for(int j=y-height/2+1;j<=y+height/2-1;j++)
							if(voxels.find(Point3D(i,j,z+depth/2+1))!=voxels.end())
								voxels[(Point3D(i,j,z+depth/2+1))] = cubes;
					z++;
					depth+=2;
					change = true;
				}
				else change = false;
				break;
			}
			if(change)
				break;
		}
	}
}


void findCuboidDimension(){
	for(unordered_map<Point3D,int>::iterator it = voxels.begin();it!=voxels.end();it++){
		if(cuboids.find(it->second)==cuboids.end())
			cuboids[it->second] = make_pair(it->first,it->first);
		else{
			if(cuboids[it->second].first.x > (*it).first.x) cuboids[it->second].first.x = (*it).first.x;
			if(cuboids[it->second].second.x < (*it).first.x) cuboids[it->second].second.x = (*it).first.x;
			if(cuboids[it->second].first.y > (*it).first.y) cuboids[it->second].first.y = (*it).first.y;
			if(cuboids[it->second].second.y < (*it).first.y) cuboids[it->second].second.y = (*it).first.y;
			if(cuboids[it->second].first.z > (*it).first.z) cuboids[it->second].first.z = (*it).first.z;
			if(cuboids[it->second].second.z < (*it).first.z) cuboids[it->second].second.z = (*it).first.z;
		}
	}
}


void createOBJ(){

	ofstream objfile("project.obj");
  
	for(map<int,pair<Point3D,Point3D> >::iterator it=cuboids.begin();it!=cuboids.end();it++){
		double x = ((*it).second.first.x + (*it).second.second.x)/4.0 ; 
		double y = ((*it).second.first.y + (*it).second.second.y)/4.0 ; 
		double z = ((*it).second.first.z + (*it).second.second.z)/4.0 ; 

		double width = ((*it).second.second.x - (*it).second.first.x + 2)/2.0 ; 
		double height = ((*it).second.second.y - (*it).second.first.y + 2)/2.0 ; 
		double depth = ((*it).second.second.z - (*it).second.first.z + 2)/2.0 ; 

		objfile << "v "<<x-width/2<<" "<<y-height/2<<" "<<z+depth/2<<"\n";
	  	objfile << "v "<<x+width/2<<" "<<y-height/2<<" "<<z+depth/2<<"\n";
	  	objfile << "v "<<x+width/2<<" "<<y+height/2<<" "<<z+depth/2<<"\n";
	  	objfile << "v "<<x-width/2<<" "<<y+height/2<<" "<<z+depth/2<<"\n";
	  	objfile << "v "<<x+width/2<<" "<<y+height/2<<" "<<z-depth/2<<"\n";
	  	objfile << "v "<<x+width/2<<" "<<y-height/2<<" "<<z-depth/2<<"\n";
	  	objfile << "v "<<x-width/2<<" "<<y-height/2<<" "<<z-depth/2<<"\n";
	  	objfile << "v "<<x-width/2<<" "<<y+height/2<<" "<<z-depth/2<<"\n";
	}
	for(int i=0;i<cuboids.size();i++){
		objfile << "f "<<8*i+1<<" "<<8*i+2<<" "<<8*i+3<<" "<<8*i+4<<"\n";
	  	objfile << "f "<<8*i+2<<" "<<8*i+6<<" "<<8*i+5<<" "<<8*i+3<<"\n";
	  	objfile << "f "<<8*i+5<<" "<<8*i+6<<" "<<8*i+7<<" "<<8*i+8<<"\n";
	  	objfile << "f "<<8*i+1<<" "<<8*i+4<<" "<<8*i+8<<" "<<8*i+7<<"\n";
	  	objfile << "f "<<8*i+3<<" "<<8*i+5<<" "<<8*i+8<<" "<<8*i+4<<"\n";
	  	objfile << "f "<<8*i+1<<" "<<8*i+7<<" "<<8*i+6<<" "<<8*i+2<<"\n";
	}
    objfile.close();
}



int main(int argc, char** argv){
	srand (time(NULL));
	ifstream myFile(argv[1]);
	string line;
	while(!myFile.eof()){
		getline(myFile,line);
		if(line.empty())
			continue;
		istringstream iss(line);
		int value;
		Point3D p;
		if(iss >> value)
			p.x = 2*value;
		if(iss >> value)
			p.y = 2*value;
		if(iss >> value)
			p.z = 2*value;
		voxels[p] = 0;
	}
	myFile.close(); 



	for(unordered_map<Point3D,int>::iterator it = voxels.begin();it!=voxels.end();it++)
		findLocalMaxCuboid(it->first);
	cout<<cubes<<endl;

	findCuboidDimension();

	createOBJ();

	return 0;
}