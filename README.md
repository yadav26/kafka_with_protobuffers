This code has reference to many github projects.
Intention is to use existing code and integrate google protobuffers to send message to kafka server and read back and interpret back to raw messages.



librdkafka - [Magnus Edenhill](http://www.edenhill.se/).



Kafka - 
install cmake
build zlip , install path to bin, libs will be picked automatically
build zookeeper 
build kaka

build proto-buffer library, install path to env.path
then build 
build openssl or get prebuilt binaries,
clone solution 
build  librdkafkacpp,
build example
only x64 builds, build32 needs proto zlib openssl etc in 32 libraries.


to make build32 configuration files, used below flags , but not successful still has __declspec dllimport linking error. so removed win32 configurations from rdkafka_example.exe

	cmake -G "Visual Studio 16 2019" ^
	  -DCMAKE_INSTALL_PREFIX=../../../../install32 ^
	  -DCMAKE_GENERATOR_PLATFORM=Win32 ^
	  -B build32 ^
	  ../..
	  
	  cmake -G "Visual Studio 16 2019" ^
	  -DCMAKE_INSTALL_PREFIX=../../../../install64 ^
	  -DCMAKE_GENERATOR_PLATFORM=x64 ^
	  -B build64 ^
	  ../..
	  
	  cmake --build build32 --config Release
	  cmake --build build32 --config Debug
	  cmake --build build64 --config Release
	  cmake --build build64 --config Debug
	  
	  
	  
USAGE - 
Run zookeeper - 
zookeeper-server-start.bat config/zookeeper.properties

Run kafka server - 
kafka-server-start.bat config/server.properties

kafka-topics.bat --create --bootstrap-server localhost:9092 --replication-factor 1 --partitions 1 --topic test
kafka-topics.bat --list --bootstrap-server localhost:9092

To run demo producer-
kafka-console-producer.bat --broker-list localhost:9092 --topic test

To run demo consumer-
kafka-console-consumer.bat --bootstrap-server localhost:9092 --topic test --from-beginning



----------------------------------------------------------------------
Run rd
----------------------------------------------------------------------
-P -t demoKafka -p 0 -b localhost:9092
----------------------------------------------------------------------
-C -t demoKafka -p 0 -b localhost:9092 -o 0


---------------------------------------------------------------------------------------
To run proto buffer .proto to .h and .c
protoc -I=$(ProjectDir) --cpp_out=$(ProjectDir) $(ProjectDir)/addressbook.proto
This is to compile the protobuf file.
---------------------------------------------------------------------------------------
