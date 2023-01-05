#pragma once
#include<vector>
#include<algorithm>
#include<iostream>
#include <functional>

using namespace std;

class Bounds {
private:
	int s = -1, e = -1;
public:
	Bounds(){}
	Bounds(int i,int j):s(i),e(j){}
	int width() { return e - s; }
	int getStart() { return s; }
	int getEnd() { return e; }
	void setStart(int s) { this->s = s; }
	void setEnd(int e) { this->e = e; }
	bool add(int v) {
		if (s < 0) {
			s = v;
			e = v;
			return true;
		}
		if (v >= s)
		{
			if (v <= e) {
				return true;
			}
			else {
				if (e + 1 == v) {
					e = v;
					return true;
				}
			}
		}
		else {
			if (s - 1 == v) {
				s = v;
				return true;
			}
		}
		return false;
	}
	bool inRange(int n) {
		return (n >= s || n <= e);
	}
	bool inRange(Bounds& n) {
		if (n.s == s || n.e == e || n.e == s || n.s == e) {
			return true;
		}
		if (n.s < s) {
			return (n.e > s);
		}
		else {
			return (e > n.s);
		}
	}
};

class ORow {
private:
	int _id;
	vector<Bounds> arr;
	ORow(int id, const vector<Bounds>& arr) :_id(id){
		this->arr = arr;
	}

	void setConnections(int i) {
		Bounds a = arr[i];
		for (int j = 0; j < arr.size(); j++) {
			Bounds b = arr[j];
			if (j != i) {
				if (a.getStart() == b.getStart()) {
					if (a.getEnd() < b.getEnd()) {
						// remove a
						remove(i);
					}
					else {
						//remove b
						remove(j);
					}
					return;
				}

				else if (a.getStart() < b.getStart()) {
					if ((a.getEnd() + 1 >= b.getStart())) {
						if ((a.getEnd() < b.getEnd())) {
							a.setEnd(b.getEnd());
						}
						// remove b;
						remove(j);
						setConnections(j);
						return;
					}

				}

				else {
					if ((b.getEnd() + 1 >= a.getStart())) {
						if ((b.getEnd() < a.getEnd())) {
							b.setEnd(a.getEnd());
						}
						// a delete
						remove(i);
						setConnections(j);
						return;
					}
				}
			}
		}
	}

	void remove(int index) {
		arr.erase(arr.begin() + index);
	}

public:
	
	ORow() :_id(-1) {}
	ORow(int id) :_id(id) {}	
	ORow clone() const {
		return ORow(_id,arr);
	}
	
	void add(ORow& row){
		if (row.isNull())
			return;
		for (int i = 0; i < row.getSize(); i++) {
			add(row.arr[i]);
		}
	}

	void add(Bounds& b) {
		// adding current bound if any previous bound can be connected
		for (int i = 0; i < arr.size(); i++) {
			Bounds a = arr[i];
			if (a.getStart() == b.getStart()) {
				if (a.getEnd() < b.getEnd()) {
					a.setEnd(b.getEnd());
					setConnections(i);
				}
				return;
			}

			else if (a.getStart() < b.getStart()) {
				if ((a.getEnd() + 1 >= b.getStart()) && (a.getEnd() + 1 <= b.getEnd())) {
					a.setEnd(b.getEnd());
					setConnections(i);
					return;
				}
			}

			else {
				if ((b.getEnd() + 1 >= a.getStart()) && (b.getEnd() + 1 <= a.getEnd()) ) {
					a.setStart(b.getStart());
					setConnections(i);
					return;
				}
			}

		}

		// adding new bound
		Bounds bb(b.getStart(), b.getEnd());
		arr.push_back(bb);
	}

	int getId() { return _id; }

	bool hasIt(int n) {
		for (int i = 0; i < arr.size(); i++) {
			if (arr[i].inRange(n))
				return true;
		}
		return false;
	}

	int hasLink(int row,int n) {
		if (row + 1 == _id || row - 1 == _id) {
			for (int i = 0; i < arr.size(); i++) {
				if(arr[i].inRange(n)) 
					return true;
			}
		}
		return false;
	}
	int hasLink(int row,Bounds n) {
		if (row + 1 == _id || row - 1 == _id) {
			for (int i = 0; i < arr.size(); i++) {
				if(arr[i].inRange(n)) 
					return true;
			}
		}
		return false;
	}
	Bounds& getBound(int index) {
		return arr[index];
	}
	void add(int v) {
		for (int i = 0; i < arr.size(); i++) {
			if (arr[i].add(v)) {
				return;
			}
		}
		Bounds b;
		b.add(v);
		arr.push_back(b);
	}
	void add(int v ,function<int(vector<Bounds>&)> fun) {
		for (int i = 0; i < arr.size(); i++) {
			if (arr[i].add(v)) {
				return;
			}
		}
		fun(arr);
		Bounds b;
		b.add(v);
		arr.push_back(b);
	}
	int getWidth() {
		int s = 0;
		for (int i = 0; i < arr.size(); i++) {
			s += arr[i].width();
		}
		return s;
	}
	int getGaps() {
		return  arr.size() -1;
	}

	int getSize() { return arr.size(); }
	bool isNull() { return _id < 0 ;}
};


class Object
{
private:
	int _id;
	std::vector<ORow> lines;

public:
	Object(int id) :_id(id) { }
	int getId() { return _id; }

	void add(int rowId, int colId) {
		for (int a = 0; a < lines.size(); a++) {
			if (lines[a].getId() == rowId) {
				lines[a].add(colId);
				return;
			}
		}
		ORow r(rowId);
		r.add(colId);
		lines.push_back(r);
	}
	void add(ORow& r) {
		for (int a = 0; a < lines.size(); a++) {
			if (lines[a].getId() == r.getId()) {
				lines[a].add(r);
				return;
			}
		}
		ORow row = r.clone();
		lines.push_back(row);
	}
	void add(int rid,Bounds& r) {
		for (int a = 0; a < lines.size(); a++) {
			if (lines[a].getId() == rid) {
				lines[a].add(r);
				return;
			}
		}
		ORow row(rid);
		row.add(r);
		lines.push_back(row);
	}
	int getSize() {
		int s = 0;
		for (int a = 0; a < lines.size(); a++) {
			s += lines[a].getSize();
		}
		return s;
	}
	bool hasLinkThenAdd(int rid,Bounds n) {
		for (int i = 0; i < lines.size(); i++) {
			if (lines[i].hasLink(rid, n)) {
				add(rid, n);
				return true;
			}
				
		}
		return false;
	}
	bool hasRow(int id) {
		for (int a = 0; a < lines.size(); a++) {
			if (lines[a].getId() == id) {
				return true;
			}
		}
		return false;
	}
	int getMaxWidth() {
		int width = 0;
		for (int i = 0; i < lines.size(); i++) {
			if (lines[i].getWidth() > width) {
				width = lines[i].getWidth();
			}
		}
		return width;
	}
	int getHeight() {
		return lines.size();
	}
	ORow& at(int index) { return lines.at(index); }

};

