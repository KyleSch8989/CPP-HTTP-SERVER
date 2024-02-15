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

http::response<http::string_body> handle500 (http::request<http::string_body> request) {
	http::response<http::string_body> response;
	response.version(request.version());
	response.result(http::status::internal_server_error);
	response.set(http::field::server, "customHTTPServer");
	response.set(http::field::content_length, "0");
	return response;

}

http::response<http::string_body> handleHTML (http::response<http::string_body> response, http::request<http::string_body> request) {
	response.set(http::field::content_type, "text/html");
	
	std::string resource;
	if (request.target() == "/") {
		resource = "test/index.html";
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
		response = handle500(request);
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
		response = handle500(request);
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
		response = handle500(request);
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
		std::cout << "Could not open JPEG image" << std::endl;
		http::response<http::string_body> stringResponse;
		stringResponse = handle500(request);
		stringResponse.prepare_payload();
		http::write(socket, stringResponse);
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

http::response<http::string_body> handleXML (http::response<http::string_body> response, http::request<http::string_body> request) {
	
	
	response.set(http::field::content_type, "application/XML");
	
	std::string resource = request.target();
	resource = resource.substr(1);

	//load in XML file
	std::ifstream file(resource);
	if (file.is_open()) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		response.body() = buffer.str();
		file.close();
	} else {
		response.body() = "Error: could not open XML file";
	}
	return response;

}

http::response<http::string_body> handleFeedback (http::request<http::string_body> request) {
	http::response<http::string_body> response;
	response.version(request.version());
	response.result(http::status::no_content);
	response.set(http::field::server, "customHTTPServer");
	response.set(http::field::content_length, "0");

	std::cout << request.body() << std::endl;

	return response;

}

http::response<http::string_body> handle404 (http::request<http::string_body> request) {
	http::response<http::string_body> response;
	response.version(request.version());
	response.result(http::status::not_found);
	response.set(http::field::server, "customHTTPServer");
	response.set(http::field::content_length, "0");

	return response;

}

void handleRequest (http::request<http::string_body>& request, tcp::socket& socket) {
	http::response<http::string_body> response;

	std::cout << "request method: " << request.method_string() << std::endl;
	std::cout << "request target: " << request.target() << std::endl;
	//std::cout << request body:   << request.base() << std::endl;

	response.version(request.version());
	response.result(http::status::ok);
	response.set(http::field::server, "customHTTPServer");
	
	std::string target = request.target();
	if (request.method_string() == "GET") {
		if (target == "/") {
			response = handleHTML(response, request);
			response.prepare_payload();
			http::write(socket, response);
			std::cout << "bosh" << std::endl;
		} else if (target.find(".html") != std::string::npos) {
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
		} else if (target.find(".xml") != std::string::npos) {
			response = handleXML(response, request);
			response.prepare_payload();
			http::write(socket, response);
		} else {
			//handle unknown target
			response = handle404(request);
			response.prepare_payload();
			http::write(socket, response);
		}
	} else if (request.method_string() == "POST") {
		if (target.find("feedback") != std::string::npos) {
			response = handleFeedback(request);
			response.prepare_payload();
			http::write(socket, response);
		} else {
			std::cout << "Unknown POST target." << std::endl;
		}
	}

	socket.shutdown(tcp::socket::shutdown_send);
}


void runServer () {

	std::cout << "Server listening on port 8080 ... " << std::endl;

	boost::asio::io_context io_context;
	tcp::acceptor acceptor(io_context, {tcp::v4(), 8080});
	
	boost::asio::thread_pool thread_pool(4);

	while (true) {
		tcp::socket socket(io_context);
		acceptor.accept(socket);
		boost::beast::flat_buffer buffer;
		http::request<http::string_body> request;
		try {	
			http::read(socket, buffer, request);
		} catch (std::exception& e) {
			continue;
		}
		post(thread_pool, [socket = std::move(socket), request = std::move(request)]() mutable {
				handleRequest(request, socket);
		});
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
