# Concurrent C++ HTTP Web Server

A multi-threaded web server implemented in C++ that can handle HTTP requests concurrently. This project aims to provide a robust and efficient solution for serving static and dynamic content over HTTP.

## Features

* Basic HTTP server functionallity supporting GET and POST requests.
* Multi-threaded architecture for handling multiple client connections concurrently.
* Static file serving for HTML, CSS, JS, images and XML files.
* Dynamic content generation using server-side scripting.
* Proper HTTP response generation with status codes, headers and content.

## Getting Started
### Prerequisites

* C++ compiler suppporting C++ 11 or later.
* Boost library for C++ networking.


### Building

1. Clone the repository.
```  
git clone https://github.com/KyleSch8989/CPP-HTTP-SERVER.git
cd CPP-HTTP-SERVER
```
2. Compile the source code:
```
g++ -I "path_to_boost_library" server.cpp -o server -lpthread
```
3. Run the server:
```
./server
```

## General Showcase
The 'test' directory includes some general web pages that can be used to showcase come of the functionallity of the server. These webpages are not built for any specific purpose other than demonstrating the HTTP server functionallity.

### Conecting to test pages
Type the following into a web browser
```
http://localhost:8080
```
```
http://localhost:8080/test/dev.html
```
