#include "crow_all.h"
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

  char* port = getenv("PORT");
  uint16_t iPort = static_cast<uint16_t>(port != NULL? stoi(port): 18080);
  cout << "PORT = " << iPort << "\n";
  app.port(iPort).multithreaded().run();
}