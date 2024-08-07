//============================================================================
// Name        : Server.cpp
// Author      : Viktor Cerny
// Version     :
// Copyright   : Your copyright notice
// Description : TCP server - demo
//============================================================================

#include <iostream>
using namespace std;

#include <cstdlib>
#include <cstdio>
#include <sys/socket.h> // socket(), bind(), connect(), listen()
#include <unistd.h> // close(), read(), write()
#include <netinet/in.h> // struct sockaddr_in
#include <strings.h> // bzero()
#include <wait.h> // waitpid()
#include <vector>
#include <cstring>
#include <sstream>

#define BUFFER_SIZE 10240
#define TIMEOUT 3
#define TIMEOUT_RECHARGING 7

bool floating_check(string command, int c) {
    stringstream split(command);
    string buff1, buff2, buff3;
    string dot = ".";

    split >> buff1 >> buff2 >> buff3;

    size_t found = command.find(dot);
    for(int i = 0; i < 3; i++) {
        if(found != std::string::npos) {
            string buffer_send = "301 SYNTAX ERROR\a\b";
            send(c, buffer_send.c_str(), buffer_send.size(), 0);
            return true;
        }
    }
    return false;
}

void direction_func(int x1, int x0, int y1, int y0, int *direction) {
    string buffer_send;
    if(x1 == x0 && y1 == y0) {
        *direction = 0;
        //cout << "BARRIER" << endl;
    } 
    else if(x1 - x0 > 0) {
        *direction = 1;
        //cout << "RIGHT" << endl;
    } else if(x1 - x0 < 0) {
        *direction = 2;
        //cout << "LEFT" << endl;
    } else if(y1 - y0 > 0) {
        *direction = 3;
        //cout << "UP" << endl;
    } else if(y1 - y0 < 0) {
        *direction = 4;
        //cout << "DOWN" << endl;
    } 
}

int main(int argc, char **argv) {

    if(argc < 2){
        cerr << "Usage: server port" << endl;
        return -1;
    }

    // Vytvoreni koncoveho bodu spojeni
    int l = socket(AF_INET, SOCK_STREAM, 0);
    if(l < 0){
        perror("Nemohu vytvorit socket: ");
        return -1;
    }

    int port = atoi(argv[1]);
    if(port == 0){
        cerr << "Usage: server port" << endl;
        close(l);
        return -1;
    }

    struct sockaddr_in adresa;
    bzero(&adresa, sizeof(adresa));
    adresa.sin_family = AF_INET;
    adresa.sin_port = htons(port);
    adresa.sin_addr.s_addr = htonl(INADDR_ANY);

    // Prirazeni socketu k rozhranim
    if(bind(l, (struct sockaddr *) &adresa, sizeof(adresa)) < 0){
        perror("Problem s bind(): ");
        close(l);
        return -1;
    }

    // Oznacim socket jako pasivni
    if(listen(l, 10) < 0){
        perror("Problem s listen()!");
        close(l);
        return -1;
    }

    struct sockaddr_in vzdalena_adresa;
    socklen_t velikost;

    while(true){
        // Cekam na prichozi spojeni
        int c = accept(l, (struct sockaddr *) &vzdalena_adresa, &velikost);
        if(c < 0){
            perror("Problem s accept()!");
            close(l);
            return -1;
        }
        pid_t pid = fork();
        if(pid == 0){
            // Kopie hlavniho vlakna ma vlastni referenci na naslouchajici soket.
            // Podproces, ktery obsluhuje klienta, tuto referenci nepotrebuje, takze je dobre
            // tuto referenci smazat. V hlavnim vlakne samozrejme reference na naslouchajici
            // soket zustava.
            close(l);

            struct timeval timeout;

            fd_set sockets;

            int retval;

            char buffer[BUFFER_SIZE];
            string temp_command;
            string s_buffer;
            string buffer_send;
            vector<string> commands;
            string command;
            int username_is_entered_flag = 0, skip_after_username_flag = 0, invalid_username_flag = 0, confirmation_failed_flag = 0, enter_counter = 0, commands_counter = 0, situation = 0, u_is_entered = 0, hash = 0, u_ascii_sum = 0, u_size = 0, client_hash = 0;
            int server_hash = 0, success = 0, full_power_skip = 1, battery = 1, second_coordinates_is_entered = 0, save_command_counter = 0, in_move = 1, x = 0, y = 0, x_save = 0, y_save = 0, direction = 0, direction0_counter = 0, trap = 0;
            char last_symbol;
            //cout << "USERNAME: " << endl;

            while(true){
                timeout.tv_sec = TIMEOUT;
                timeout.tv_usec = 0;
                FD_ZERO(&sockets);
                FD_SET(c, &sockets);
                // Prvnim parametrem je cislo nejvyssiho soketu v 'sockets' zvysene o jedna.
                // (Velka jednoduchost a efektivvnost funkce je vyvazena spatnou
                // ergonomii pro uzivatele.)
                // Posledni z parametru je samotny timeout. Je to struktura typu 'struct timeval',
                // ktera umoznuje casovani s presnosti na mikrosekundy (+/-). Funkci se predava
                // odkaz na promennou a funkce ji muze modifikovat. Ve vetsine implementaci
                // odecita funkce od hodnoty timeoutu cas straveny cekanim. Podle manualu na
                // to nelze spolehat na vsech platformach a ve vsech implementacich funkce
                // select()!!!
                retval = select(c + 1, &sockets, NULL, NULL, &timeout);
                if(retval < 0){
                    perror("Chyba v select(): ");
                    close(c);
                    return -1;
                }
                if(!FD_ISSET(c, &sockets)){
                    // Zde je jasne, ze funkce select() skoncila cekani kvuli timeoutu.
                    cout << "Connection timeout!" << endl;
                    close(c);
                    return 0;
                }
                int bytesRead = recv(c, buffer, BUFFER_SIZE - 1, 0);
                if(bytesRead <= 0){
                    perror("Chyba pri cteni ze socketu: ");
                    close(c);
                    return -3;
                }
                buffer[bytesRead] = '\0';
                // Pisem otsuda

                // Buffer division    
                if(username_is_entered_flag == 0) {
                    for(int i = 0; i < bytesRead; i++) {
                        if(buffer[i] != '\b' && u_is_entered == 1) {
                            u_is_entered = 0;
                        }
                        if(buffer[i] == '\a' && u_is_entered == 0) {
                            u_is_entered++;
                        } 
                        if(buffer[i] == '\b' && u_is_entered == 1) {
                            u_is_entered++;
                            username_is_entered_flag = 1;
                            last_symbol = buffer[i];
                            break;
                        }
                        u_ascii_sum += static_cast<int>(buffer[i]);
                        u_size++;
                        if(u_size >= 18) {
                            invalid_username_flag = 1;
                            buffer_send = "301 SYNTAX ERROR\a\b";
                            send(c, buffer_send.c_str(), buffer_send.size(), 0);
                            close(c);
                            break;
                        } 
                    }               
                    //cout << buffer;
                    if(u_is_entered == 2) {
                        u_size -= 1;
                        u_ascii_sum -= 7;
                        //cout << "USERNAME ASCII SUM: " << u_ascii_sum << endl;
                        if(u_size >= 18) {
                            invalid_username_flag = 1;
                            buffer_send = "301 SYNTAX ERROR\a\b";
                            send(c, buffer_send.c_str(), buffer_send.size(), 0);
                            close(c);
                            break;
                        } 
                        if(invalid_username_flag != 1) {
                            hash = (u_ascii_sum * 1000) % 65536;
                            //cout << "HASH: " << hash << endl;
                            buffer_send = "107 KEY REQUEST\a\b";
                            send(c, buffer_send.c_str(), buffer_send.size(), 0);
                            skip_after_username_flag = 1;
                        }     
                    }
                }
                if(username_is_entered_flag == 1) {
                    if((buffer[0] == 'O' && buffer[1] == 'K' && bytesRead > 12)) {
                        buffer_send = "301 SYNTAX ERROR\a\b";
                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                        close(c);
                        break;
                    }
                    if(temp_command != "\b") {
                        s_buffer = temp_command + buffer;
                    } else {
                        s_buffer = buffer;
                    }
                    if(situation == 6 && s_buffer.size() > 98) {
                        buffer_send = "301 SYNTAX ERROR\a\b";
                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                        close(c);
                        break;
                    }
                    temp_command.clear();
                    size_t to = s_buffer.find("\a\b");
                    size_t from = 0;
                    if(bytesRead == 3) {
                        enter_counter++;
                    }
                    while (to != string::npos) {
                        int ascii = 0;
                        if(from == 0) {
                            command = s_buffer.substr(from, to);
                        } else {
                            command = s_buffer.substr(from, to - from);
                        }
                        enter_counter++;
                        for(int i = 0; i < bytesRead; i++) {
                            ascii += static_cast<int>(buffer[i]);
                        }
                        if(situation == 6) {
                                if(command == "FULL POWER" || command == "RECHARGING") {
                                    buffer_send = "302 LOGIC ERROR\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    close(c);
                                    break;
                                } else { 
                                    commands_counter++;
                                    commands.push_back(command); 
                                    cout << command << endl;
                                }
                        } else if((enter_counter == 1 && static_cast<int>(buffer[0]) != 8 && static_cast<int>(buffer[0]) != 7) || (bytesRead == 3 && (static_cast<int>(buffer[0]) < 48 || static_cast<int>(buffer[0]) > 57))) {
                            //cout << "SKIP: " << command << endl;
                        } else {
                            if(((static_cast<int>(buffer[0]) != 8 && bytesRead == 1) && (static_cast<int>(buffer[0]) != 7 && bytesRead == 1)) || (ascii == 15 && enter_counter == 1) || command.size() == 0 || s_buffer == to_string(last_symbol)) {} else {
                                if(full_power_skip == 0) {
                                    full_power_skip = 1;
                                }
                                if(command == "RECHARGING" && situation < 6) {
                                    timeout.tv_sec = TIMEOUT_RECHARGING;
                                    battery = 0;
                                } else if(command == "FULL POWER" && situation < 6 && battery == 0) {
                                    timeout.tv_sec = TIMEOUT;
                                    battery = 1;
                                    full_power_skip = 0;
                                } else if((command == "FULL POWER" && battery == 1) || (command == "RECHARGING" && battery == 0)) {
                                    buffer_send = "302 LOGIC ERROR\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    close(c);
                                    break;
                                } else {
                                    commands_counter++;
                                    commands.push_back(command); 
                                }
                                cout << command << endl;
                                if(skip_after_username_flag == 1 && command != "RECHAGNING" && command != "FULL POWER") {
                                    situation = 1;
                                    skip_after_username_flag = 0;
                                }
                            }
                        }
                        from = to + 2;
                        to = s_buffer.find("\a\b", to + 1);
                    }
                    if(from <= s_buffer.size() - 1) {
                        if(commands_counter == 0) {
                            temp_command = s_buffer.substr(from, s_buffer.size() - from);
                        } else {
                            temp_command = s_buffer.substr(from, s_buffer.size() - from);
                        }
                    }
                    //cout << "TEMP: " << temp_command << endl;
                }     
                
                if(battery == 1) {
                    
                // Hash counting    
                    if(situation == 1 && full_power_skip == 1) {
                        server_hash = hash;
                        client_hash = hash;
                        stringstream str_client_key;
                        str_client_key << commands[0];
                        if(!isdigit(commands[0][0]) && commands[0][1] != '\a' && commands[0][2] != '\b') {
                            buffer_send = "301 SYNTAX ERROR\a\b";
                            send(c, buffer_send.c_str(), buffer_send.size(), 0);
                            close(c);
                            break;
                        }
                        int client_key;
                        try {
                            str_client_key >> client_key;
                            if(commands[0].size() > 3) {
                                buffer_send = "301 SYNTAX ERROR\a\b";
                                send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                close(c);
                                break;
                            } else if(commands[0].size() <= 3 && (client_key > 4 || client_key < 0)) {
                                buffer_send = "303 KEY OUT OF RANGE\a\b";
                                send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                close(c);
                                break;
                            } else {
                                switch(client_key) {
                                    case 0: server_hash += 23019; client_hash += 32037; break;
                                    case 1: server_hash += 32037; client_hash += 29295; break;
                                    case 2: server_hash += 18789; client_hash += 13603; break;
                                    case 3: server_hash += 16443; client_hash += 29533; break;
                                    case 4: server_hash += 18189; client_hash += 21952; break;
                                }
                                server_hash = server_hash % 65536;
                                buffer_send = to_string(server_hash) + "\a\b";
                                send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                situation = 2;
                            }
                        } catch (std::invalid_argument & i) {
                            if(commands[0] != "RECHARGING" && commands[0] != "FULL POWER") {
                                buffer_send = "301 SYNTAX ERROR\a\b";
                                send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                close(c);
                                break;
                            }
                        }
                    } 
                    if(situation == 2 && commands_counter > 1) {
                        client_hash = client_hash % 65536;
                        int confirmation_counter = 0;
                        for(size_t i = 0; i < commands[1].size(); i++) {
                            if(confirmation_counter >= 5) {
                                confirmation_failed_flag = 1;
                                buffer_send = "301 SYNTAX ERROR\a\b";
                                send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                close(c);
                                break;
                            }
                            if(commands[1][i] != 0 && commands[1][i] != 32)  {
                                confirmation_counter++;
                            } else if(commands[1][i] == 0) {
                                break;
                            } else if(commands[1][i] == 32) {
                                confirmation_failed_flag = 1;
                                buffer_send = "301 SYNTAX ERROR\a\b";
                                send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                close(c);
                                break;
                            }
                        }
                        if(to_string(client_hash) == commands[1]) {
                            if(confirmation_failed_flag != 1) {
                                buffer_send = "200 OK\a\b";
                                send(c, buffer_send.c_str(), buffer_send.size(), 0);  
                                success = 1;  
                            }
                        } else {
                            if(confirmation_failed_flag != 1) {
                                buffer_send = "300 LOGIN FAILED\a\b";
                                send(c, buffer_send.c_str(), buffer_send.size(), 0); 
                                success = 0;
                                close(c);
                                break;
                            }
                        }
                        situation = 3;
                    }
                    if(situation == 3 && success == 1) {
                        buffer_send = "102 MOVE\a\b";
                        send(c, buffer_send.c_str(), buffer_send.size(), 0);    
                        situation = 4;
                    }
                    if(situation == 4 && success == 1 && commands_counter >= 3) {
                        if(floating_check(commands[commands_counter - 1], c)) {
                            close(c);
                            break;
                        }
                        stringstream split(commands[commands_counter - 1]);
                        string buff1, buff2, buff3;
                        split >> buff1 >> buff2 >> buff3;

                        if(commands[commands_counter - 1].size() > (buff1.size() + buff2.size() + buff3.size() + 2)) {
                            buffer_send = "301 SYNTAX ERROR\a\b";
                            send(c, buffer_send.c_str(), buffer_send.size(), 0);
                            close(c);
                            break;
                        }

                        if(second_coordinates_is_entered == 0) {
                            x_save = stoi(buff2);
                            y_save = stoi(buff3);
                            buffer_send = "102 MOVE\a\b";
                            send(c, buffer_send.c_str(), buffer_send.size(), 0); 
                            second_coordinates_is_entered = 1;  
                        } else if(second_coordinates_is_entered == 1 && commands_counter >= 4) {
                            x = stoi(buff2);
                            y = stoi(buff3); 
                            second_coordinates_is_entered = 2;
                            situation = 5;
                        }
                    }    

                    if(situation == 5 && commands_counter >= 4 && commands_counter > save_command_counter) {
                        // Split buffer
                        save_command_counter = commands_counter;
                        if(floating_check(commands[commands_counter - 1], c)) {
                            close(c);
                            break;
                        }
                        stringstream split(commands[commands_counter - 1]);
                        string buff1, buff2, buff3;
                        split >> buff1 >> buff2 >> buff3;
                        if(commands[commands_counter - 1].size() > (buff1.size() + buff2.size() + buff3.size() + 2)) {
                            buffer_send = "301 SYNTAX ERROR\a\b";
                            send(c, buffer_send.c_str(), buffer_send.size(), 0);
                            close(c);
                            break;
                        }
                        x = stoi(buff2);
                        y = stoi(buff3);

                        if(in_move == 1 && trap == 0) {
                            direction_func(x, x_save, y, y_save, &direction);
                        }

                        if(trap == 2) {
                            trap = 0;
                        }

                        if(x == 0 && y == 0) {
                            buffer_send = "105 GET MESSAGE\a\b";
                            send(c, buffer_send.c_str(), buffer_send.size(), 0);
                            situation = 6;
                        }

                        // RIGHT = 1, LEFT = 2, UP = 3, DOWN = 4, BARRIER = 0
                        else if(direction != 0) {
                            if(x == 0 && y > 0) {
                                if(direction != 4) {
                                    in_move = 0;
                                    if(direction == 1) {
                                        buffer_send = "104 TURN RIGHT\a\b";
                                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                        direction = 4;
                                    } else if(direction == 2) {
                                        buffer_send = "103 TURN LEFT\a\b";
                                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                        direction = 4;
                                    } else if(direction == 3) {
                                        buffer_send = "104 TURN RIGHT\a\b";
                                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                        direction = 1;
                                    }
                                } else if(direction == 4) {
                                    buffer_send = "102 MOVE\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 1;
                                } 
                            } else if(x == 0 && y < 0) {
                                if(direction != 3) {
                                    in_move = 0;
                                    if(direction == 1) {
                                        buffer_send = "103 TURN LEFT\a\b";
                                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                        direction = 3;
                                    } else if(direction == 2) {
                                        buffer_send = "104 TURN RIGHT\a\b";
                                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                        direction = 3;
                                    } else if(direction == 4) {
                                        buffer_send = "103 TURN LEFT\a\b";
                                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                        direction = 2;
                                    }
                                } else if(direction == 3) {
                                    buffer_send = "102 MOVE\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 1;
                                }
                            }
                            else if(x > 0) {
                                if(direction != 2) {
                                    in_move = 0;
                                    if(direction == 1) {
                                        buffer_send = "104 TURN RIGHT\a\b";
                                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                        direction = 4;
                                    } else if(direction == 3) {
                                        buffer_send = "103 TURN LEFT\a\b";
                                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                        direction = 2;
                                    } else if(direction == 4) {
                                        buffer_send = "104 TURN RIGHT\a\b";
                                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                        direction = 2;
                                    }
                                } else if(direction == 2) {
                                    buffer_send = "102 MOVE\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 1;
                                } 
                            } else if(x < 0) {
                                if(direction != 1) {
                                    in_move = 0;
                                    if(direction == 2) {
                                        buffer_send = "104 TURN RIGHT\a\b";
                                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                        direction = 3;
                                    } else if(direction == 3) {
                                        buffer_send = "104 TURN RIGHT\a\b";
                                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                        direction = 1;
                                    } else if(direction == 4) {
                                        buffer_send = "103 TURN LEFT\a\b";
                                        send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                        direction = 1;
                                    }
                                } else if(direction == 1) {
                                    buffer_send = "102 MOVE\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 1;
                                }  
                            }
                        }    

                        else if((direction == 0 && x == 0) || trap == 1) {
                            //cout << "TRAP" << endl;
                            trap = 1;
                            if(y < 0) {
                                if(direction0_counter == 0) {
                                    buffer_send = "104 TURN RIGHT\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 0;
                                    direction0_counter++;
                                } else if(direction0_counter == 1) {
                                    buffer_send = "102 MOVE\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 1; 
                                    direction0_counter++;
                                } else if(direction0_counter == 2) {
                                    buffer_send = "103 TURN LEFT\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 0;
                                    direction0_counter++;
                                } else if(direction0_counter == 3) {
                                    buffer_send = "102 MOVE\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 1; 
                                    direction0_counter++;
                                } else if(direction0_counter == 4) {
                                    buffer_send = "102 MOVE\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 0;
                                    direction0_counter = 0;
                                    trap = 2;
                                    direction = 3;
                                }
                            } else if(y > 0) {
                                if(direction0_counter == 0) {
                                    buffer_send = "104 TURN RIGHT\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 0;
                                    direction0_counter++;
                                } else if(direction0_counter == 1) {
                                    buffer_send = "102 MOVE\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 1; 
                                    direction0_counter++;
                                } else if(direction0_counter == 2) {
                                    buffer_send = "103 TURN LEFT\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 0;
                                    direction0_counter++;
                                } else if(direction0_counter == 3) {
                                    buffer_send = "102 MOVE\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 1; 
                                    direction0_counter++;
                                } else if(direction0_counter == 4) {
                                    buffer_send = "102 MOVE\a\b";
                                    send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                    in_move = 0;
                                    direction0_counter = 0;
                                    trap = 2;
                                    direction = 4;
                                }
                            }
                        }

                        else if(direction == 0 && trap == 0) {
                            if(direction0_counter == 0) {
                                buffer_send = "104 TURN RIGHT\a\b";
                                send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                in_move = 0;
                                direction0_counter++;
                            } else if(direction0_counter == 1) {
                                buffer_send = "102 MOVE\a\b";
                                send(c, buffer_send.c_str(), buffer_send.size(), 0);
                                in_move = 1; 
                                direction0_counter = 0;
                            }   
                        }

                        x_save = x;
                        y_save = y;
                    }
                    
                    else if(situation == 6 && commands_counter > save_command_counter) {
                        buffer_send = "106 LOGOUT\a\b";
                        send(c, buffer_send.c_str(), buffer_send.size(), 0); 
                        cout << "---" << endl; 
                        close(c);
                        break;
                    }
                }

                // Pokud prijmu "konec" ukoncim aktualni dceriny proces
                if(string("konec") == buffer){
                    break;
                }
            }
            close(c);
            return 0;
        }


        // Aby nam nezustavaly v systemu zombie procesy po kazdem obslouzeneme klientovi,
        // je nutne otestovat, zda se uz nejaky podproces ukoncil.
        // Prvni argument rika, cekej na jakykoliv proces potomka, treti argument zajisti,
        // ze je funkce neblokujici (standardne blokujici je, coz ted opravdu nechceme).
        int status = 0;
        waitpid(0, &status, WNOHANG);

        close(c); // Nove vytvoreny socket je nutne zavrit v hlavnim procesu, protoze by na nem v systemu
        // zustala reference a jeho zavreni v novem procesu by nemelo zadny efekt.
    }

    close(l);
    return 0;
}
