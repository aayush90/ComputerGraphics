#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <cstdlib>
#include <time.h> 
#include <sstream>
#include "CUBE.h"


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

      return ((hash<int>()(k.x)
               ^ (hash<int>()(k.y) << 1)) >> 1)
               ^ (hash<int>()(k.z) << 1);
    }
  };
}


unordered_map<Point3D,int> voxels;
int cubes=0;
map<int,pair<Point3D,Point3D> > cuboids;
GLfloat **n;
GLint **faces;
GLfloat **v;


void findLocalMaxCuboid(Point3D p){
	if(voxels[p])
		return;
	cubes++;
	voxels[p] = cubes;
	int x_opt = p.x;
	int y_opt = p.y;
	int z_opt = p.z;
	int max_width = 2,max_height = 2,max_depth = 2;

	for(int k=0;k<6;k++){
		int width = 2, height = 2, depth = 2;
		int x = p.x, y = p.y, z = p.z;
		bool temp;
		switch(k){
			case 0:// -X
			temp=true;
			while(temp){
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
					x--;
					width+=2;
				}
			}
			break;
			case 1: // +X 
			temp = true;
			while(temp){
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
					x++;
					width+=2;
				}
			}
			break;
			case 2: // -Y
			temp = true;
			while(temp){
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
					y--;
					height+=2;
				}
			}
			break;
			case 3: // +Y
			temp = true;
			while(temp){
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
					y++;
					height+=2;
				}
			}
			break;
			case 4: // -Z
			temp = true;
			while(temp){
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
					z--;
					depth+=2;
				}
			}
			break;
			case 5: // +Z
			temp = true;
			while(temp){
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
					z++;
					depth+=2;
				}
			}
			break;
		}
		if(width*height*depth > max_width*max_height*max_depth){
			x_opt = x;
			y_opt = y;
			z_opt = z;
			max_width = width;
			max_height = height;
			max_depth = depth;
		}
	}
	
	/*Update voxels*/
	if(max_width>2)
		for(int i=x_opt-max_width/2+1;i<=x_opt+max_width/2-1;i+=2){
			voxels[Point3D(i,y_opt,z_opt)] = cubes;
		}

	if(max_height>2)
		for(int i=y_opt-max_height/2+1;i<=y_opt+max_height/2-1;i+=2){
			voxels[Point3D(x_opt,i,z_opt)] = cubes;
		}

	if(max_depth>2)
		for(int i=z_opt-max_depth/2+1;i<=z_opt+max_depth/2-1;i+=2){
			voxels[Point3D(x_opt,y_opt,i)] = cubes;
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

	ofstream objfile("ds0-5pb.obj");
  
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


pair<double,pair<double,double> > CalculateNormal(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3]){

	double x = (v2[1]-v3[1])*(v1[2]-v2[2]) - (v2[2]-v3[2])*(v1[1]-v2[1]) ;
	double y = (v2[2]-v3[2])*(v1[0]-v2[0]) - (v2[0]-v3[0])*(v1[2]-v2[2]) ;
	double z = (v2[0]-v3[0])*(v1[1]-v2[1]) - (v2[1]-v3[1])*(v1[0]-v2[0]) ;

	return make_pair(x,make_pair(y,z));

}


void initializeArray(){

	n = new GLfloat*[6*cubes];
	faces = new GLint*[6*cubes];
	v = new GLfloat*[8*cubes];

	int i;

	for (i = 0;i < 6*cubes;++i){
		n[i] = new GLfloat[3];
		faces[i] = new GLint[4];
	}
	for (i = 0; i < 8*cubes; ++i){
		v[i] = new GLfloat[3];
	}

  	
  	i=0;
	for(map<int,pair<Point3D,Point3D> >::iterator it=cuboids.begin();it!=cuboids.end();it++){
		double x = ((*it).second.first.x + (*it).second.second.x)/4.0 ; 
		double y = ((*it).second.first.y + (*it).second.second.y)/4.0 ; 
		double z = ((*it).second.first.z + (*it).second.second.z)/4.0 ; 

		double width = ((*it).second.second.x - (*it).second.first.x + 2)/2.0 ; 
		double height = ((*it).second.second.y - (*it).second.first.y + 2)/2.0 ; 
		double depth = ((*it).second.second.z - (*it).second.first.z + 2)/2.0 ; 


	  	v[i][0] = x-width/2 ; v[i][1] = y-height/2 ; v[i][2] = z-depth/2 ;  
	  	v[i+1][0] = x+width/2 ; v[i+1][1] = y-height/2 ; v[i+1][2] = z-depth/2 ;  
	  	v[i+2][0] = x+width/2 ; v[i+2][1] = y+height/2 ; v[i+2][2] = z-depth/2 ;  
	  	v[i+3][0] = x-width/2 ; v[i+3][1] = y+height/2 ; v[i+3][2] = z-depth/2 ;  
	  	v[i+4][0] = x+width/2 ; v[i+4][1] = y+height/2 ; v[i+4][2] = z+depth/2 ;  
	  	v[i+5][0] = x+width/2 ; v[i+5][1] = y-height/2 ; v[i+5][2] = z+depth/2 ;  
	  	v[i+6][0] = x-width/2 ; v[i+6][1] = y-height/2 ; v[i+6][2] = z+depth/2 ;  
	  	v[i+7][0] = x-width/2 ; v[i+7][1] = y+height/2 ; v[i+7][2] = z+depth/2 ;  

	  	i+=8;

	}
	for(i=0;i<6*cubes;i+=6){
		faces[i][0] = 8*i/6 ; faces[i][1] = 8*i/6+1 ; faces[i][2] = 8*i/6+2 ; faces[i][3] = 8*i/6+3 ;
		faces[i+1][0] = 8*i/6+1 ; faces[i+1][1] = 8*i/6+5 ; faces[i+1][2] = 8*i/6+4 ; faces[i+1][3] = 8*i/6+2 ;
		faces[i+2][0] = 8*i/6+4 ; faces[i+2][1] = 8*i/6+5 ; faces[i+2][2] = 8*i/6+6 ; faces[i+2][3] = 8*i/6+7 ;
		faces[i+3][0] = 8*i/6 ; faces[i+3][1] = 8*i/6+3 ; faces[i+3][2] = 8*i/6+7 ; faces[i+3][3] = 8*i/6+6 ;
		faces[i+4][0] = 8*i/6+2 ; faces[i+4][1] = 8*i/6+4 ; faces[i+4][2] = 8*i/6+7 ; faces[i+4][3] = 8*i/6+3 ;
		faces[i+5][0] = 8*i/6 ; faces[i+5][1] = 8*i/6+6 ; faces[i+5][2] = 8*i/6+5 ; faces[i+5][3] = 8*i/6+1 ;
	}

	for(i=0;i<6*cubes;i++){
		pair<double,pair<double,double> > p = CalculateNormal(v[faces[i][0]],v[faces[i][1]],v[faces[i][2]]);	
		n[i][0] = p.first ; n[i][1] = p.second.first ; n[i][2] = p.second.second ;	
	}

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

	findCuboidDimension();

	createOBJ();

	initializeArray();

	project(1,NULL,v,n,faces,cubes);
	cout<<cubes<<endl;

	return 0;
}