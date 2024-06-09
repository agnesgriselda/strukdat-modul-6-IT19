#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <memory>
#include <thread>
#include <chrono>
#include <limits>

using namespace std;

class Game {
private:
    string judul;
    string genre;
    int tahunRilis;
    string platform;

public:
    Game(string judul, string genre, int tahunRilis, string platform)
        : judul(judul), genre(genre), tahunRilis(tahunRilis), platform(platform) {}

    string getJudul() const {
        return judul;
    }

    string getGenre() const {
        return genre;
    }

    int getTahunRilis() const {
        return tahunRilis;
    }

    string getPlatform() const {
        return platform;
    }
};

class GameLibrary {
private:
    vector<Game> games;

public:
    void addGame(const Game& game) {
        games.push_back(game);
    }

    void listGames() const {
        cout << "List of games:\n";
        for (const auto& game : games) {
            cout << "- " << game.getJudul() << endl;
        }
        cout << endl;
    }

    bool searchGame(const string& judul) const {
        auto it = find_if(games.begin(), games.end(), [&](const Game& game) {
            return game.getJudul() == judul;
        });

        return it != games.end();
    }

    void removeGame(const string& judul) {
        games.erase(remove_if(games.begin(), games.end(), [&](const Game& game) {
            return game.getJudul() == judul;
        }), games.end());
    }

    Game cariGameDariJudul(const string& judul) const {
        auto it = find_if(games.begin(), games.end(), [&](const Game& game) {
            return game.getJudul() == judul;
        });
        if (it != games.end()) {
            return *it;
        } else {
            return Game("", "", 0, "");
        }
    }

    void tampilkanDetailGame(const Game& game) const {
        cout << "Game Details:\n";
        cout << "- Title: " << game.getJudul() << endl;
        cout << "- Genre: " << game.getGenre() << endl;
        cout << "- Release Year: " << game.getTahunRilis() << endl;
        cout << "- Platform: " << game.getPlatform() << endl;
        cout << endl;
    }

    void saveGamesToFile(const string& username) const {
        ofstream outFile(username + ".txt");
        for (const auto& game : games) {
            outFile << game.getJudul() << " " << game.getGenre() << " " << game.getTahunRilis() << " " << game.getPlatform() << endl;
        }
        outFile.close();
    }

    void loadGamesFromFile(const string& username) {
        ifstream inFile(username + ".txt");
        string judul, genre, platform;
        int tahunRilis;
        games.clear();
        while (inFile >> judul >> genre >> tahunRilis >> platform) {
            addGame(Game(judul, genre, tahunRilis, platform));
        }
        inFile.close();
    }
};

class User {
protected:
    string username;
    string password;

public:
    User(string uname, string pwd) : username(uname), password(pwd) {}

    string getUsername() const {
        return username;
    }

    string getPassword() const {
        return password;
    }

    bool authenticate(string pwd) {
        return password == pwd;
    }
};

class GameManagementSystem {
private:
    vector<unique_ptr<User>> users;
    GameLibrary& library;

public:
    GameManagementSystem(GameLibrary& lib) : library(lib) {}

    void addUser(unique_ptr<User> user) {
        users.push_back(std::move(user));
    }

    User* findUser(const string& uname, const string& pwd) {
        for (const auto& user : users) {
            if (user->getUsername() == uname && user->authenticate(pwd)) {
                return user.get();
            }
        }
        return nullptr;
    }

    void displayUsers() const {
        for (const auto& user : users) {
            cout << "Username: " << user->getUsername() << endl;
        }
    }

    bool isUsernameTaken(const string& uname) const {
        for (const auto& user : users) {
            if (user->getUsername() == uname) {
                return true;
            }
        }
        return false;
    }

    void saveUsersToFile() const {
        ofstream outFile("creds.txt");
        for (const auto& user : users) {
            outFile << user->getUsername() << " " << user->getPassword() << endl;
        }
        outFile.close();
    }

    void loadUsersFromFile() {
        ifstream inFile("creds.txt");
        string uname, pwd;
        while (inFile >> uname >> pwd) {
            addUser(make_unique<User>(uname, pwd));
        }
        inFile.close();
    }

    void saveUserGames(const string& username) const {
        library.saveGamesToFile(username);
    }

    void loadUserGames(const string& username) {
        library.loadGamesFromFile(username);
    }

    void listUserGames() const {
        library.listGames();
    }

    Game searchUserGame(const string& judul) const {
        return library.cariGameDariJudul(judul);
    }
};

class LoginRegister {
private:
    GameManagementSystem& gms;
    GameLibrary& library;

    string getPasswordInput() {
        string password;
        cout << "Enter password: ";
        cin >> password;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return password;
    }

    int getChoiceInput() {
        int choice;
        cin >> choice;
        while (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a digit: ";
            cin >> choice;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return choice;
    }

public:
    LoginRegister(GameManagementSystem& system, GameLibrary& lib) : gms(system), library(lib) {}

    void registerUser(string uname, string pwd) {
        if (gms.isUsernameTaken(uname)) {
            cout << "Username is already taken!" << endl;
        } else {
            gms.addUser(make_unique<User>(uname, pwd));
            gms.saveUsersToFile();
            cout << "Registration successful!" << endl;
        }
        this_thread::sleep_for(chrono::seconds(1));
    }

    User* loginUser(string uname, string pwd) {
        return gms.findUser(uname, pwd);
    }

    void run() {
        gms.loadUsersFromFile();
        int choice;
        int gameChoice;
        int listChoice;
        int searchChoice;

        do {
            cout << "Welcome to our Game Store\n";
            cout << "1. Register\n2. Login\n3. Display Users\n4. Exit\n";
            cout << "Choose an option: ";

            choice = getChoiceInput();
            cout << "\n";

            this_thread::sleep_for(chrono::seconds(1));

            if (choice == 1){
                string uname, pwd;
                cout << "Register\n";
                cout << "Username: ";
                getline(cin, uname);
                cout << "Password: ";
                pwd = getPasswordInput();

                registerUser(uname, pwd);
            } else if (choice == 2) {
                string uname, pwd;
                cout << "Login\n";
                cout << "Username: ";
                getline(cin, uname);
                cout << "Password: ";
                pwd = getPasswordInput();
                cout << "\n";

                User* user = loginUser(uname, pwd);
                if (user) {
                    cout << "Login successful!" << endl;
                    cout << "\n";
                    cout << "Welcome " << user->getUsername() << "!" << endl;

                    // Load user games from file
                    gms.loadUserGames(uname);

                    string judul;
                    string genre;
                    int tahunRilis;
                    string platform;
                    do {
                        cout << "Game Options:\n";
                        cout << "1. List games\n2. Add game\n3. Search game\n4. Delete game\n5. Logout\n";
                        cout << "Choose an option: ";

                        gameChoice = getChoiceInput();
                        cout << "\n";
                        Game foundGame("", "", 0, "");

                        switch (gameChoice) {
                            case 1:
                                do {
                                    gms.listUserGames();
                                    cout << "\n";
                                    cout << "4. Exit\n";
                                    cout << "Choose an option: ";
                                    listChoice = getChoiceInput();
                                    switch (listChoice) {
                                        case 4:
                                            break;
                                        default:
                                            cout << "Invalid option!\n";
                                    }
                                } while (listChoice != 4);
                                break;
                            case 2:
                                cout << "Enter game title: ";
                                getline(cin, judul);
                                cout << "Enter genre: ";
                                getline(cin, genre);
                                cout << "Enter release year: ";
                                cin >> tahunRilis;
                                cout << "Enter platform: ";
                                cin >> platform;
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                library.addGame(Game(judul, genre, tahunRilis, platform));
                                // Save user games to file
                                gms.saveUserGames(uname);
                                cout << "Game added successfully!\n";
                                break;
                            case 3:
                                cout << "Enter game title to search: ";
                                getline(cin, judul);
                                foundGame = gms.searchUserGame(judul);
                                if (foundGame.getJudul() != "") {
                                    do {
                                        library.tampilkanDetailGame(foundGame);
                                        cout << "\n";
                                        cout << "4. Exit\n";
                                        cout << "Choose an option: ";
                                        searchChoice = getChoiceInput();
                                        switch (searchChoice) {
                                            case 4:
                                                break;
                                            default:
                                                cout << "Invalid option!\n";
                                        }
                                    } while (searchChoice != 4);
                                } else {
                                    cout << "Game not found!\n";
                                }
                                break;
                            case 4:
                                cout << "Enter game title to delete: ";
                                getline(cin, judul);
                                library.removeGame(judul);
                                // Save user games to file
                                gms.saveUserGames(uname);
                                cout << "Game deleted successfully!\n";
                                break;
                            case 5:
                                cout << "Logging out...\n";
                                gms.saveUserGames(uname);
                                break;
                            default:
                                cout << "Invalid option!\n";
                                break;
                        }
                        cout << "\n";

                        this_thread::sleep_for(chrono::seconds(1));
                    } while (gameChoice != 5);

                } else {
                    cout << "Login failed!" << endl;
                    cout << "\n";
                }
                this_thread::sleep_for(chrono::seconds(1));
            } else if (choice == 3) {
                cout << "All registered users:" << endl;
                gms.displayUsers();
                cout << "\n";
                this_thread::sleep_for(chrono::seconds(1));
            } else if (choice != 4) {
                cout << "Invalid option!" << endl;
                cout << "\n";
                this_thread::sleep_for(chrono::seconds(1));
            }
        } while (choice != 4);
    }
};

int main() {
    GameLibrary library;
    GameManagementSystem gms(library);
    LoginRegister lr(gms, library);

    lr.run();

    return 0;
}
