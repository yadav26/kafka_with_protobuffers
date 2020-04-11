

#include "ProtocolBuffer_Sender_Receiver.h"

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


int Sender(std::ostringstream& os)
{
    cout << "\nPROTOBUF : Starting Protocol buffer - Encoder. \n";
    tutorial::AddressBook address_book;
    PromptForAddress(address_book.add_people());
    if (!address_book.SerializeToOstream(&os) )
    {
        cerr << "Failed to write address book." << endl;
        return -1;
    }
    cout << "PROTOBUF : Finished encoding message, ready to send\n";
    return 0;
}

int Reciever(byte* hrdval)
{
    cout << "\nPROTOBUF : Starting Protocol buffer - Decoder. \n";
    tutorial::AddressBook address_book;
    std::string str2((char*)hrdval);
    std::istringstream iss(str2);
    std::istream& input(iss);
    if (!address_book.ParseFromIstream(&input)) {
        cerr << "\nPROTOBUF : ***Failed to parse message***. " << endl;
        return -1;
    }
    ListPeople(address_book);
    cout << "PROTOBUF : Finished decoding message, successfully\n";
    return 0;
}

void ShutdownProtoLib()
{
    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

}