#pragma comment(linker, "/STACK:16777216")
#pragma once
#include "Base.h"
#include "Person.h"
#include "Object.h"
#include "Image.h"


#define DEEP 1
class Point {
public:
	int x, y;
	Point(int i, int j) {
		x = i; y = j;
	}

};

class ObjectIdentification
{
	// it will store the value of object id -1 mean garbage  0 mean not travered and > 0 mean belong to some object
	int*** visited=nullptr;
	Image img;

	Mat _img;

	int rows=0,cols=0;

	std::vector<Person>objs;
	std::vector<Object>_objs;
	std::vector<ORow>_rows;


	Coords camCoords;
	double fov = 68.0;
	double camHeight = 10668;

	void addObject(int id, int x, int y) {
		for (int i = 0; i < _objs.size(); i++) {
			if (_objs[i].getId() == id) {
				_objs[i].add(x, y);
				return;
			}
		}
		Object p(id);
		p.add(x, y);
		_objs.push_back(p);
	}
	void addLinkedObject(int rid, Bounds& b) {
		for (int i = 0; i < _objs.size(); i++) {
			if (_objs[i].hasLinkThenAdd(rid, b)) {
				return;
			}
		}
		Object p(_objs.size()+1);
		p.add(rid, b);
		_objs.push_back(p);
	}
	bool _addPerson(int id, int x, int y) {

		if (objs.size() < id) {
			// new object
			Person p(id);
			p.add(x, y,(int) _img.at<Vec3b>(x, y)[0], (int)_img.at<Vec3b>(x, y)[1], (int)_img.at<Vec3b>(x, y)[2]);
			objs.push_back(p);
			return true;
		}
		else {
			// old object
			(objs.at(id - 1)).add(x, y, (int)_img.at<Vec3b>(x, y)[0], (int)_img.at<Vec3b>(x, y)[1], (int)_img.at<Vec3b>(x, y)[2]);
			return true;
		}
		return false;
	}
	bool isValid(int x, int y, int id) {
		// in the boundry of img or not
		if (x < 0 || x >= img.getHeight() || y < 0 || y >= img.getWidth()) {
			return false;
		}
		if (visited[x][y][0] == 0) {
			// colors are not zero
			bool color = false;
			for (int i = 0; i < img.getChannel(); i++) {
				color = (img.get(x, y, i) == 0);
				if (!color) {
					visited[x][y][1] = id;
					return true;
				}
			}
			visited[x][y][0] = -1;
		}
		return false;
	}
	bool _isValid(int x, int y) {
		// in the boundry of img or not
		if (x < 0 || x >= rows || y < 0 || y >= cols ) {
			return false;
		}
		if ((visited[x][y][0] == 0) && (visited[x][y][1] != 255)) {
			return true;
		}
		visited[x][y][0] = -1;
		return false;;
	}
	int _valid(int x, int y) {
		// in the boundry of img or not
		if (x < 0 || x >= rows || y < 0 || y >= cols) {
			return -1;
		}
		return ((visited[x][y][0] == 0) && (visited[x][y][1]) != 255);
	}
	void removeObject(Object& p,Mat img) {
		for (int a = 0; a < p.getHeight(); a++) {
			ORow r = p.at(a);
			for (int b = 0; b < r.getSize(); b++) {
				Bounds bb = r.getBound(b);
				for (int br = bb.getStart(); br <= bb.getEnd(); br++) {
					img.at<Vec3b>(r.getId(), br) = (Vec3b)((uchar)0, (uchar)0,(uchar) 0);
				}
			}
		}
	}

	/// <summary>
	/// recursive function that find objects
	/// 
	/// * currently not working and throwing error stackoverflow
	/// </summary>
	void _travers(int, int, int );
public:
	ObjectIdentification();

	/// <summary>
	/// set the image in the class for transforming it later
	/// </summary>
	/// <param name="nimg">segmented image </param>
	void _fit(Mat&);

	/// <summary>
	/// travers image/frame and get all object out
	/// </summary>
	/// <return> return objs count in the frame/image</return>
	int _transform();
	int _transform(Mat& img);

	/// <summary>
	/// updated version of _transform()
	/// not completed
	/// </summary>
	void _transform_updated() {

		auto remove = [](vector<Bounds>& vec,int rid) { 
			if (vec.size() > 0) {
				if (vec[vec.size() - 1].width() <= 5) {
					vec.erase(vec.begin() + (vec.size() - 1));
				}
			}
			return 1; 
		};
		int id = 0;
		for (int x = 0; x < _img.rows; x++) {
			ORow r(x);

			for (int y = 0; y < _img.cols; y++) {
				if (_isValid(x, y)) { r.add(y); }
			}

			for (int i = 0; i < r.getSize(); i++) {
				int w = r.getBound(i).width();
				if (w > 5 && w < 20)
					addLinkedObject(r.getId(), r.getBound(i));
				else {

				}
			}

		}

		cout << "Objs before : " << _objs.size();

		// Large type Object removal
		bool clean = false;
		int i = 0;
		while (!clean && (i < _objs.size())) {
			if (_objs[i].getMaxWidth() > 100 || _objs[i].getSize() < 100) {
				_objs.erase(_objs.begin() + i);
				i--;
			}
			if (i + 1 == _objs.size())
				clean = true;
			i++;
		}

		cout << " after: " << _objs.size() << endl;

		// Object Connection



		// Object Removal



		_objs.clear();
		return;
		Location loc;
		Coords geo;
		int x, y;
		for (int i = 0; i < objs.size(); i++) {
			Person p = objs.at(i);

			double inMeter = perPxs(fov, _img.cols) * p.height() * 1000;
			//cout << inMeter << endl;
			if (inMeter < 25 || inMeter > 50) {
				removeObject(p);
			}
		}

		/*

		for (auto a = objs.begin(); a != objs.end();++a) {
			Person p = (*a);
			if (p.height() < 3) {
				removeObject(p);
				objs.erase(a);
			}


			if (false&&p.height() > 3) {


				double perPx = distance(camHeight ,10 ,_img.rows , _img.cols , _img.rows-3);
				double height = p.height() * perPx;
				cout << "height is: "<<height << endl;

			}




			//loc.setCoords(p.xyLocation());
			//geo = loc.getGeoLocation(p.headLocation(),p.feetLocation(),camCoords, fov, camHeight,90);
			//
			//if (!geo.isEmpty()) {
			//	cout << "Geo coordinates are: " << geo.getXcord()<<", "<<geo.getYcord()<<endl;
			//	//break;
			//}
			//else{
			//	cout << "Geo is null: " << loc.getCoords().getXcord() << ", " << loc.getCoords().getYcord() << endl;
			//
			//}
			//
		}
		*/

		objs.clear();
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/*   Slow Implementation   not compeleted */
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// find objects in the imge
	/// <param name="nimg"> new segmented image </param>
	/// <param name="height"> image height </param>
	/// <param name="width"> image width </param>
	/// <param name="channel"> image color format RGB=3 </param>
	/// </summary>
	void fit(int*** nimg, int height, int width, int channel) {
		Image img;
		img.setImage(nimg, height, width, channel);
		fit(img);
	}
	void fit(Image nimg) {
		//if (visited != nullptr) {
		//	for (int i = 0; i < img.getHeight(); i++) {
		//
		//		delete visited[i];
		//	}
		//	delete[] visited;
		//}
		img = nimg;

		//visited = new int* [img.getHeight()];
		//for (int i = 0; i < img.getHeight(); i++) {
		//	visited[i] = new int[img.getWidth()] {};
		//}
	}

	void traverse(int x, int y,int id) {
		// Ignore position if it is not valid or already visited
		if (!isValid(x, y, id)) {
			return;
		}
		//addPerson(id, x, y);

		// Check all neighbors
		traverse(x - 1, y + 1, id);
		traverse(x - 1, y,id);
		traverse(x - 1, y - 1, id);

		traverse(x , y + 1, id);
		traverse(x , y - 1, id);

		traverse(x + 1, y + 1, id);
		traverse(x + 1, y, id);
		traverse(x + 1, y - 1, id);
	}

	
	/// <summary>
	/// clean unwanted onjects
	/// </summary>
	int transform() {
		for (int x = 0; x < img.getHeight(); x++) {
			for (int y = 0; y < img.getWidth(); y++) {
				// If position has a value greater than 0 and is not visited,
				// check its neighbors recursively
				if (isValid(x, y, objs.size() + 1)) {
					traverse(x, y, objs.size() + 1);
				}
			}
		}
		return objs.size();
	}
	void Locate() {
		Person* p;
		Location loc;
		Coords geo;
		int x, y;
		for (auto a = objs.begin(); a != objs.end(); ++a) {
			*p = *a;





			loc.setCoords(p->xyLocation());
			geo = loc.getGeoLocation(p->headLocation(), p->feetLocation(), camCoords, fov, camHeight, 90);
			if (!geo.isEmpty()) {
				cout << "Geo coordinates are: " << geo.getXcord() << ", " << geo.getYcord() << endl;
				break;
			}
		}
	}
	void _Locate() {
		Location loc;
		Coords geo;
		int x, y;
		for (int i = 0; i < objs.size(); i++) {
			Person p = objs.at(i);
		
			double inMeter = perPxs(fov, _img.cols) * p.height()*1000;
			//cout << inMeter << endl;
			if (inMeter < 25 || inMeter > 50) {
				removeObject(p);
			}
		}

		/*
		
		for (auto a = objs.begin(); a != objs.end();++a) {
			Person p = (*a);
			if (p.height() < 3) {
				removeObject(p);
				objs.erase(a);
			}


			if (false&&p.height() > 3) {


				double perPx = distance(camHeight ,10 ,_img.rows , _img.cols , _img.rows-3);
				double height = p.height() * perPx;
				cout << "height is: "<<height << endl;

			}
			
			


			//loc.setCoords(p.xyLocation());
			//geo = loc.getGeoLocation(p.headLocation(),p.feetLocation(),camCoords, fov, camHeight,90);
			//
			//if (!geo.isEmpty()) {
			//	cout << "Geo coordinates are: " << geo.getXcord()<<", "<<geo.getYcord()<<endl;
			//	//break;
			//} 
			//else{
			//	cout << "Geo is null: " << loc.getCoords().getXcord() << ", " << loc.getCoords().getYcord() << endl;
			//
			//}
			//
		}
		*/

		objs.clear();
	}

	void removeObject(Person& p) {
		for (int a = 0; a < p.height(); a++) {
			Row r=p.at(a);
			for (int b = 0; b < r.size(); b++) {
				_img.at<Vec3b>(r.id(), r.at(b).id())=(Vec3b)((uchar)0, (uchar)0, (uchar)0);
			}
		}
	}


	double calculateDistance(int y, double camHeight, double camAngle){
		// calculate distance from camera based on person location in image
		// and camera height and pitch (angle of camera)
		double distance = camHeight / tan(camAngle + atan((y - 240) / 480));
		return distance;
	}
	double possibleHeight(double camHeight, double camAngle, double imgHeight, double imgWidth, double personY) {
		return camHeight / tan(camAngle + atan((personY - imgHeight / 2) / imgWidth)) * tan(atan((personY - imgHeight / 2) / imgWidth) - camAngle);
	}
	double distance(double camHeight, double camAngle, double imgHeight, double imgWidth, double point) {
		return  camHeight / tan(camAngle + atan((point - imgHeight / 2) / imgWidth));
	}
	double perPxs(double fov, double imgWidth) {
		return 1 / (imgWidth / 2) * tan((fov * (M_PI / 180)) / 2);
	}





};

