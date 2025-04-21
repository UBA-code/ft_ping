FROM debian:stable

WORKDIR /work

RUN apt update && apt install wget git build-essential -y

RUN wget https://ftpmirror.gnu.org/inetutils/inetutils-2.0.tar.gz 

RUN tar -xvf inetutils-2.0.tar.gz

RUN cd inetutils-2.0 && \
	./configure && \
	make

COPY . /work/ft_ping

CMD [ "tail", "-f",  "/dev/null"]
