
FROM ubuntu:18.04
LABEL Maintainer="JGV"

RUN apt-get update
RUN apt-get install -y apt-utils gcc git wget make libncurses-dev flex bison gperf python python-pip python-setuptools python-serial python-cryptography python-future python-pyparsing python-pyelftools curl

WORKDIR /tmp
RUN curl -O 'https://dl.espressif.com/dl/xtensa-esp32-elf-gcc8_2_0-esp32-2019r1-linux-amd64.tar.gz'
RUN tar -xvf xtensa-esp32-elf-gcc8_2_0-esp32-2019r1-linux-amd64.tar.gz
RUN echo 'export PATH=$PATH:/tmp/xtensa-esp32-elf/bin' >> /etc/profile.d/esp32.sh
RUN echo 'export PATH=$PATH:/tmp/xtensa-esp32-elf/bin' >> ~/.bashrc
RUN git clone --recursive https://github.com/espressif/esp-idf.git
RUN echo 'export IDF_PATH=/tmp/esp-idf' >> /etc/profile.d/esp32.sh
RUN echo 'export IDF_PATH=/tmp/esp-idf' >> ~/.bashrc
RUN python -m pip install --user -r /tmp/esp-idf/requirements.txt