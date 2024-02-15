#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <vector>

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

http::response<http::string_body> handleHTML (http::response<http::string_body> response, http::request<http::string_body> request) {
	response.set(http::field::content_type, "text/html");
	
	std::string resource;
	if (request.target() == "/") {
		resource = "index.html";
	} else {
		resource = request.target();
		resource = resource.substr(1);
	}

	//load in html file
	std::ifstream file(resource);
	if (file.is_open()) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		response.body() = buffer.str();
		file.close();
	} else {
		response.body() = "Error: could not open HTML file";
	}
	return response;
}

http::response<http::string_body> handleJS (http::response<http::string_body> response, http::request<http::string_body> request) {
	
	
	response.set(http::field::content_type, "application/javascript");
	
	std::string resource = request.target();
	resource = resource.substr(1);

	//load in JS file
	std::ifstream file(resource);
	if (file.is_open()) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		response.body() = buffer.str();
		file.close();
	} else {
		response.body() = "Error: could not open JS file";
	}
	return response;

}

http::response<http::string_body> handleCSS (http::response<http::string_body> response, http::request<http::string_body> request) {
	
	
	response.set(http::field::content_type, "text/css");
	
	std::string resource = request.target();
	resource = resource.substr(1);

	//load in CSS file
	std::ifstream file(resource);
	if (file.is_open()) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		response.body() = buffer.str();
		file.close();
	} else {
		response.body() = "Error: could not open css file";
	}
	return response;

}

void handleJPEG (http::request<http::string_body> request, tcp::socket& socket) {
	http::response<http::vector_body<char>> response;

	response.version(request.version());
	response.result(http::status::ok);
	response.set(http::field::server, "customHTTPServer - JPEG");
	response.set(http::field::content_type, "image/jpeg");

	std::string resource = request.target();
	resource = resource.substr(1);

	std::ifstream image_file(resource, std::ios::binary);
	if (!image_file.is_open()) {
		std::cout << "Error: could not open JPEG image" << std::endl;
		exit(3);
	}

	image_file.seekg(0, std::ios::end); // set cursor to end of file
	std::size_t image_size = image_file.tellg(); // record position of cursor and use it as the size
	image_file.seekg(0, std::ios::beg); // set cursor back to start
	std::vector<char> image_data(image_size);

	if (!image_file.read(image_data.data(), image_size)) {
		std::cout << "Error: could not read image file" << std::endl;
		exit(4);
	}

	response.body().assign(image_data.begin(), image_data.end());

	response.prepare_payload();
	http::write(socket, response);
}

void handleRequest (http::request<http::string_body>& request, tcp::socket& socket) {
	http::response<http::string_body> response;

	std::cout << "request method: " << request.method_string() << std::endl;
	std::cout << "request target: " << request.target() << std::endl;
	//std::cout << request body:  << request.base() << std::endl;

	response.version(request.version());
	response.result(http::status::ok);
	response.set(http::field::server, "customHTTPServer");
	
	std::string target = request.target();
	if (target.find(".html") != std::string::npos) {
		response = handleHTML(response, request);
		response.prepare_payload();
		http::write(socket, response);
	} else if (target.find(".js") != std::string::npos) {
		response = handleJS(response, request);
		response.prepare_payload();
		http::write(socket, response);
	} else if (target.find(".jpeg") != std::string::npos) {
		handleJPEG(request, socket);
	} else if (target.find(".css") != std::string::npos) {
		response = handleCSS(response, request);
		response.prepare_payload();
		http::write(socket, response);
	} else {
		response = handleHTML(response, request);
		response.prepare_payload();
		http::write(socket, response);
	}
}


void runServer () {

	std::cout << "Server listening on port 8080 ... " << std::endl;

	boost::asio::io_context io_context;
	tcp::acceptor acceptor(io_context, {tcp::v4(), 8080});

	while (true) {
		tcp::socket socket(io_context);
		acceptor.accept(socket);
		boost::beast::flat_buffer buffer;
		http::request<http::string_body> request;
		http::read(socket, buffer, request);
		
		std::cout << "connection accepted" << std::endl;

		handleRequest(request, socket);


		socket.shutdown(tcp::socket::shutdown_send);
	}
}


int main () {
	try {
		runServer();
	} catch (const std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return 0;
}
