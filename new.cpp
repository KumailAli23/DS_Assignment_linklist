#include <iostream>
#include <cstring>
#include <random>
#include <sstream> 
#include <string>  

// EMPSCRIPTEN CHANGE: These headers are for running C++ in the browser
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

using namespace std;

// Global variables remain the same
int score=0;
int life=5;

// The PNode class is unchanged
class PNode
{
    public:
        bool hasplayer;
        char data;
        char related;
        int position;
        PNode*next;
        PNode*prev;

        PNode()
        {
            hasplayer=false;
            data='*';
            position=0;
            prev=nullptr;
            next=nullptr;
        }
};

// The plinklist class is unchanged
class   plinklist
        { public:
            int count=0;
            PNode*head=nullptr;
            PNode*Player=nullptr;
            plinklist()
            {

            }
               
            void insertatend() 
                {   
                    PNode* newNode = new PNode();
                    
                    if (head == nullptr) 
                    {
                        head = newNode;
                           count++;
                        newNode->position=count;
                        newNode->next=nullptr;
                        return; 
                       
                    }
                    else
                    {
                        PNode* temp = head;
                        while (temp->next != nullptr) 
                        {
                        temp = temp->next; 
                        } 
                        temp->next = newNode;
                        newNode->prev = temp;
                        count++;
                        newNode->position=count;
                         if (count==3)
                        {
                        Player=newNode;
                        newNode->hasplayer = true;
                        newNode->data = 'P';                   
                        }
                    }
                }         
            
            void movement(char input) 
            {
                    if (Player == nullptr) {
                        cout << "Error: Player not found in the list." << endl;
                        return;
                    }

                    if (input == 'd') // Move Right
                    { 
                        if (Player->next != nullptr) 
                        {
                            Player->data = '*';
                            Player->hasplayer = false;
                            Player = Player->next;
                            Player->data = 'p';
                            Player->hasplayer = true;
                        } 

                    }
                    else if (input == 'a') // Move Left
                    { 
                        if (Player->prev != nullptr) 
                        {
                            Player->data = '*';
                            Player->hasplayer = false;
                            Player = Player->prev;
                            Player->data = 'p';
                            Player->hasplayer = true;
                        } 
                    }
            }
};

// The ANode class is unchanged
class ANode
{   public:
    bool hasprojectile;
    bool hasalien;
    char data;
    int position;
    ANode*next;
    ANode*prev;

    ANode()
    {
        hasprojectile=false;
        hasalien=false;
        data='*';
        position=0;
        next=nullptr;
        prev=nullptr;
    };
};

// The Alinklist class is unchanged
class Alinklist
{
public:
    ANode* tail = nullptr;
    bool emptycolumn = true;
    bool AMove = false;
    bool PMove = false;
    ANode* head = nullptr;
    ANode* Alien = nullptr;
    ANode* projectile = nullptr;
    int count = 0;
    ANode* current = nullptr;

    Alinklist()
    {
        current = head;
        projectile = nullptr;
    }

    void insertatend(int position)
    {
        ANode* newNode = new ANode();
        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
            count++;
            newNode->position = count;
            current = head;
            return;
        }
        ANode* temp = head;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
        count++;
        newNode->position = count;
        tail = newNode;
    }

    void checkprojetile()
    {
        if (Alien == nullptr || Alien->next == nullptr)
            return;
        ANode* temp = Alien->next;
        if (temp->hasprojectile == true)
        {
            Alien->hasalien = false;
            score++;
            emptycolumn = true;
            AMove = false;
            temp->hasprojectile = false;
            temp->data = '*';
            Alien->data = '*';
            Alien = nullptr;
        }
        else
        {
            AMove = true;
        }
    }

    void spawn()
    {
        int chance;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> halph(1, 2);
        chance = halph(gen);
        if (emptycolumn == true && chance == 2)
        {
            int rng;
            uniform_int_distribution<> dis(1, 3);
            rng = dis(gen);
            switch (rng)
            {
            case 1:
                head->data = 'A';
                head->hasalien = true;
                Alien = head;
                emptycolumn = false;
                break;
            case 2:
                if (head->next != nullptr) {
                    head->next->data = 'A';
                    head->next->hasalien = true;
                    Alien = head->next;
                    emptycolumn = false;
                }
                break;
            case 3:
                if (head->next != nullptr && head->next->next != nullptr) {
                    head->next->next->data = 'A';
                    head->next->next->hasalien = true;
                    Alien = head->next->next;
                    emptycolumn = false;
                }
                break;
            }
        }
    }

    void Alienmovement()
    {
        if (emptycolumn == true)
        {
            this->spawn();
        }
        else
        {
            this->checkprojetile();
            if (Alien == nullptr) return;
            if (Alien->next == nullptr)
            {
                life--;
                Alien->data = '*';
                Alien = nullptr;
                emptycolumn = true;
            }
            else if (AMove == true)
            {
                Alien->hasalien = false;
                Alien->data = '*';
                Alien = Alien->next;
                Alien->hasalien = true;
                Alien->data = 'A';
            }
        }
    }

    void spawnprojectile()
    {
        if (tail == nullptr) return;
        if (tail->data == 'A')
        {
            score++;
            if (Alien != nullptr)
            {
                Alien->data = '*';
                Alien->hasalien = false;
                Alien = nullptr;
                emptycolumn = true;
            }
        }
        else
        {
            tail->hasprojectile = true;
            tail->data = '|';
            projectile = tail;
        }
    }

    void checkalien()
    {
        if (projectile == nullptr || projectile->prev == nullptr) return;
        ANode* temp = projectile->prev;
        if (temp->hasalien == true)
        {
            if (Alien != nullptr)
            {
                Alien->hasalien = false;
                Alien->data = '*';
                Alien = nullptr;
            }
            score++;
            emptycolumn = true;
            PMove = false;
            projectile->hasprojectile = false;
            projectile->data = '*';
            projectile = nullptr;
        }
        else
        {
            PMove = true;
        }
    }

    void preojectilemovement()
    {
        this->checkalien();
        this->checkprojetile();
        if (projectile == nullptr) return;
        if (projectile->prev == nullptr)
        {
            projectile->data = '*';
            projectile = nullptr;
            emptycolumn = true; 
        }
        else if (PMove == true)
        {
            projectile->hasprojectile = false;
            projectile->data = '*';
            projectile = projectile->prev;
            projectile->hasprojectile = true;
            projectile->data = '|';
        }
    }
};

// Global instances remain the same
plinklist p1;
Alinklist columns[5];
char last_input = 0; 
bool game_started = false; // NEW: Tracks if the start screen has been passed

// This function updates the HTML page's text display
void update_display() {
    stringstream ss;

    ss << "Score: " << score << "   Lives: " << life << "\n\n";

    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 5; col++) {
            ANode* temp = columns[col].head;
            for (int i = 0; i < row; i++) {
                if (temp) temp = temp->next;
            }
            if (temp) ss << temp->data;
        }
        ss << "\n";
    }

    PNode* temp = p1.head;
    while(temp) {
        ss << temp->data;
        temp = temp->next;
    }
    ss << "\n\n";
    
    string js_command = "document.getElementById('game_output').textContent = `" + ss.str() + "`;";
    emscripten_run_script(js_command.c_str());
}

// This is the main game loop
void game_loop() {
    // If the game hasn't started, don't run any game logic, just wait.
    if (!game_started) {
        return;
    }
    
    char input = last_input;
    last_input = 0; 

    // NEW: Check for game over conditions FIRST
    if (life <= 0 || input == 'q') {
        // Build a C++ string containing the JavaScript command
        string js_command = "showGameOverScreen(" + to_string(score) + ");";
        // Execute the JavaScript
        emscripten_run_script(js_command.c_str());
        // Stop the game loop entirely
        emscripten_cancel_main_loop();
        return;
    }

    // Process player input and game logic
    if (input == 'a' || input == 'd') {
        p1.movement(input);

        for (int i = 0; i < 5; i++) {
            columns[i].Alienmovement();
            columns[i].preojectilemovement();
        }
    } else if (input == 's') {
        if (p1.Player) {
            int pos = p1.Player->position;
            if (pos > 0 && pos <= 5) {
                columns[pos - 1].spawnprojectile();
            }
        }
    }

    update_display();
}

// This function captures keyboard events from the browser
EM_BOOL key_callback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
    if (eventType == EMSCRIPTEN_EVENT_KEYDOWN) {
        // NEW: The very first key press will start the game
        if (!game_started) {
            game_started = true;
        }
        
        if (strlen(keyEvent->key) > 0) {
            last_input = keyEvent->key[0];
        }
    }
    return EM_TRUE;
}

int main()
{
    for (int i = 0; i < 5; i++)
        p1.insertatend();

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            columns[i].insertatend(i + 1);
        }
    }
    
    // Initial display draw before the game starts
    update_display();

    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback);
    
    emscripten_set_main_loop(game_loop, 0, 1);

    return 0;
}