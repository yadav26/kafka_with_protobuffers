// See README.txt for information and build instructions.

#include <ctime>
#include <fstream>
#include <google/protobuf/util/time_util.h>
#include <iostream>
#include <string>
#include <sstream>

#include "addressbook.pb.h"


std::ostringstream os;


using namespace std;

using google::protobuf::util::TimeUtil;


// This function fills in a Person message based on user input.
void PromptForAddress(tutorial::Person* person) {
    cout << "Enter person ID number: ";
    int id;
    cin >> id;
    person->set_id(id);
    cin.ignore(256, '\n');

    cout << "Enter name: ";
    getline(cin, *person->mutable_name());

    cout << "Enter email address (blank for none): ";
    string email;
    getline(cin, email);
    if (!email.empty()) {
        person->set_email(email);
    }

    while (true) {
        cout << "Enter a phone number (or leave blank to finish): ";
        string number;
        getline(cin, number);
        if (number.empty()) {
            break;
        }

        tutorial::Person::PhoneNumber* phone_number = person->add_phones();
        phone_number->set_number(number);

        cout << "Is this a mobile, home, or work phone? ";
        string type;
        getline(cin, type);
        if (type == "mobile") {
            phone_number->set_type(tutorial::Person::MOBILE);
        }
        else if (type == "home") {
            phone_number->set_type(tutorial::Person::HOME);
        }
        else if (type == "work") {
            phone_number->set_type(tutorial::Person::WORK);
        }
        else {
            cout << "Unknown phone type.  Using default." << endl;
        }
    }
    *person->mutable_last_updated() = TimeUtil::SecondsToTimestamp(time(NULL));
}



int Sender(char* argv)
{
    cout << "Starting protocol buffer - Sender *** encoding ***\n";
    tutorial::AddressBook address_book;

    //{
    //    // Read the existing address book.
    //    fstream input(argv, ios::in | ios::binary);
    //    if (!input) {
    //        cout << argv[1] << ": File not found.  Creating a new file." << endl;
    //    }
    //    else if (!address_book.ParseFromIstream(&input)) {
    //        cerr << "Failed to parse address book." << endl;
    //        return -1;
    //    }
    //}
    // Add an address.

    PromptForAddress(address_book.add_people());

    {
        // Write the new address book back to disk.
        //fstream output(argv, ios::out | ios::trunc | ios::binary);


        
        if (!address_book.SerializeToOstream(&os)) {
            cerr << "Failed to write address book." << endl;
            return -1;
        }
    }

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}

// Iterates though all people in the AddressBook and prints info about them.
void ListPeople(const tutorial::AddressBook& address_book) {
    for (int i = 0; i < address_book.people_size(); i++) {
        const tutorial::Person& person = address_book.people(i);

        cout << "Person ID: " << person.id() << endl;
        cout << "  Name: " << person.name() << endl;
        if (person.email() != "") {
            cout << "  E-mail address: " << person.email() << endl;
        }

        for (int j = 0; j < person.phones_size(); j++) {
            const tutorial::Person::PhoneNumber& phone_number = person.phones(j);

            switch (phone_number.type()) {
            case tutorial::Person::MOBILE:
                cout << "  Mobile phone #: ";
                break;
            case tutorial::Person::HOME:
                cout << "  Home phone #: ";
                break;
            case tutorial::Person::WORK:
                cout << "  Work phone #: ";
                break;
            default:
                cout << "  Unknown phone #: ";
                break;
            }
            cout << phone_number.number() << endl;
        }
        if (person.has_last_updated()) {
            cout << "  Updated: " << TimeUtil::ToString(person.last_updated()) << endl;
        }
    }
}


int Reciever(char* argv)
{
    cout << "Starting protocol buffer - Reciever *** DEcoding ***\n";
    tutorial::AddressBook address_book;

    {
        // Read the existing address book.
        //fstream input(argv, ios::in | ios::binary);
        std::istringstream iss(os.str());
        std::istream& input(iss);
        if (!address_book.ParseFromIstream(&input)) {
            cerr << "Failed to parse address book." << endl;
            return -1;
        }
    }

    ListPeople(address_book);

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
// Main function:  Reads the entire address book from a file,
//   adds one person based on user input, then writes it back out to the same
//   file.
int main(int argc, char* argv[]) {
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (argc != 3) {
        cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
        return -1;
    }
    vector < string> invec = { argv[1], argv[2] };
    string enc = "-E";
    string dec = "-D";
    std::vector<string>::iterator it = std::find_if(invec.begin(), invec.end(), [&](const string& val) {
        if (val == "-E")
            return true;
        return false;
        });

    if (it != invec.end())
        Sender(argv[2]);

   // else
        Reciever(argv[2]);


}
