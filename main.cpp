//
//  main.cpp
//  URLparse
//
//  Created by Erik Rodriguez on 2/8/21.
//

#include <iostream>
#include <string>
using namespace std;

bool isValidProtocol(string protocol);
bool isValidDomain(string& domain, string& port);
bool isValidFilePath(string& filepath, string& parameters);
void URLparse(char url[]);
bool isValidPort (string& s);
bool isValidTLD (string TLD);
bool isValidDomainToken1(string token1);
bool isValidDomainToken2(string token2);
bool isValidDomainToken3(string token3, string& port);

/**
 * Driver Code
 * @return
 */
int main() {

    cout << "Welcome to URL Parser " << endl;
    cout << "Enter a URL: "; string line; getline(cin, line);
    cout << "\n";

    char *url = &line[0];
    URLparse(url);
}


/**
 * Function that parses a string in character array form
 * @param url character array
 */
void URLparse(char url[])
{
    // "protocol://www.domiain.com:80/file-path.html?paramters";
    string domain = "", filepath = "", protocol = "", parameters = "", port = "";

    protocol = strtok(url, "://");
    domain = strtok(NULL, "/"); // domain could contain port
    filepath = strtok(NULL, ""); // file path could contain parameters

    if (!isValidProtocol(protocol)){
        cout << protocol << " is not a valid protocol" << endl;
    }
    if (!isValidDomain(domain, port)) {
        cout << domain << " is an invalid domain name." << endl;
    }
    if (!isValidPort(port)){
        cout << "port number must be between 1 and 65535 " << endl;
    }
    if (!isValidFilePath(filepath, parameters)) {
        cout << filepath << " is an invalid filepath." << endl;
    }

    if (isValidProtocol(protocol) && isValidDomain(domain, port) && isValidFilePath(filepath, parameters)){
        cout << "Protocol:     " << protocol << endl;
        cout << "Domain:       " << domain << endl;
        if (port.length() != 0)
            cout << "Port:         " << port << endl;
        cout << "Filepath:     " <<filepath << endl;
        if (port.length() != 0)
            cout << "Parameters:   " << parameters << endl;
    }

}

/**
 * Checks if the protocol is valid
 * @param s
 * @return
 */
bool isValidProtocol(string s)
{
    string protocols[] = {"http", "https", "ftp", "ftps"};

    for (int i=0; i < sizeof(protocols)/sizeof(protocols[0]); i++) {
        if (s.compare(protocols[i]) == 0) {
            return true;
        }
    }

    return false;
}

/**
 * Checks if a domain is valid
 * @param s the domain
 * @param port
 * @return
 */
bool isValidDomain(string& domain, string &port)
{
    // ex www.google.com <token 0>.<token 1>.<token 2>

    string tokens[3];

    int currentToken = 0;
    int traversalPointer = 0;

    string s = domain; // we made a copy of the domain string to find its component

    while (currentToken < 3){ // if we traverse entire string or we finihsed filling the array

        if (s.at(traversalPointer) == '.'){ // change to next string
            currentToken++;
        }
        else {

            tokens[currentToken] += s.at(traversalPointer); // add character to current string
        }

        traversalPointer++;
        if (traversalPointer == s.length()) // exit
            break;
    }

    int portLocation = domain.find(":");
    if (portLocation != string::npos){ // if the domain has port
        domain = domain.substr(0, portLocation); //updates the current domain
    }


    return (isValidDomainToken1(tokens[0]) && isValidDomainToken2(tokens[1]) && isValidDomainToken3(tokens[2], port));

}

/**
 * subfunction of validate domain
 * checks if the first token of a domain is valid
 * @param token1
 * @return
 */
bool isValidDomainToken1(string token1) {
    return true;
}

/**
 * subfunction of validatedomain
 * checks if the second token of a domain is valid
 * @param token2
 * @return
 */
bool isValidDomainToken2(string token2) {
    return true;
}

/**
 * subfunction of validatedomain
 * Checks if the third token is valid and also
 * parses the port of the function if it has any
 * @param token3
 * @param port
 * @return T/F and also parses port if it has any otherwise it will be "" IMPORTANT: It Doesn't validate port;
 */
bool isValidDomainToken3(string token3, string& port) {

    int portLocation = token3.find(":");    // port usually looks like this ":someportnumber"

    string TLD = token3;

    if (portLocation != string::npos){
        port = token3.substr(portLocation+1, token3.length() -1);
        TLD = token3.substr(0,portLocation);
    }

    return isValidTLD(TLD);
}


/**
 * Validates TLD
 * @param TLD
 * @return
 */

bool isValidTLD (string TLD) {
    string entries[] = {"com","edu","org"};

    for (int i=0;i<sizeof(entries)/sizeof(entries[0]);i++) {
        if (entries[i] == TLD) {
            return true;
        }
    }

    return false;
}

/**
 * Checks if a string is a valid port
 * since a port is optional when the string is empty it is also valid
 * @param s
 * @return
 */
bool isValidPort (string &s) {

    if (s.length() == 0) // if the string is empty
        return true;

    try {
        int portNum = stoi(s); // first we cast it to integer

        if (portNum < 0 || portNum > 65353)
        {
            throw -1; // not in valid range
        }
    } catch (int e) {
        return false;
    }
    return true;
}


/**
 *  function that takes a string
    and determines if its a valid
    URL file path
    it also fills parameters if the filepath has any
 * @param filepath
 * @param parameters
 * @return
 */
bool isValidFilePath(string& filepath, string &parameters) {

    string vpe[] = {".htm", ".html"}; // valid path endings

    for (int i=0; i<sizeof(vpe)/sizeof(vpe[0]); i++){

        int endpos = filepath.find(vpe[i]);

        if (endpos != string::npos){ // in other words if filepath has a valid ending

            // now we check for parameters, they start with a '?' ex : ?key1=value1&key2=value2

            int params = filepath.find("?");
            if (params != string::npos) { // if params are found lets extract them
                parameters = filepath.substr(params+1, filepath.length() - 1);
                filepath = filepath.substr(0, params);
            }

            return true;
        }
    }

    return false;
}


