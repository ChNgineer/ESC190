import sys

class entry:
    def __init__ (self, word, definition, author):
        self.word = str(word)
        self.definition = definition
        self.author = author

    def __str__ (self):
        return (self.word + "\nDefinition: " + self.definition + "\nAuthor: " + self.author + "\n")

N_Psi_dict = {}

def insert_word():
    '''
    Insert a new word parked into the lot by a stocker.
    '''
    word = input("Enter new word : \n")
    definition = input("Enter a definition for " + word + "\n")
    author = input("Enter author: \n")
    new_word = entry(word, definition, author)
    if(word in N_Psi_dict):
        print("That word already exists! Try the replace command.\n")
    else:
        N_Psi_dict[word] = new_word

def delete_word(word):
    '''
    Deletes the word object. Returns -1 if no word with the word is found. Returns -1 if deletion is aborted.
    '''
    if(word not in N_Psi_dict):
        print("404: word not found.\n")
        return -1
    else:
        prompt = input("Are you sure you want to delete " + word + "? (Y/N)\n").upper()
        while(True):
            if(prompt == "Y"):
                print("word successfully deleted.\n")
                del N_Psi_dict[word]
                return 1
            elif(prompt == "N"):
                print("Deletion aborted\n")
                return -1
            else:
                prompt = input().upper()

def replace_word(word):
    '''
    Replaces an existing word with a new word by using identical word.

    TODO: Make replacement for word object append new author or replacement to an array of authors for a word.
    '''
    if(word not in N_Psi_dict):
        print("404: word not found.\n")
        return -1
    else:
        prompt = input("Are you sure you want to replace " + word + "? (Y/N)\n").upper()
        while(True):
            if(prompt == "Y"):
                del N_Psi_dict[word]
                definition = input("Enter new word definition: \n")
                author = input("Enter new author: \n")
                new_word = entry(word, definition, author)
                N_Psi_dict[new_word.word] = new_word
                return 1
            elif(prompt == "N"):
                print("Replacement aborted\n")
                return -1
            else:
                prompt = input().upper

def replace_search_for_word(word):
    '''
    Returns the word object. Returns -1 if no word with the word is found.

    TODO: Implement levenshtein distance for search. 
    '''
    if(word not in N_Psi_dict):
        print("404: word not found.\n")
        return False
    else:
        print("word successfully located:\n")
        print(N_Psi_dict[word])
        return N_Psi_dict[word]

def load_save():
    '''
    Reads attributes of word objects from a text file, and transports them to the N_Psi_dict data segment.
    '''
    if(input("Are you sure you want to load a new save? Current data will be lost. (Y/N)\n").upper() == "Y"):
        N_Psi_dict.clear()
        with open("dictionary.txt") as f:
            data = f.readlines()
            for line in data:
                dat = line.strip().split('\t')
                print(dat)
                word, definition, author = dat[0], dat[1], dat[2]
                N_Psi_dict[word] = entry(word, definition, author)
        return N_Psi_dict

def save():
    '''
    Writes attributes of word objects from the N_Psi_dict data segment to a new save file.
    '''
    f = open("dictionary.txt", "w")
    for i in N_Psi_dict.values():
        f.write(str(i.word) + "\t" + str(i.definition) + "\t" + str(i.author) + "\n")
    f.close()

if(__name__ == "__main__"):
    while(True):
        user = str(input("Enter a command:\n======================\ninsert\tdelete\nreplace\tsearch\nload\tsave\nquit\n======================\n"))
        if(user.lower() == "insert"):
            print("Inserting... Please follow the prompts.\n")
            insert_word()
        elif(user.lower() == "delete"):
            delete_word(input("Enter word to be deleted\n"))
        elif(user.lower() == "replace"):
            replace_word(input("Enter word to be replaced\n"))
        elif(user.lower() == "search"):
            replace_search_for_word(input("Enter word to search\n"))
        elif(user.lower() == "load"):
            clarification = input("Would you like to override your current work? (Y/N)?\n")
            while(True):
                if(clarification.upper() == "Y"):
                    load_save()
                    break
                elif(clarification.upper() == "N"):
                    break
                else:
                    clarification = input("Please answer (Y/N)\n")
        elif(user.lower() == "save"):
            clarification = input("Would you like to overwrite your previous save file? (Y/N)?\n")
            save_state = 0
            while(True):
                if(clarification.upper() == "Y"):
                    print("Saving... Please do not turn off your device!\n")
                    save()
                    save_state = 1
                    break
                elif(clarification.upper() == "N"):
                    break
                else:
                    clarification = input("Please answer (Y/N)\n")
            while(save_state == 1):
                save_and_quit = input("Would you like to quit after saving? (Y/N)?\n")
                if(save_and_quit.upper() == "Y"):
                    sys.exit()
                elif(save_and_quit.upper() == "N"):
                    break
                else:
                    save_and_quit = input("Please answer (Y/N)\n")
        elif(user.lower() == "quit"):
            clarification = input("Are you sure you want to quit (Y/N)?\n")
            while(True):
                if(clarification.upper() == "Y"):
                    sys.exit("Force Quit Program")
                elif(clarification.upper() == "N"):
                    break
                else:
                    clarification = input("Please answer (Y/N)\n")
        else:
            user = input("Please select a command shown above.\n")