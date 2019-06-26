FROM ubuntu:18.04
LABEL Maintainer="JGV"


RUN apt-get update
RUN apt-get install -y apt-utils gcc git wget make libncurses-dev flex bison gperf python python-pip python-setuptools python-serial python-cryptography python-future python-pyparsing python-pyelftools curl libtool autoconf texinfo libusb-dev libftdi1-dev libhidapi-dev pkg-config unzip minicom nano

# openocd esp32

WORKDIR /tmp
RUN curl -LO 'https://github.com/espressif/openocd-esp32/releases/download/v0.10.0-esp32-20190313/openocd-esp32-linux64-0.10.0-esp32-20190313.tar.gz'
RUN tar -xvf openocd-esp32-linux64-0.10.0-esp32-20190313.tar.gz

# IDF

WORKDIR /tmp
RUN git clone -b v3.2.2 --recursive https://github.com/espressif/esp-idf.git
RUN mv esp-idf esp-idf-3.2.2

RUN echo 'export IDF_PATH=/tmp/esp-idf-3.2.2/' >> /etc/profile.d/esp32.sh
RUN echo 'export IDF_PATH=/tmp/esp-idf-3.2.2/' >> ~/.bashrc
RUN python -m pip install --user -r esp-idf-3.2.2/docs/requirements.txt

#gcc-esp

WORKDIR /tmp
RUN curl -O 'https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz'
RUN tar -xvf xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz
RUN echo 'export PATH=/tmp/xtensa-esp32-elf/bin/:$PATH' >> /etc/profile.d/esp32.sh
RUN echo 'export PATH=/tmp/xtensa-esp32-elf/bin/:$PATH' >> ~/.bashrc

RUN rm *.tar.gz

RUN chmod u+x /tmp/esp-idf-3.2.2/add_path.sh
RUN echo '/tmp/esp-idf-3.2.2/add_path.sh' >> ~/.bashrc


WORKDIR /root

RUN mkdir BruCON2019
