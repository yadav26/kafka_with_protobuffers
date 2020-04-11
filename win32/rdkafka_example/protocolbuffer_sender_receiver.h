#pragma once

#include <ctime>
#include <fstream>
#include <google/protobuf/util/time_util.h>
#include <iostream>
#include <string>

#include "addressbook.pb.h"

#include <sstream>

using namespace std;

using google::protobuf::util::TimeUtil;

using tutorial::AddressBook;


int Reciever(byte*);

int Sender(std::ostringstream& os);

void ShutdownProtoLib();