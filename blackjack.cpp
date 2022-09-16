#include<iostream>
#include<ctime>
#include<string>
#include<vector>

bool running = true;
bool player_turn = true;
int player_score = 0;
bool dealer_turn = true;
int dealer_score = 0;
bool get_score = false;
int deck[52];

/*
void Clear() // very system dependent and not good code but does clean up the look quite a bit
{
    std::cout << "\x1B[2J\x1B[H";
}
*/

    /*
    Entity class that wraps the other two classes
    -creates and shuffles deck
    -gives player and dealer subclass the functions they needs to play
    */

class Entity
{
public:
    std::vector<int> cards;
    std::vector<int> value;
    std::vector<std::string> suit;
    std::vector<std::string> face;

    int total; int ace_count; int hand_count;
    Entity()
    {
        total = 0;
        ace_count = 0;
        hand_count = 0;
    }

    /*
    the deck is a series of integers 0-51 (52 cards total) and each int corresponds with a card
    we check the int and determine the face (Ace-King), the value (1-10), and the suit

    aces are tracked as 11 and then changed to ones if necessary by the ace count tracker
    */

    void deckInit()
    {
        for(int i = 0; i < 52; i++) deck[i] = i; // makes deck
        
        std::srand(std::time(NULL));
        for(int i = 0; i < 52; i++) // shuffles
        {
            int temp = (int)rand() % 51;
            std::swap(deck[temp], deck[i]);
        }
    }

    int findValue(int input)
    {
        while(input > 12){input -= 13;}
        input++;
        if(input > 10)input = 10;
        if(input == 1)input = 11;
        return input;
    }

    std::string findSuit(int input)
    {
        int temp = input / 13;
        if(temp == 0) return "Spades";
        else if(temp == 1) return "Hearts";
        else if(temp == 2) return "Clubs";
        else return "Diamonds";
    }

    std::string findFace(int input)
    {
        std::string face_list[13] =
        {
            "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"
        };
        while(input > 12){input -= 13;}
        std::string output = face_list[input];
        return output;
    }

    void displayCards()
    {
        std::cout << face[face.size() - 1] << " of " << suit[suit.size() - 1] << std::endl;
    }

    void displayTotal()
    {
        while(hand_count < value.size())
        {
            total += value[hand_count];
            hand_count++;
        }
        if(total > 21 && ace_count > 0)
            {total -= 10; ace_count--;}
        std::cout << "Total: " << total << std::endl;
    }

};

/*
these subclasses look complicated but all they are doing is drawing cards and
running throught the "find" functions I made in entity to determine
what card was dealt and then they are displaying to the terminal for the user to see
*/

class Dealer : public Entity
{
public:
    int deal_num = 0;
    void getCards()
    {
        cards.push_back(deck[deal_num]);
        deal_num += 2;
        value.push_back(findValue(cards[cards.size() - 1]));
        if(value[value.size() -1] == 11) ace_count++;
        suit.push_back(findSuit(cards[cards.size() - 1]));
        face.push_back(findFace(cards[cards.size() - 1]));
    }

    void displayTurn()
    {
        getCards(); displayCards(); displayTotal();
    }

    void dealerTurn()
    {
        while (total <= 16) displayTurn();
    }
};

class Player : public Entity
{
public:
    int deal_num = 1;
    void getCards()
    {
        cards.push_back(deck[deal_num]);
        deal_num += 2;
        value.push_back(findValue(cards[cards.size() - 1]));
        if(value[value.size() -1] == 11) ace_count++;
        suit.push_back(findSuit(cards[cards.size() - 1]));
        face.push_back(findFace(cards[cards.size() - 1]));
    }

    void displayTurn()
    {
        getCards(); displayCards(); displayTotal();
    }

    void playerTurn()
    {
        std::string input;
        if(total == 21) player_turn == false;
        else
        {
            std::cout << "\n" << "Hit or Stay?" << "\n" << std::endl;
            std::getline(std::cin, input);
            std::cout << "\n"<< std::endl;
            if(input == "Hit" || input == "hit" || input == "h") displayTurn();
            else if(input == "Stay" || input == "stay" || input == "s")player_turn = false;
            else if(input == "quit") {running = false; player_turn = false; dealer_turn = false;}
            else if(input == "score") {std::cout << "\n" << "Your Score: " << player_score << "   Dealer Score: " << dealer_score << "\n" << std::endl;}
            else {playerTurn();}
        }
    }
};

/*
rest you can deduce by looking at the code, we initialize the game with the start menu
deal the cards and play the game :)
*/

void init()
{
    if (running == true)
    {    
        get_score = false;
        std::string input;
        std::cout << "\n" << "Press Enter to Start" << "\n" << std::endl;
        std::getline(std::cin, input);
        Entity init;
        init.deckInit();
        player_turn = true;
        dealer_turn = true;
        if (input == "quit")
            {running = false; player_turn = false; dealer_turn = false;}
        else if(input == "score") 
        {
            std::cout << "\n" << "Your Score: " << player_score << "   Dealer Score: " << dealer_score << "\n" << std::endl;
            player_turn = false; dealer_turn = false; get_score = true;
        }
    }
}

void endMessage(int input)
{
    std::string message[4] = {"BLACKJACK!", "You Win!", "You Lose!", "Tie!"};

    std::cout << "\n" << message[input] << "\n" << std::endl;
    if(input < 2) player_score++;
    else if(input == 2) dealer_score++;
    player_turn = false;
    dealer_turn = false;
}

int main()
{
    while (running == true)
    {
        init();
        
        Dealer dealer;
        Player player;

        if (running == true && get_score == false)
        {
            //Clear(); if your terminal supports it
            std::cout << "Dealer's Cards: " << std::endl;
            dealer.displayTurn();

            std::cout << "\n\n" << "Your Cards: " << std::endl;
            player.getCards();
            player.displayCards();
            player.displayTurn();
        }

        while (player_turn == true)
        {
            player.playerTurn();
            if (player.total == 21)
            {
                if(player.hand_count == 2)
                    endMessage(0);
                else
                    endMessage(1); //break; //change to break if you want to have the dealer go when you get 21, I like the instant win.
            }
            else if (player.total > 21)
                endMessage(2);
        }

        if(dealer_turn == true)
            std::cout << "\n" << "Dealer's Cards: " << std::endl;

        while (dealer_turn == true)
        {
            if(dealer.total > 21)
                endMessage(1);
            else if(dealer.total > player.total)
                endMessage(2);
            else if(dealer.total == player.total && dealer.total > 16)
                endMessage(3);
            else if (dealer.total < player.total)
                dealer.displayTurn();
        }
    }
}