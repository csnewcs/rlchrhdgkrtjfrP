FROM ubuntu:24.04
MAINTAINER csnewcs
LABEL "purpose"="practice"

ARG cert_location=/usr/local/share/ca-certificates
RUN apt update -y
RUN apt install golang-go ca-certificates openssl -y
RUN export PATH=$PATH:/usr/local/go/bin
RUN openssl s_client -showcerts -connect proxy.golang.org:443 </dev/null 2>/dev/null|openssl x509 -outform PEM >  ${cert_location}/proxy.golang.crt
RUN update-ca-certificates
RUN mkdir /app
ADD ./* /app
WORKDIR /app 
RUN GO111MODULE="on" CGO_ENABLED=0 GOOS=linux go build
EXPOSE 8080
CMD ./project
