# Dockerfile
FROM hello_crow:latest
WORKDIR /usr/src/cppweb/hello_crow/build
CMD ["./hello_crow"]
