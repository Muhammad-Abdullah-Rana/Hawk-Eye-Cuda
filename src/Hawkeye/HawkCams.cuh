#pragma once
#include "CameraHandler.cuh"
#include <crow.h>


class HawkCam {
public:
	HawkCam(std::string* paths,int count):_paths(paths),_count(count) {
		handlers = std::vector<CamHandler>();
		checker = std::vector<bool>();
	}
	void loadStreams() {
		for (int i = 0; i < _count; i++) {
			handlers.push_back(CamHandler(_paths[i], i));
			checker.push_back(false);
		}
			
		bool goOn = false;
		while (true) {
			for (int j = 0; j < _count; j++) {
				if (handlers[j].load()) {
					checker[j] = true;
				}
			}
			for (int j = 0; j < _count; j++) {
				if (checker[j]) {
					goOn = true;
				}
				else {
					goOn = false;
					break;
				}
			}
			if (goOn)
				break;
		}
	}
	void setupStreams() {
		for (int i = 0; i < _count; i++) {
			handlers[i].setup();
		}
	}

	void loop(std::unordered_set<crow::websocket::connection*>& clients) {
		bool goOn = false;
		for (int j = 0; j < _count; j++) {
			handlers[j].preloop();
		}

		while (true) {
			for (int j = 0; j < _count; j++) {
				if (handlers[j].loop(clients)) {
					checker[j] = false;
				}
			}
			for (int j = 0; j < _count; j++) {
				if (!checker[j]) {
					goOn = true;
				}
				else {
					goOn = false;
					break;
				}
			}
			if (goOn)
				break;
		}
	}
	void finalize() {
		for (int i = 0; i < _count; i++) {
			handlers[i].destroy();
		}
		handlers.clear();
		_paths = NULL;
	}


private:
	std::string* _paths;
	std::vector<CamHandler> handlers;
	std::vector<bool> checker;
	int _count = 0;
};