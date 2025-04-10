FROM enderthecoder/oatpp-dev
COPY ./ /libimousepp/
RUN mkdir libimousepp/build
WORKDIR /libimousepp/build
RUN cmake ..
RUN cmake --build . -j
RUN cmake --install .

