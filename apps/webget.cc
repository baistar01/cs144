#include "socket.hh"
#include "util.hh"

#include <cstdlib>
#include <iostream>

using namespace std;

void get_URL(const string &host, const string &path) {
    TCPSocket socket;

    // 连接到web服务器
    socket.connect(Address(host, "http"));

    // 创建请求报文，包含 Host 和 Connection 头
    socket.write("GET " + path + " HTTP/1.1\r\n");
    socket.write("Host: " + host + "\r\n");
    socket.write("Connection: close\r\n");           // 关闭连接，避免保持长连接
    socket.write("User-Agent: webget/1.0\r\n\r\n");  // 加上 User-Agent

    // 结束写操作
    socket.shutdown(SHUT_WR);

    // 读取响应报文
    while (!socket.eof()) {
        cout << socket.read();
    }

    // 关闭socket(可以不用手动关闭，socket对象被析构时会自动调用FDWrapper.close()释放文件描述符)
    socket.close();
}

int main(int argc, char *argv[]) {
    try {
        if (argc <= 0) {
            abort();  // For sticklers: don't try to access argv[0] if argc <= 0.
        }

        // The program takes two command-line arguments: the hostname and "path" part of the URL.
        // Print the usage message unless there are these two arguments (plus the program name
        // itself, so arg count = 3 in total).
        if (argc != 3) {
            cerr << "Usage: " << argv[0] << " HOST PATH\n";
            cerr << "\tExample: " << argv[0] << " stanford.edu /class/cs144\n";
            return EXIT_FAILURE;
        }

        // Get the command-line arguments.
        const string host = argv[1];
        const string path = argv[2];

        // Call the student-written function.
        get_URL(host, path);
    } catch (const exception &e) {
        cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
