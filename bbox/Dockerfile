FROM gcc:13

RUN apt-get -qq update
RUN apt-get -qq upgrade
RUN apt-get -qq install cmake

RUN apt-get -qq install --no-install-recommends libboost-all-dev
RUN apt-get -qq install build-essential libtcmalloc-minimal4 && \
  ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so

WORKDIR /usr/src

RUN git clone https://github.com/mongodb/mongo-c-driver.git \
&& cd mongo-c-driver && git checkout 1.10.1 \
&& mkdir cmake-build && cd cmake-build \
&& cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF .. \
&& make && make install && ldconfig /usr/local/lib

RUN curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.10.1/mongo-cxx-driver-r3.10.1.tar.gz && \
    tar -xzf mongo-cxx-driver-r3.10.1.tar.gz && \
    cd mongo-cxx-driver-r3.10.1/build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
    make -j4 && \
    make install && \
    cd / && \
    rm -rf mongo-cxx-driver-r3.10.1.tar.gz mongo-cxx-driver-r3.10.1

ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH