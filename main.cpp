#include "crow_all.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <boost/filesystem.hpp>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/oid.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::basic::kvp;
using mongocxx::cursor;

using namespace std;
using namespace crow;

const string BASE_PATH = "/usr/src/cppweb/hello_crow/public/";
void sendFile(response &res, const string &filename, const string &contentType) {
  ifstream in(BASE_PATH + filename, ifstream::in);
  if(in) {
    ostringstream contents;
    contents << in.rdbuf();
    in.close();
    res.set_header("Content-Type", contentType);
    res.write(contents.str());
  } else {
    res.code = 404;
    res.write("Not found");
  }
  res.end();
}

void sendHtml(response &res, const string &filename) {
  sendFile(res, filename + ".html", "text/html");
}

void sendImage(response &res, const string &filename) {
  sendFile(res, "images/"+filename, "image/jpeg");
}

void sendStyle(response &res, const string &filename) {
  sendFile(res, "styles/"+filename, "text/css");
}

void sendScript(response &res, const string &filename) {
  sendFile(res, "scripts/"+filename, "text/javascript");
}

int main(int argc, char* argv[]) {
  crow::SimpleApp app;

  /*Mongo Connection*/
  mongocxx::instance inst{};
  const char* mongoConnect = getenv("MONGODB_URI");
  if (!mongoConnect) {
    cerr << "Error: MONGODB_URI environment variable is not set." << endl;
    return 1;
  }

  mongocxx::client conn{mongocxx::uri{mongoConnect}};
//  auto collection = conn["testdb"]["testcollection"];
  auto collection = conn["Contacts_db"]["Contacts"];

  CROW_ROUTE(app, "/styles/<string>")
  ([](const request &req, response &res, const string &filename) ->void {
    sendStyle(res, filename);
  });

  CROW_ROUTE(app, "/scripts/<string>")
  ([](const request &req, response &res, const string &filename) ->void {
  sendScript(res, filename);
  });

  CROW_ROUTE(app, "/images/<string>")
  ([](const request &req, response &res, const string &filename) ->void {
  sendImage(res, filename);
  });

  CROW_ROUTE(app, "/")
  ([](const request &req, response &res) ->void {
    sendHtml(res, "index");
  });

  CROW_ROUTE(app, "/about")
  ([](const request &req, response &res) ->void {
    sendHtml(res, "about");
  });

  CROW_ROUTE(app, "/contact")
  ([&collection](){
    mongocxx::options::find opts;
    opts.limit(10);
    auto docs = collection.find({}, opts);
    std::ostringstream os;
    for(auto doc : docs) {
      os << bsoncxx::to_json(doc) << std::endl;
    }
    return crow::response(os.str());
  });
  char* port = getenv("PORT");
  uint16_t iPort = static_cast<uint16_t>(port != NULL? stoi(port): 18080);
  cout << "PORT = " << iPort << "\n";
  app.port(iPort).multithreaded().run();
}