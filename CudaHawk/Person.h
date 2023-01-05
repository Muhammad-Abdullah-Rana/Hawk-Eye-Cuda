#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "Location.h" 

class Column {
public:
	Column(int id) : id_(id) {}
	Column() : id_(-1){}
	Column(const Column& c) : id_(c.id_){
		*pixel = *c.pixel;
	}
	
	Column& Empty() { return*this; }
	bool isEmpty() { return id_ == -1; }
	// Getters
	int id() const { return id_; }
	int* getPixel() { return pixel; }
	// Setters
	void setPixel(int r, int g, int b) {
		pixel[0] = r;
		pixel[1] = g;
		pixel[2] = b;
	}
private:
	int id_;
	int pixel[3]{};
};

class Row {
public:
	Row(int id) : id_(id) { cols = std::vector<Column>(); }
	Row(int id, int cid) : id_(id) { 
		Column c(cid);
		cols.push_back(c);
	}
	Row(const Row& r):id_(r.id_) {
		cols = r.cols;
	}

	int id() const { return id_; }
	
	bool add(int col, int* val,int size) {
		if (size == 3) {
			add(col,val[0], val[1], val[2]);
			return true;
		}
		return false;
	}
	void add(int col, int r,int g,int b) {
		sorted = false;

		try {
			for (int i = 0; i < cols.size(); i++) {
				if (cols.at(i).id() == col) {
					cols.at(i).setPixel(r, g, b);
					return;
				}
			}

			Column c(col);
			c.setPixel(r, g, b);
			if(cols.capacity()>cols.size())
				cols.push_back(c);
		}
		catch (std::exception e) {

		}
		
	}
	int size() { return cols.size();}
	Column at(int i) { return cols.at(i);}
	void sort() {
		std::sort(cols.begin(), cols.end(), [](const Column& a, const Column& b) { return a.id() < b.id(); });
	}
	Column getLastColumn() {
		if (cols.empty())
			return Column().Empty();
		if (!sorted)
			sort();
		return cols.at(cols.size() - 1);
	}
	
	
private:
	int id_;
	bool sorted = false;
	std::vector<Column>cols;
};


class Person
{
public:
	Person(int id) : id_(id) {}
	int id() const { return id_; }


	bool add(int row, int col,int *val, int size) {
		if (size == 3) {
			add(row,col, val[0], val[1], val[2]);
			return true;
		}
		return false;
	}
	void add(int row,int col, int r, int g, int b) {
		sorted = false;
		if (!rows.empty()) {
			for (int i = 0; i < rows.size(); i++) {
				if (rows.at(i).id() == row) {
					rows.at(i).add(col, r, g, b);
					return;
				}
			}
		}
	
		Row c(row);
		c.add(col,r, g, b);
		rows.push_back(c);
	}
	Row at(int i) { return rows.at(i); }
	int height() { return rows.size();}
	int width(bool max) {
		int width=(max?0:2000);
		for (auto r = rows.begin(); r != rows.end();++r) {
			if (max) {
				if ((*r).size() > width) {
					width = (*r).size();
				}
			}
			else {
				if ((*r).size() < width) {
					width = (*r).size();
				}
			}
		}
		return width;
	}

	void sort() {
		std::sort(rows.begin(), rows.end(), [](Row a, Row b) { return a.id() < b.id(); });
	}
	Coords feetLocation() {
		return feetLocation(sorted);
	}
	Coords headLocation() {
		return headLocation(sorted);
	}
	Coords xyLocation() {
		return xyLocation(sorted);
	}


	
private:
	int id_;
	bool sorted;
	std::vector<Row>rows;

	Coords feetLocation(bool isSorted) {
		if (rows.empty())
			return Coords().Empty();
		if (!isSorted)
			sort();
		int row = rows.at(rows.size() - 1).id();
		int col = rows.at(rows.size() - 1).getLastColumn().id();
		return Coords(row,col);
	}
	Coords headLocation(bool isSorted) {
		if (rows.empty())
			return Coords().Empty();
		if (!isSorted)
			sort();
		return Coords(rows.at(0).id(), rows.at(0).at(0).id());
	}
	Coords xyLocation(bool isSorted) {
		if (rows.empty())
			return Coords().Empty();
		if (!isSorted)
			sort();
		int row = rows.at(rows.size() - 2).id();
		int col = rows.at(rows.size() - 2).at(rows.size()/2).id();
		return Coords(row, col);
	}

};

