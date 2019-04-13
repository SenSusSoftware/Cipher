#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//function declarations
void read_text(std::ifstream &files,std::ifstream &alpha);
void decrypt_text(std::ifstream &brutekeys,std::ifstream &alpha,std::ifstream &strings);
bool is_extracted(std::vector<std::string> &keys,std::vector<std::string> &messages);
bool check_data(std::ifstream &mess);

bool check_data(std::ifstream &mess){
    if(mess.peek() == std::ifstream::traits_type::eof()){
        return true;
    }else{
        return false;
    }
}

bool is_extracted(std::vector<std::string> &keys,std::vector<std::string> &messages){
    if(!keys.empty() && !messages.empty()){
        return true;
    }else{
        return false;
    }
}

void decrypt_text(std::ifstream &brutekeys,std::ifstream &alpha,std::ifstream &strings){
    //initialize vectors
    std::vector<std::string> keys{};
    std::vector<std::string> messages {};
    std::vector<std::string> decrypted {};

    //initialize help strings
    std::string line {""};
    std::string key {""};
    std::string alphabet {""};
    std::string message {""};
    std::string decrypted_message {""};

    std::string temp_str {};
    std::string control_str {};

    int choice {};
    //output file
    std::ofstream brute_output {"../brute_log.txt"};

    //test files
    if(!brutekeys){
        std::cerr << "Sorry this file can't be opened" << std::endl;
    }else{
        std::cout << "File containing keys for bruteforce exists,extracting keys" << std::endl;
        while(std::getline(brutekeys,line))
            keys.push_back(line);
    }

    if(!alpha){
        std::cerr << "Sorry alphabet file doesn't exist" << std::endl;
    }else{
        std::cout << "Alphabet file exists, extracting alpha " << std::endl;
        while(std::getline(alpha,line))
            alphabet += line;
    }

    if(!strings){
        std::cerr << "Sorry file containing messages doesn't exist" << std::endl;
    }else{
        std::cout << "File with messages exists, extracting data" << std::endl;
        while(std::getline(strings,line)){
            messages.push_back(line);
        }
    }

    if(is_extracted(keys,messages) == true){
        std::cout << "Done" << std::endl;
    }else{
        std::cout << "Failed to extract keys and data" << std::endl;
    }

    std::cout << "Please read the following message !" << std::endl;
    std::cout << "Do you want to use manual mode or bruteforce ? 1/2: ";
    std::cin >> choice;

    if(choice == 1){
        std::cout << "Please enter the key that you want to use 0-26: ";
        std::cin >> choice;
        key = keys.at(choice-1);
        for(std::string msg : messages){
            std::string temp = msg;
            for(char c : temp){
                size_t position = key.find(c);
                if(position != std::string::npos){
                    char new_char {alphabet.at(position)};
                    temp_str += new_char;
                }else{
                    temp_str += c;
                }
                control_str = temp_str;
            }
            temp_str = "";
            decrypted.push_back(control_str);
        }
        std::cout << "\tBruteforce is done  :)\n\tCheck the bruteforce output :)\n";
    }
    else if(choice == 2){
        for(std::string keya : keys){
            std::string test = keya;
            for(std::string msg : messages){
                std::string temp = msg;
                for(char c : temp){
                    size_t position = test.find(c);
                    if(position != std::string::npos){
                        char new_char {alphabet.at(position)};
                        temp_str += new_char;
                    }else{
                        temp_str += c;
                    }
                    control_str = temp_str;
                }
                temp_str = "";
                decrypted.push_back(control_str);
            }
        }
    
        for(std::string outme : decrypted){
            brute_output << outme << std::endl;
        }
    
        std::cout << "\tBruteforce is done :)\n\tCheck the brute_log.txt file !" << std::endl;
    
        brute_output.close();
    }else{
        std::cout << "Unknown option" << std::endl;
    }

    for(std::string msg : decrypted){
        std::string extract = msg;
        brute_output << extract << std::endl;
    }

    brute_output.close();
}

void read_text(std::ifstream &files,std::ifstream &alpha,std::ifstream &msg){

    std::vector<std::string> keys{};
    std::vector<std::string> messages {};
    std::vector<std::string> crypted {};

    std::string line {""};
    std::string key {""};
    std::string alphabet {""};
    std::string message {""};
    std::string crypted_mes {""};
    std::string cme {""};

    std::ofstream crypted_file {"../crypted.txt"};

    int choice {};

    while(std::getline(files,line)){
        keys.push_back(line);
    }

    while(std::getline(alpha,alphabet)){
        std::cout << "Alphabet loaded\n"; 
    }

    std::cout << "Please type in the number of the key you want to use 0-26: ";
    std::cin >> choice;
    key = keys.at(choice-1);

    std::cout << "You have selected key \n" << key << "\n";
    std::cout << "Alphabet -> " << alphabet << "\n";

    while(std::getline(msg,message)){
        messages.push_back(message);
    }

    std::cout << "=#=#=#=#=#=#=#=#=#=" << std::endl;
    for(auto m : messages){
        std::cout << m << std::endl;
    }

    std::cout << "=#=#=#=#=#=#=#=#=#=" << std::endl;
    std::string temp_str {};

    for(std::string msg : messages){
        temp_str = msg;
        for(char c : temp_str){
            size_t position = alphabet.find(c);
            if(position != std::string::npos){
                char new_char {key.at(position)};
                crypted_mes += new_char;
            }else{
                crypted_mes += c;
            }
            cme = crypted_mes;
        }
        crypted_mes = "";
        crypted.push_back(cme);
    }

    if(!crypted_file){
        std::cerr << "Crypted file not created" << std::endl;
    }else{
        std::cout << "Crypted file created" << std::endl;
    }

    std::cout << std::endl;
    
    for(std::string msg : crypted){
        std::string temp_str = msg;
        crypted_file << temp_str << std::endl;
    }

    crypted_file.close();
}

int main(){
    
    std::ifstream alphabet {"../alphabet.txt"};
    std::ifstream keys{"../keys.txt"};
    std::ifstream messages {"../messages.txt"};
    std::ifstream cryptedfile {"../crypted.txt"};

    std::string choice {""};
    
    std::cout << "Do you want to crypt text or to decrypt C/D ? ";
    std::getline(std::cin,choice);
        
    if(choice == "C" || choice == "c"){
        if(check_data(messages) == true){
            std::cout << "Encoding can't be done because there is no data in messages.txt" << std::endl;
        }else{
            read_text(keys,alphabet,messages);
        }
    }else if(choice == "D" || choice == "d"){
        if(check_data(cryptedfile) == true){
            std::cout << "Decryption can't be done because there is no data in crypted.txt" << std::endl;
        }else{
            std::cout << "Decrypt option selected" << std::endl;
            decrypt_text(keys,alphabet,cryptedfile);
        }
    }else{
        std::cout << "Unknown option - please enter C or D" << std::endl;
    }
    
    return 0;
}