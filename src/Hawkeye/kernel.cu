#pragma once
#include "HawkCams.cuh"
#include <crow.h>
#include <unordered_set>
#include <iostream>
#include <thread>
#include <chrono>

void working_code(std::unordered_set<crow::websocket::connection*>& clients) {

    const short int camsCount = 3;
    std::string paths[camsCount] = { "videos\\v1.mp4", "videos\\v2.mp4", "videos\\v3.mp4"};
    HawkCam cams(paths, camsCount);
    cams.loadStreams();
    cams.setupStreams();
    cams.loop(clients);
    cams.finalize();
}

int main() {
    crow::SimpleApp app;

    // Define a set to keep track of connected clients
    std::unordered_set<crow::websocket::connection*> clients;

    std::thread broadcastThread(working_code, std::ref(clients));

    CROW_ROUTE(app, "/image_frame")
        .websocket()
        .onopen([&clients](crow::websocket::connection& conn) {
        clients.insert(&conn);
            std::cout << "New connection established" << std::endl;
        })
        .onclose([&clients](crow::websocket::connection& conn, const std::string& /*reason*/) {
            clients.erase(&conn);
            std::cout << "Connection closed" << std::endl;
        });

        // Start the server on port 8080
        app.port(8080).multithreaded().run();

        // Wait for the broadcast thread to finish (which will never happen)
        broadcastThread.join();

    return 0;
}
