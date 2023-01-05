#include "ObjectIdentification.h"

ObjectIdentification::ObjectIdentification() { camCoords.setCoords(31.451747, 74.292998); }


void ObjectIdentification::_fit(Mat& nimg) {
	if (visited != nullptr) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++)
				delete visited[i][j];
			delete visited[i];
		}
		delete[] visited;
	}
	
	rows= nimg.rows;
	cols= nimg.cols;

	visited = new int** [rows];
	for (int i = 0; i < rows; i++) {
		visited[i] = new int* [cols]; 
		for (int j = 0; j < cols; j++) {
			visited[i][j] = new int[2]{};
			visited[i][j][1] = (int) nimg.at<Vec3b>(i, j)[0];
		}
	}
}

int currId=0;
int wId = 0;
void ObjectIdentification::_travers(int x, int y, int id) {
	// Ignore position if it is not valid or already visited
	if (_isValid(x, y)) {
		currId = id;
		visited[x][y][0] = id;
		_travers(x + 1, y, id); // right
		_travers(x - 1, y, id); // left
		_travers(x, y + 1, id); // up
		_travers(x, y - 1, id); // down
	}

	//addObject(id, x, y);
}

int ObjectIdentification::_transform() {
	int ids = 0;

	// assign ids to pixels
	for (int x = 0; x < rows; x++) {
		for (int y = 0; y < cols; y++) {
			// If position has a value greater than 0 and is not visited,
			// check its neighbors recursively
			_travers(x, y, ids);
			if (currId == ids) {
				ids++;	
			}
			currId = 0;

			wId = visited[x][y][0];
			if (wId > 0) {
				addObject(currId, x, y);
			}
		}
	}
	// form objects
	for (int x = 0; x < _objs.size(); x++) {
		if (_objs[x].getMaxWidth() > 50) {

		}
	}




	return objs.size();
}
int ObjectIdentification::_transform(Mat& img) {
	int ids = 0;

	for (int x = 0; x < rows; x++) {
		for (int y = 0; y < cols; y++) {
			// If position has a value greater than 0 and is not visited,
			// check its neighbors recursively
			// assign ids to pixels


			_travers(x, y, ids);
			if (currId == ids) {
				ids++;
			}
			currId = 0;

		}
	}
	for (int x = 0; x < rows; x++) {
		for (int y = 0; y < cols; y++) {
			// add in person
			wId = visited[x][y][0];
			if (wId > 0) {
				addObject(currId, x, y);
			}
		}
	}
	// form objects
	for (int x = 0; x < _objs.size(); x++) {
		if (_objs[x].getSize() < 50 || _objs[x].getHeight() > 50 || _objs[x].getMaxWidth() > 50)
			removeObject(_objs[x],img);
	}
	return objs.size();
}


