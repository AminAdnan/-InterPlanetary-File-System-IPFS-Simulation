#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <Windows.h>
#include <Wincrypt.h>
#include "Btrees.h"
#include "Conversions.h"
#include "machines.h"
#pragma comment(lib, "advapi32.lib")
using namespace std;

void mainPrintRoutingTable(CircularLinkedList& obj, int nummachines, string*& ary, string*& keys, int identifierspace);
void mainPrintBtree(CircularLinkedList& obj, int nummachines, string*& ary, string*& keys);
void mainSearchFile(CircularLinkedList& obj, int nummachines, string*& ary, int identifierspace);
void mainAddFiles(CircularLinkedList& obj, int nummachines, string*& ary, string*& keys, int identifierspace);
void mainDeleteFile(CircularLinkedList& obj, int nummachines, string*& ary, string*& keys, int identifierspace);
void mainPrintMachines(CircularLinkedList& obj);
void mainDeleteMachine(CircularLinkedList& obj, int nummachines, string*& ary, string*& keys, int identifierspace, int order);
void mainAddMachine(CircularLinkedList& obj, int nummachines, string*& ary, string*& keys, int identifierspace, int order);



void main()
{
    srand(time(0));

    // Welcome message and prompt for identifier space
    cout << "\n\n...................................WELCOME TO OUR INTERPLANETARY FILE SYSTEM (IPFS)...................................\n\n";
    cout << "                               Project By: Hamna Arshad, Arrij Fawwad and Amin Adnan\n\n\n";
    cout << "   To begin with, the Initial Configuration.\n   Please meet the prerequisites.\n\n";
   
   
    
    
    // Initialize identifier space variable
    int identifierspace = 0;
    cout << "\n   Enter the required Identifier Space [bits]: ";
    cin >> identifierspace;

    // Ask user for the order of B-Tree
    int sizeofft = 0, order = 0;
    sizeofft = identifierspace;
    cout << "\n   Enter the order for your B-Tree: ";
    cin >> order;

    // Calculate the total number of machines possible in the system (2^identifierspace)
    string totalmachines = power(identifierspace);
    string NumMachines = "0";
    // Take input for the total number of machines
    int nummachines = 0;

    cout << "\n   Enter the total number of machines that your system needs: ";
    do
    {
        cin >> nummachines;
        NumMachines = to_string(nummachines);
    } while (greaterthanequalto(NumMachines,totalmachines));



    //////////////////////////////////////////SET NAMES FOR ALL THE MACHINES//////////////////////////////////////////////
    // Prompt the user if they want to enter machine names
    char check;
    cout << "\n   Are you willing to input the names of your machines? [y/n]: ";
    cin >> check;

    // Initialize arrays to store machine names and keys
    string* ary = new string[nummachines]; // Stores the names of all the machines
    string* keys = new string[nummachines];      // Stores the keys for all the machines
    string shamachines = "";
    string temp = "";
    string temp2 = "";

    // Ask user to insert machine names, else manually assign machine ids/names
    switch (check) {
    case 'Y':
    case 'y':
        cin.ignore();
        // Loop to input machine names and calculate corresponding keys
        for (int i = 0; i < nummachines; i++) {
            cout << "\n\tEnter name of machine " << i + 1 << ": ";
            getline(cin, ary[i]);
            temp = calculateSHA1(ary[i]);  // SHA in hexadecimal
            keys[i] = GenerateKey(temp, identifierspace); // SHA and the identifier space
            cout << "\tkey: " << keys[i] << endl << endl;
        }
        break;

    case 'N':
    case 'n':
        bool flag = 0;
        cout << "\n   All machines have names allocated to them automatically. \n\n";
        // If names are not entered, they are automatically assigned
        for (int i = 0; i < nummachines; i++) {
            int num = rand() % 100;
            ary[i] = "MACHINE " + to_string(i + 1) + to_string(num);
            temp = calculateSHA1(ary[i]);  // SHA in hexadecimal
            keys[i] = GenerateKey(temp, identifierspace); // SHA and the identifier space

            for (int j = 0; j < i; j++) {
                if (keys[i] == keys[j]) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                i--;
                flag = 0;
                continue;
            }
            cout << "\t" << ary[i];         //print the automatically assigned names to the user
            cout << "\tkey: " << keys[i] << endl << endl;
        }

        break;
    }

    //////////////////////////////////////////////////SETTING UP THE Btree FingerTable and Folder for each MACHINES///////////////////////////////////////////////

    // Insert nodes to create a circular list of machines
    CircularLinkedList obj;

    for (int i = 0; i < nummachines; i++) {
        obj.insert(keys[i], ary[i]); // ary stores the names for the machines
    }
    obj.setfolder();  // Create folders for all the machines
    obj.setfingertable(sizeofft, identifierspace); // Set finger tables for all machines
    obj.set_btree(order); // Create B-Trees for all machines
    


   //////////////////////////////////////////////          WORKING OF IPS         //////////////////////////////////////////////////////////////////////////////////////////////////////////
    cout << "\n\n   Your system is prepared to operate.\n   Let's get started with the working!\n\n\tPress any Key to continue\n";
    cin >> check;
    int option = 9;

    do 
    {
        cout << "\n   The functions, our Interplanetary File Fystem provides\n";
        cout << "\n\tSelect an option[1-9]: \n";

        cout << "\n\tPress 1: To print the Machines present in our IPFS.\n";
        cout << "\n\tPress 2: To print the Btree of any Machine.\n";
        cout << "\n\tPress 3: To print the RoutingTable of any Machine.\n";
        cout << "\n\tPress 4: To Insert a File\n";
        cout << "\n\tPress 5: To Search a File\n";
        cout << "\n\tPress 6: To Delete a File\n";
        cout << "\n\tPress 7: To Add a Machine\n";
        cout << "\n\tPress 8: To Delete a Machine\n";
        cout << "\n\tPress 9: To Exit\n";

        cout << "\n\tYour Option: ";
        cin >> option;

        if (option > 9)
            option = 9;

        switch (option)
        {
        case 1:
            cout << "\n\n\tMachines in IPFS\n\n";
            mainPrintMachines(obj);
            break;

        case 2:
            mainPrintBtree(obj, nummachines, ary, keys);
            break;

        case 3:
            mainPrintRoutingTable(obj, nummachines, ary, keys,identifierspace);
            break;

        case 4:
            cout << "\tFILE INSERTION\n";
            mainAddFiles(obj, nummachines, ary, keys, identifierspace);
            break;

        case 5:
            cout << "\tSEARCH FILE\n";
            mainSearchFile(obj, nummachines, ary, identifierspace);
            break;

        case 6:
            cout << "\n\tFILE DELETION\n";
            mainDeleteFile(obj, nummachines, ary, keys, identifierspace);
            break;

        case 7:
            mainAddMachine(obj, nummachines, ary, keys, identifierspace, order);
            break;

        case 8:
            mainDeleteMachine(obj, nummachines, ary, keys, identifierspace, order);
            break;

        }

        if(option != 9)
        {
            cout << "\n\n\tPress Enter to Continue\n";
            cin >> check;
        }

    } while (option != 9);

    cout << "\n\n.....................................................THANK YOU.....................................................\n\n";

    // Cleanup dynamic memory
    delete[] ary;
    delete[] keys;

}



/////////////////////////////////////////////////PRINTING THE ROUTING TABLE OF MACHINES////////////////////////////////////////
void mainPrintRoutingTable(CircularLinkedList& obj, int nummachines, string*& ary, string*& keys, int identifierspace)
{
    char choice20;

    cout << "\n\n\tWould you like to print the ALL the routing tables[y/n]: ";
    cin >> choice20;
    if (choice20 == 'Y' || choice20 == 'y')
    {

        cout << "\n\tThe bit size for your finger table is " << identifierspace << endl;
        obj.PrintAllRoutingTable();
        return;
    }
    choice20 = ' ';
    cout << "\n\tWould you like to print the routing table for any machine[y/n]: ";
    cin >> choice20;
    if (choice20 == 'Y' || choice20 == 'y')
    {
        string machinename1;

        // present user names of all machines to pick one
        cout << "\n\tEnter one of the following Machine name: \n";
        mainPrintMachines(obj);

        cin.ignore();
        cout << "\n\tEnter name: ";
        getline(cin, machinename1);

        int machine_num = 0;
        for (int i = 0; i < nummachines; i++) {
            if (ary[i] == machinename1) {
                machine_num = i;        //select the key corresponding to the machine selected
            }
        }
        //  send the machine number to the function
        obj.printroutingtable(keys[machine_num]);
    }
    return;
}

//////////////////////////////////////////////////////PRINTING THE B TREE OF ANY MACHINE///////////////////////////////////////////
void mainPrintBtree(CircularLinkedList& obj, int nummachines, string*& ary, string*& keys)
{
    
        string machinename2;

        // present user names of all machines to pick one
        cout << "\n\tEnter one of the following Machine name: \n";
        mainPrintMachines(obj);

        cin.ignore();
        cout << "\n\tEnter name: ";
        getline(cin, machinename2);


        int machine_num1 = 0;
        for (int i = 0; i < nummachines; i++) {
            if (ary[i] == machinename2) {
                machine_num1 = i;        //select the key corresponding to the machine selected
            }
        }
        //  send the machine number to the function
        obj.printBTree(keys[machine_num1]);

    return;
}

/////////////////////////////////////////////////SEARCHING A FILE USING ROUTING TABLE////////////////////////////////////////
void mainSearchFile(CircularLinkedList& obj, int nummachines, string*& ary, int identifierspace)
{
    bool searchfile = false;
    char choice40 = 'n';
    char choice2 = 'n';
    string machinenamee;
    string filedeckey;
    string filemodkey;

    do
    {
        do
        {
            cout << "\n\tEnter your file key in Hexadecimal: ";
            cin >> filedeckey;
            filemodkey = GenerateKey(filedeckey, identifierspace);//mod
            filedeckey = GenerateFileKey(filedeckey, identifierspace);//decimal
            cin.ignore();

            if(!obj.FileExist(filedeckey))
            {
                cout << "\n\tInvalid Input\n";
                cout << "\n\tWould you like to exit? [y/n]: ";
                cin >> choice2;
                if (choice2 == 'Y' || choice2 == 'y')
                    return;
            }

        } while (!obj.FileExist(filedeckey));
         


        cout << "\n\tWould you like to start searching from a specific machine[y/n]: ";
        cin >> choice40;

        if (choice40 == 'Y' || choice40 == 'y')
        {
            // present user names of all machines to pick one
            cout << "\n\tEnter one of the following Machine name: \n";
            mainPrintMachines(obj);

            cin.ignore();
            cout << "\tEnter name: ";
            getline(cin, machinenamee);
        }
        else
        {
            machinenamee = obj.head->machinename;
        }


        obj.SearchFileByFT(machinenamee, filemodkey, filedeckey, identifierspace);

        char check21;
        cout << "\tWould you like to search for more files[y/n]: ";
        cin >> check21;
        if (check21 == 'y' || check21 == 'Y') 
        {
            searchfile = true;
        }
        else 
        {
            searchfile = false;
        }

    } while (searchfile);
    return;
}

///////////////////////////////////////////FOR STORING THE GIVEN FILES INTO THE SYSTEM/////////////////////////////////////
void mainAddFiles(CircularLinkedList& obj, int nummachines, string*& ary, string*& keys, int identifierspace)
{
    bool inputfile = false;
    do
    {
        // Input file name and path
        string filename = "";
        cout << "\n\tEnter name for file to input into the system: ";
        cin >> filename;


        // Read data from the file and calculate SHA1
        char filepath[200];
        cout << "\n\tEnter your file path: ";
        cin >> filepath;
        cin.ignore();

        string keynum = "";
        char choice1;
        cout << "\n\tWould you like to manually assign the request to any one of the machines[y/n]: ";
        cin >> choice1;
        string machinename = "";
        switch (choice1) {
        case 'Y':
        case 'y':

            cout << "\n\tEnter one of the following machines to send the insertion request to: \n";
            mainPrintMachines(obj);

            cin.ignore();
            cout << "\n\tEnter name: ";
            getline(cin, machinename);

            // cout << "Input machine name: " << machinename << endl;

            for (int i = 0; i < nummachines; i++) {
                if (ary[i] == machinename) {
                    keynum = keys[i];        //select the key corresponding to the machine selected
                    break;
                }
            }

            break;
        case 'N':
        case 'n':

            CLLNode * ptr = obj.returnhead_ADD();
            machinename = ptr->machinename;  //automatically assign the request to the first machine
            keynum = ptr->machinekey;
            break;
        }


        // Create Record object, read file, and calculate hash value
        Record r(filepath);
        r.record_readfile();
        string hashval = calculateSHA1(r.shastring);
        string key_for_file = GenerateKey(hashval, identifierspace); // SHA and the identifier space
        cout << "\n\t\tThe File hash: " << hashval << endl;

        // Display the entire content of the file
       // cout << endl << r.shastring << endl;
        cout << "\n\t\tThe File Key: " << key_for_file << endl;
        // Convert hash value to binary and decimal for file insertion into B-Tree
        string binary = hex_binary(hashval, identifierspace); // For the file
        string decimal = bin_dec(binary, identifierspace);    // For the file
        //  string path = obj.insert_file_btree(key_for_file, filename, decimal, keynum, identifierspace); // Insert the file into the appropriate position in the B-Tree
        string path;
        //string temp = obj.insert_file_routingtable(key_for_file, filename, decimal, keynum, identifierspace, path); // Insert the file into the appropriate position in the B-Tree
        string temp = obj.insert_file_btree(key_for_file, filename, decimal, keynum, identifierspace); // Insert the file into the appropriate position in the B-Tree

        r.createfile(r.shastring, temp);//creates actual file on the given path
        // cin.ignore();
        char check10;
        cout << "\n\tWould you like to insert more files[y/n]: ";
        cin >> check10;
        if (check10 == 'y' || check10 == 'Y') {
            inputfile = true;
        }
        else {
            inputfile = false;
        }
    } while (inputfile);

}

void mainPrintMachines(CircularLinkedList& obj)
{
    // Print circular list
    obj.print();
}

///////////////////////////////////////////FOR DELETION THE GIVEN FILE FROM THE SYSTEM/////////////////////////////////////
void mainDeleteFile(CircularLinkedList& obj, int nummachines, string*& ary, string*& keys, int identifierspace)
{
   
    bool deletefile = false;

    do
    {
        cin.ignore();
        // Input file name and path
        string filesha = "";
        cout << "\n\tEnter key of the file to delete in hexadecimal: ";
        cin >> filesha;

        string keynum1 = "";
        char choice2;
        cout << "\n\tWould you like to manually assign the request to any one of the machines[y/n]: ";
        cin >> choice2;
        string machinename1 = "";
        switch (choice2) {
        case 'Y':
        case 'y':

            cout << "\tEnter one of the following machines to send the deletion request to: \n";
            mainPrintMachines(obj);

            cin.ignore();
            cout << "\tEnter name: ";
            getline(cin, machinename1);

            // cout << "Input machine name: " << machinename << endl;

            for (int i = 0; i < nummachines; i++) {
                if (ary[i] == machinename1) {
                    keynum1 = keys[i];        //select the key corresponding to the machine selected
                    break;
                }
            }

            break;
        case 'N':
        case 'n':

            CLLNode * ptr = obj.returnhead_ADD();
            machinename1 = ptr->machinename;  //automatically assign the request to the first machine
            keynum1 = ptr->machinekey;
            break;
        }

        string key_for_file1 = GenerateKey(filesha, identifierspace); // SHA and the identifier space

        // Display the entire content of the file
        cout << "\tkey_for_file: " << key_for_file1 << endl;
        // Convert hash value to binary and decimal for file deletion into B-Tree
        string binary = hex_binary(filesha, identifierspace); // For the file
        string decimal = bin_dec(binary, identifierspace);    // For the file
        string path;
        obj.delete_file_btree(key_for_file1, decimal, keynum1, identifierspace,path); // Insert the file into the appropriate position in the B-Tree
        
        Record rec;
        rec.deleteFile(path);//creates actual file on the given path
        // cin.ignore();
        char check11;
        cout << "\n\tWould you like to delete more files[y/n]: ";
        cin >> check11;
        if (check11 == 'y' || check11 == 'Y') {
            deletefile = true;
        }
        else {
            deletefile = false;
        }
    } while (deletefile);

}

/////////////////////////////////////////////////////////ADD MACHINE////////////////////////////////////////////////////
void mainAddMachine(CircularLinkedList& obj, int nummachines, string*& ary, string*& keys, int identifierspace, int order)
{
    string temp = "";
    string mac;
    cin.ignore();
    cout << "\n\tEnter the name for the machine: ";
    getline(cin, mac);

    nummachines++;   //1 mode machine added to the system
    int index = nummachines - 1;

    string* tempary = ary;
    string* tempkeys = keys;


    //create arrays with the updated/inserted tree
    ary = new string[nummachines];
    keys = new string[nummachines];


    for (int i = 0; i < nummachines - 1; i++) {
        ary[i] = tempary[i];
        keys[i] = tempkeys[i];
    }

    temp = calculateSHA1(mac);  // SHA in hexadecimal
    string k = GenerateKey(temp, identifierspace); // SHA and the identifier space


    ary[nummachines - 1] = mac;  //machine name
    keys[nummachines - 1] = k;    //key for the given machine name

    CLLNode* ptr = obj.AddMachine(k, mac, order, identifierspace);              //machine key, order of btree, machinename

}

///////////////////////////////////////////////////////////DELETE MACHINE////////////////////////////////////////////////////
void mainDeleteMachine(CircularLinkedList& obj, int nummachines, string*& ary, string*& keys, int identifierspace, int order)
{
    


//char choice52;
//cout << "\n\n\tWould you like to delete an existing machine[y/n]: ";
//cin >> choice52;

//if (choice52 == 'y') {
//    temp = "";
//    string mac;
//    cin.ignore();
//    cout << "\n\tEnter the name for the machine: ";
//    getline(cin, mac);






//}
}